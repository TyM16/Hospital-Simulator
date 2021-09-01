#ifndef RECORD_H_
#define RECORD_H_

#include <iostream>
#include <vector>

struct Record
{
public:
	Record(std::string temp_name, int severity, int total_time) 
	{ // Case for new record.
		name = temp_name; // Set the person this record is attatched to.
		severities_per_trip.push_back(severity); // Add the severity to the vector.
		number_of_visits = 1; // Set number of visits to 1 at time of record creation.
		time_in_hospital = total_time; // Total time in hospital, set from patient.
	}

	Record(std::string temp_name) : name(temp_name) {} // Default constructor.

	std::string name; // Person the record belongs to.
	int number_of_visits; // Number of times they visited the hospital.
	int time_in_hospital; // Total time spent in hospital.
	std::vector<int> severities_per_trip; // vectors of severities during visits.

	void update_record(int total_time, int severity) // update existing record. 
	{
		number_of_visits++; // increment number of visits.
		time_in_hospital = time_in_hospital + total_time; // update total time spent in hospital.
		severities_per_trip.push_back(severity); // add a severity for this trip to the list of severities.
	}

};
#endif
