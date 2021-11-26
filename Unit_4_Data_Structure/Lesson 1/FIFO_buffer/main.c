#include "fifo.h"

int main(){

	FIFO_Buf_t FIFO_UART;
	element_type i, temp;

	if(FIFO_init(&FIFO_UART , uart_buffer, 5) == FIFO_no_error)
		printf("FIFO init is Done...\n");

	for(i = 0; i < 7; ++i){
		printf("FIFO Enqueue (%x) \n",i);
		if(FIFO_enqueue(&FIFO_UART, i) == FIFO_no_error)
			printf("\t FIFO enqueue is Done...\n");
		else
			printf("\t FIFO enqueue is failed...\n");
	}

	FIFO_print(&FIFO_UART);
	if(FIFO_dequeue(&FIFO_UART, &temp) == FIFO_no_error)
		printf("\t FIFO dequeue %X is Done...\n", temp);
	if(FIFO_dequeue(&FIFO_UART, &temp) == FIFO_no_error)
		printf("\t FIFO dequeue %X is Done...\n", temp);
	FIFO_print(&FIFO_UART);


	return 0;
}
