#include "CA.h"

void setup(){
	// Init all the drivers
	// Init IRQ..
	// Init HAL US_Driver DC_Driver
	// Init BLOCK
	// Set State pointers for each Block
	CA_state = STATE(CA_waiting);
}

int main(){
	volatile int d;
	setup();
	while(1){
		// Call state for each block
		CA_state();
		for(d = 0; d < 1000; ++d);
	}
	return 0;
}
