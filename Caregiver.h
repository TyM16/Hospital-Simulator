#ifndef CAREGIVER_H_
#define CAREGIVER_H_

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include "Random.h"
#include "Patient.h"
#include "WaitRoom.h"
#include "Record.h"

extern Random rng;

class Caregiver 
{
protected:
	std::queue<Patient *> the_patient;  // patient the nurse/doctor is working with.
	int treatment_start = -1;           // Variable to help determine when the patient should leave the caregiver's treatment.
	WaitRoom *wait_room;                // pointer to the wait room.

	virtual bool get_patient() = 0; //Defined differently for nurse and doctor. Gets a patient from wait list to work on.
	
public:

	Caregiver() {} // Default constructor.

	void set_wait_room(WaitRoom *wait_room) //Sets a pointer to the wait room for operations.
	{
		this->wait_room = wait_room;
	}

	void update(int clock) // Loop through with each clock tick (one week in minutes).
	{
		if (the_patient.empty() == true) // If the caregiver has no patient
		{
			if(get_patient()) // get a new patient if there is one in the wait room.
			{
				treatment_start = clock; // Start the proceedure timer to keep track of when the patient is done.
			}
		}

		/* If the affliction has been treated, (proportional to severity,)
		release the patient and make/update record. */

		else if (clock - treatment_start > the_patient.front()->severity) //check if patient is done when the caregiver is tending a patient.
		{
			the_patient.front()->leave_time = clock; // set the last clock tick the patient is at the hospital

			if (wait_room->search_name(the_patient.front()->name)) // If the name is already in records, update it.
			{
				wait_room->update_record(the_patient.front()->name, the_patient.front()->get_total_time(), the_patient.front()->severity);
			}

			else // If not, make a new entry.
			{
				wait_room->patient_records.push_back(new Record(the_patient.front()->name, the_patient.front()->severity, the_patient.front()->get_total_time()));
			}

			the_patient.pop(); // remove the patient from the caregiver's care so a new one can be seen.
		}
	}
};
#endif