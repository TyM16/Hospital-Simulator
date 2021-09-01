#ifndef Doctor_H_
#define Doctor_H_

#include "Caregiver.h"

class Doctor : public Caregiver {
private:
	bool get_patient() 
	{
		if (!wait_room->serious_queue.empty()) // Check the serious queue for doctors first
		{
			the_patient.push(wait_room->serious_queue.top()); // Add the patient from the wait room serious queue to the doctor's queue.
			wait_room->serious_queue.pop(); // Remove the patient from the wait room serious queue.
			return true; // There was a serious case the doctor could tend to.
		}

		else if (!wait_room->nonserious_queue.empty()) // If no serious case found, proceed to non serious.
		{
			the_patient.push(wait_room->nonserious_queue.top()); // Add the patient from the wait room nonserious queue to the doctor's queue.
			wait_room->nonserious_queue.pop(); // Remove the patient from the wait room nonserious queue.
			return true; // There was a nonserious case the doctor could tend to.
		}
		else
			return false; // There was noone in the wait room.
	}

};
#endif
