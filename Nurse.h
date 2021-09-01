#ifndef NURSE_H_
#define NURSE_H_

#include "Caregiver.h"

class Nurse : public Caregiver {
private:
	bool get_patient()
	{
		if (!wait_room->nonserious_queue.empty()) // Nurses can only treat below severity 10, so check that queue.
		{
			the_patient.push(wait_room->nonserious_queue.top()); // Set nurse's patient to the most urgent case in the nonserious queue.
			wait_room->nonserious_queue.pop(); // Remove the patient from the wait room.
			return true; // There was a patient in the wait room that the nurse could attend to.
		}

		else
			return false; // There was nop patient in the wait room the nurse could attend to.
	}
};
#endif