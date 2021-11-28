#include "DC.h"


// Variables
int DC_speed = 0;

// STATE Pointer to function
void (*DC_state)();

void DC_init(){
	// Init PWM
	printf("DC_init \n");

}
void DC_motor(int s){
	DC_speed = s;
	DC_state = STATE(DC_busy);
	printf("CA --------speed=%d---------> DC\n",DC_speed);
}

STATE_define(DC_idle)
{
	// State Name
	DC_state_id = DC_idle;

	printf("DC_idle State: Speed = %d\n", DC_speed);
}

STATE_define(DC_busy){
	// State Name
	DC_state_id = DC_busy;

	// State_Action
	DC_state = STATE(DC_idle);


	printf("DC_busy State: Speed = %d\n", DC_speed);
}







