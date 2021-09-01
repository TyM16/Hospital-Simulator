#ifndef WAITROOM_H_
#define WAITROOM_H_

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <set>
#include <fstream>
#include "Random.h"
#include "Record.h"
#include "Patient.h"

extern Random rng; // Use global variable based in Simulator

class WaitRoom 
{
private:
	double arrival_rate; // Arrival rate set in Simulator.h by the user (not more than 60 per hour).
	std::priority_queue<Patient *> nonserious_queue;  // queue of patients in the waiting room with non serious injury (less than 10 rating).
	std::priority_queue<Patient *> serious_queue;     // queue of patients in the waiting room with serious injuries (more than 10 rating).
	int num_served = 0;  // number of pateints served through the queue
	std::vector<std::string> potential_patients; // vector of names from text file.
	std::vector<Record *> patient_records;   // Patient records filed for later.


	void init_residents(std::string residents) // Init names for use in program
	{
		std::string line;
		std::ifstream resfile(residents); // Open file based on parameter from Simulator.
		if (resfile.is_open()) // Check if file is open.
		{
			while (getline(resfile, line))
			{
				potential_patients.push_back(line); // Add names to vector
			}
			resfile.close();
		}

		else {
			std::cout << "Unable to open file of residents, exiting program";
			exit(1); // Exit program if file fails to load with code 1.
		}
	}

public:

	WaitRoom(std::string resident_file)// Input the file name to search for
	{  
		init_residents(resident_file);    // Run init function to add names from text file
	}    

	void set_arrival_rate(double rate)
	{
		arrival_rate = rate;  // Set arrival rate of patients based on user input sent from Simulator
	}

	std::vector<Record *> get_records()
	{
		return patient_records;  // To get the patient_records
	}

	bool search_name(std::string name) // To search for the name in patient_record, so that if a record is already made it doesn't make another
	{
		for (std::vector<Record *>::iterator iter = patient_records.begin(); iter != patient_records.end(); iter++)
		{
			if ((*iter)->name == name)
				return true;
		}
		return false;
	}

	std::set<Record *> make_set() // To introduce the set we put the information of the vector into the set
	{
		std::set<Record *> temp_set; // Where to store teh stuffs from teh veector

		for (std::vector<Record *>::iterator iter = patient_records.begin(); iter != patient_records.end(); iter++) // Until it reaches the end of the vector keep inserting stuffs into the set
		{
			temp_set.insert(*iter);
		}

		return temp_set; // Returns the newly made set
	}

	void update_record(std::string name, int total_time, int severity) // Allows the the record to be updated if a patients were to come back to the hospital
	{
		for (std::vector<Record *>::iterator iter = patient_records.begin(); iter != patient_records.end(); iter++)
		{
			if ((*iter)->name == name)
			{
				(*iter)->update_record(total_time, severity);
			}
		}
	}

	void update(int clock)
	{
		//Do we need a new patient based on chance
		if (rng.next_double() < arrival_rate)
		{
			int i;                   

			i = rng.next_int(99);

			//Should have been a name check if in hospital

			if (i < 70) { nonserious_queue.push(new Patient(clock, rng.next_int(9) + 1, potential_patients[rng.next_int(potential_patients.size())])); }                //generate a patient with a 1-10 severity illness at 70% rate.
			else if (i >= 70 || i < 90) { serious_queue.push(new Patient(clock, rng.next_int(4) + 11, potential_patients[rng.next_int(potential_patients.size())])); }  //generate a patient with a 11-15 severity illness at 20% rate.
			else { serious_queue.push(new Patient(clock, rng.next_int(4) + 16, potential_patients[rng.next_int(potential_patients.size())])); }                         //generate a patient with a 16-20 severity illness at 10% rate.

			num_served++; //Increment total number of visits to the hospital.

			//Note: patients are removed by doctors and nurses respectively when they are ready to take on another from the waiting room.
		}
	}

	friend class Caregiver;
	friend class Doctor;
	friend class Nurse;
};
#endif
