#ifndef US_H_
#define US_H_

#include "state.h"

//Define States
enum{
	US_busy
}US_state_id;

// Declare state functions CA
STATE_define(US_busy);

void US_init();


// STATE Pointer to function
extern void (*US_state)();

#endif
