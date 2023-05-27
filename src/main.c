#include <zephyr/kernel.h>

#include "displayThread.h"
#include "measThread.h"

K_THREAD_DEFINE(measThread_id, MEAS_THREAD_STACKSIZE,
				measThread_entry, NULL, NULL, NULL,
				MEAS_THREAD_PIORITY, 0, 0);

K_THREAD_DEFINE(displayThread_id, DISPLAY_THREAD_STACKSIZE,
				displayThread_entry, NULL, NULL, NULL,
				DISPLAY_THREAD_PIORITY, 0, 0);
