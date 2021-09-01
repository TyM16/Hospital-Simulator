#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <ios>
#include <vector>
#include <set>
#include <iterator> // Required for inserter()

#include "WaitRoom.h"
#include "Doctor.h"
#include "Nurse.h"
#include "Record.h"
#include "Random.h"

Random rng; //Random number generator,

class Simulator {
private:
	int total_time = 7*24*60; //Total time to run the simulation (in minutes, set to one week).
	int clock;                //Current time.

	WaitRoom *wait_room;  // Pointer to the wait room.
	std::vector<Caregiver *> nurses;      // Vector of nurse objects, the whole staff of nurses.
	std::vector<Caregiver *> doctors;     // Vector of doctor objects, the whole staff of doctors.
	std::set<Record *>       patient_records;  // vector of records, change to a set and insert the members of the vector in wait_room->the_patients.

	int read_int(const std::string &prompt, int low, int high) //Use for inputs of num doctors/nurses and patients per hour.
	{
		if (low >= high) // invalid range
			throw std::invalid_argument("invalid range specified"); //Bad Range.

		std::cin.exceptions(std::ios_base::failbit);
		int num = 0;
		while (true) {
			try {
				while (true) {
					std::cout << prompt;
					std::cin >> num;
					if (num >= low && num <= high) { // within the specified range
						std::cout << std::endl;
						return num;
					}
				}
			}
			catch (std::ios_base::failure) {
				std::cout << "Bad numeric string -- try again\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			}
		}
	}

public:

	Simulator() 
	{
		wait_room = new WaitRoom("simville.txt"); // Initialize a wait room and send in the names from text file for use in program.
	}

	void main_menu() // Menu for the program to take inputs from user and most of the interface.
	{
		std::cout << "Welcome to the ER of Simville!\n\n";
		std::cout << "Our goal is to treat any of the 2000 residents that may come here so lets get started.\n\n";

		double rate = read_int("Please enter the patient arrival rate (per hour, max 60): ", 1, 60);  // Input patient arrival rate
		double arrival_rate = rate / 60.0; // Convert to per minute.

		int num_nurses = (read_int("Please enter the number of nurses on staff (max 10): ", 1, 10)); // Input number of nurses for simulation.
		int num_doctors = (read_int("Please enter the number of doctors on staff (max 10): ", 1, 10)); // Input number of doctors for simulation.

		for (int i = 0; i < num_nurses; i++)  { nurses.push_back(new Nurse()); } // Enter the number of nurses specified into the vector.
		for (int i = 0; i < num_doctors; i++) { doctors.push_back(new Doctor()); } // Enter the number of doctors specified into the vector.

		//Set arrival rate for wait list priority queue.
		wait_room->set_arrival_rate(arrival_rate);

		// Pass reference from wait_room to the doctors and nurses so they can alter patient vectors and update records.
		for (int i = 0; i < num_nurses; i++)  { nurses.at(i)->set_wait_room(wait_room); }
		for (int i = 0; i < num_doctors; i++) { doctors.at(i)->set_wait_room(wait_room); }
	}

	void run_simulation() // Here is where the clock ticks happen and the program loops through for one week.
	{
		// Run the simulation
		for (clock = 0; clock < total_time; ++clock) // Clock starts at 0 and loop the number of times specified by total_time.
		{
			// for each clock tick
			wait_room->update(clock);
			for (int i = 0; i < nurses.size(); i++)  { nurses.at(i)->update(clock); } // Run nurses first for more efficiency when dealing with patients. Doctors should be free for serious cases.
			for (int i = 0; i < doctors.size(); i++) { doctors.at(i)->update(clock); }
		}

	}

	void review_records() // Here is where we allow the user to look at the records of all the patients and give back the stats
	{
		//Total wait time and total visits of all of the patients
		int total_visits = 0;
		int total_time   = 0;
		
		// Puts contents from a vector into a set
		patient_records = wait_room->make_set();

		// This is to get the total number of visits from each patient and the total amount of time spent in the hospital
		for (std::set<Record *>::iterator iter = patient_records.begin(); iter != patient_records.end(); ++iter)
		{
			// Add the number of visits from the current patient into the total
			total_visits = total_visits + (*iter)->number_of_visits;
			// Add the total wait time from the current patient into the total 
			total_time = total_time + (*iter)->time_in_hospital;
		}

		//Where we display the average visit time to the user
		std::cout << "The average visit time was: " << total_time / total_visits << " minutes per visit." << std::endl;
		system("pause");
		system("cls");

		// An expression to check if the the user is finished
		bool done = false;

		// A do-while loop to check if the user would like to continue in the simulator
		do {

			//First the simulator checks to see if you would like to see a list of patients from the week or look at the 
			//records of an individual patient
			if (read_int("Would you like to list the patients or search by name? 0/1: ", 0, 1) == 0)
			{
				int total_records = 0;
				std::cout << "The patients that visited the hospital are: \n\n";

				// Goes through the set and outputs all of the patients from the week
				for (std::set<Record *>::iterator iter = patient_records.begin(); iter != patient_records.end(); ++iter)
				{
					std::cout << (*iter)->name << ": "<<(*iter)->number_of_visits << std::endl;
					total_records++;
				}

				//Output the total number of people who came to the ER in the week (no duplicates)
				std::cout << "\nTotal number of records: " << total_records<< std::endl;
			}

			//Where you can search by an individual name
			else
			{
				std::string name;
				std::cout << "Enter the name of the patient you would like to see: ";
				std::cin >> name;

				//Goes through the set to check if the name was in the records
				for(std::set<Record *>::iterator iter = patient_records.begin(); iter != patient_records.end(); ++iter)
				{ 
					//If the name is in the record it outputs the number of trips to the hospital and the severity of each visit.
					if ((*iter)->name == name) 
					{
						std::cout << "Name: " << (*iter)->name << std::endl;
						std::cout << "Trips to hospital: " << (*iter)->number_of_visits << std::endl;
						std::cout << "Severity of visits in order: " << std::endl;
						//Goes through the vector to get each of the severities for the trip(s)
						for (std::vector<int>::iterator it = (*iter)->severities_per_trip.begin(); it != (*iter)->severities_per_trip.end(); it++) 
						{
							std::cout << *it << std::endl;
						}
					} 
				}
			}

			system("Pause");
			system("cls");
			//Would you like to continue the simulation
			if (read_int("Would you like to exit the simulation? 0(no)/1(yes): ", 0, 1) == 0);
			//1 makes done true and exits the program
			else
				done = true;

		} while (!done);
	}
};
#endif