#ifndef __DATA_QUEUE_H
#define __DATA_QUEUE_H

#include <zephyr/kernel.h>

/* node for VL6180x FIFO queue that will hold measurement data */
struct rangeDataFifoNode_t
{
	void *fifo_reserved;	// Zephyr RTOS mandatory field, 4B size
	int32_t rangeMeas;		// range data value in [mm]
	uint32_t rangeError;	// range data read error (0 on success)
	uint32_t alsMeas;		// als data value [lux]
	uint32_t alsError;		// als data read error (0 on success)
	int32_t initError;		// VL6180x initialization error (0 on success)
};

extern struct k_fifo rangeDataFifo;


#endif /* __DATA_QUEUE_H */
