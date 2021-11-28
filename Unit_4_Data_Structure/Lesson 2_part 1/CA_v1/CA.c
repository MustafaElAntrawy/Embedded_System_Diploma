#include "CA.h"


// Variables
int CA_speed = 0;
int CA_distance = 0;
int CA_threshold = 50;

// STATE Pointer to function
void (*CA_state)();
int US_Get_distance_random(int l, int r, int count);

STATE_define(CA_waiting)
{
	// State Name
	CA_state_id = CA_waiting;

	// State_Action
	CA_speed = 0;
	// DC_Motor(CA_speed)

	// Event_Check
	// US_Get_distance(CA_driving)
	CA_distance = US_Get_distance_random(45, 55, 1);
	(CA_distance <= CA_threshold)? (CA_state = STATE(CA_waiting)): (CA_state = STATE(CA_driving));
	printf("CA_WAiting State: distance = %d Speed = %d\n",CA_distance, CA_speed);
}

STATE_define(CA_driving){
	// State Name
	CA_state_id = CA_driving;

	// State_Action
	CA_speed = 30;
	// DC_Motor(CA_speed)

	// Event_Check
	// US_Get_distance(CA_driving)
	CA_distance = US_Get_distance_random(45, 55, 1);
	(CA_distance <= CA_threshold)? (CA_state = STATE(CA_waiting)): (CA_state = STATE(CA_driving));
	printf("CA_driving State: distance = %d Speed = %d\n",CA_distance, CA_speed);
}

int US_Get_distance_random(int l, int r, int count){
	 int i;
	 for(i = 0; i < count; ++i){
		 int rand_num = (rand() % (r - l + 1)) + l;
		 return rand_num;
	 }
	 return 0;
}






