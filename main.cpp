//Hospital Simulator
//Last Updated: 11PM 12/11/2017

#include "Simulator.h"

Simulator hospital;

int main() 
{

	hospital.main_menu();
	hospital.run_simulation();
	hospital.review_records();


	return 0;
}