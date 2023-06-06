#ifndef __RANGE_METER_H
#define __RANGE_METER_H

#include "vl6180x_def.h"

typedef struct {
	VL6180x_RangeData_t rangeData;
	VL6180x_AlsData_t alsData;
} rangeMeterData_t;

int rangemeter_init();
void rangemeter_rangeMeas();

#endif /* __RANGE_METER_H */
