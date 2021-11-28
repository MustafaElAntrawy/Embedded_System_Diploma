#include "US.h"
#include "CA.h"
#include "DC.h"


void setup(){
	// Init all the drivers
	// Init IRQ..
	// Init HAL US_Driver DC_Driver
	// Init BLOCK
	US_init();
	DC_init();
	// Set State pointers for each Block
	CA_state = STATE(CA_waiting);
	US_state = STATE(US_busy);
	DC_state = STATE(DC_idle);
}

int main(){
	volatile int d;
	setup();
	while(1){
		// Call state for each block
		US_state();
		CA_state();
		DC_state();
		for(d = 0; d < 1000; ++d);
	}
	return 0;
}
