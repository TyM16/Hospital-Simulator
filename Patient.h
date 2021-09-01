#ifndef PATIENT_H_
#define PATIENT_H_

#include <iostream>

struct Patient
{
public:
	Patient() {}; // Default constructor (not used).
	Patient(int clock, int illness_level, std::string temp_name) : arrival_time(clock), severity(illness_level), name (temp_name), leave_time(-1) {} //Init patient (arrival, sickness level, name)

	std::string name; // Name of patient.
	int severity;     // Severity of affliction.
	int arrival_time; // Time patient arrived at hospital.
	int leave_time; // Time the patient was discharged from the hospital.

	int get_total_time()
	{ 
		return (leave_time - arrival_time); // Total patient time in hospital. Used in record keeping.
	} 


	bool operator<(const Patient &other) const 
	{ // For pqueue to rate based on severity.
		return severity < other.severity;
	}
};
#endif
