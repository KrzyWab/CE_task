#ifndef __RANGE_METER_H
#define __RANGE_METER_H

#include "vl6180x_api.h"

int rangemeter_init();
void rangemeter_rangeMeas(VL6180x_RangeData_t *rangeData);
void rangemeter_alsMeas(VL6180x_AlsData_t *alsData);

#endif /* __RANGE_METER_H */
