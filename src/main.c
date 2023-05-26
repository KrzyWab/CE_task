#include <zephyr/kernel.h>

#include "displayThread.h"

K_THREAD_DEFINE(displayThread_id, DISPLAY_THREAD_STACKSIZE,
				displayThread_entry, NULL, NULL, NULL,
				DISPLAY_THREAD_PIORITY, 0, 0);
