#ifndef LIFO_H_
#define LIFO_H_
#include "stdio.h"
#include "stdlib.h"
//Type Definitions
typedef struct LIFO_Buf{
	unsigned int length;
	unsigned int count;
	unsigned int* base;
	unsigned int* head;
}LIFO_BUF_t;

typedef enum{
	LIFO_no_error,
	LIFO_full,
	LIFO_empty,
	LIFO_Null
}LIFO_status;

//APIS
LIFO_status LIFO_add_item(LIFO_BUF_t* lifo_buf,unsigned int  item );
LIFO_status LIFO_get_item(LIFO_BUF_t* lifo_buf,unsigned int*  item );
LIFO_status LIFO_init(LIFO_BUF_t* lifo_buf,unsigned int*  buf, unsigned length);


#endif /* LIFO_H_ */
