#ifndef STATE_H_
#define STATE_H_

#include "stdio.h"
#include "stdlib.h"

// Automatic STATE function generated
#define STATE_define(_statFuN_) void ST_##_statFuN_()
#define STATE(_statFuN_) ST_##_statFuN_

// States Connections

void US_set_distance(int d);
void DC_motor(int s);

#endif /* STATE_H_ */
