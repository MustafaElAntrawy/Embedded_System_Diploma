
#include "lifo.h"

LIFO_status LIFO_add_item(LIFO_BUF_t* lifo_buf,unsigned int  item ){
	//LIFO is valid ?
	if(!lifo_buf->base || !lifo_buf->head) return LIFO_Null;

	//LIFO is full ?
	if (lifo_buf->count == lifo_buf->length) return LIFO_full;

	//Add value
	*(lifo_buf->head) = item;
	lifo_buf->head++;
	lifo_buf->count++;
	return LIFO_no_error;

}
LIFO_status LIFO_get_item(LIFO_BUF_t* lifo_buf,unsigned int*  item ){
	//LIFO is valid ?
	if(!lifo_buf->base || !lifo_buf->head) return LIFO_Null;

	//Check LIFO is empty
	if (lifo_buf->count == 0) return LIFO_empty;

	lifo_buf->head--;
	*item = *(lifo_buf->head);
	lifo_buf->count--;
	return LIFO_no_error;
}
LIFO_status LIFO_init(LIFO_BUF_t* lifo_buf,unsigned int*  buf, unsigned length){
	if(buf == NULL){
		return LIFO_Null;
	}
	lifo_buf->base = buf;
	lifo_buf->head = buf;
	lifo_buf->length = length;
	lifo_buf->count = 0;
	return LIFO_no_error;

}
