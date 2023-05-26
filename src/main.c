#include <zephyr/kernel.h>

#include "blinkThread.h"
#include "displayThread.h"

K_THREAD_DEFINE(blinkThread_id, BLINK_THREAD_STACKSIZE,
				blinkThread_entry, NULL, NULL, NULL,
				BLINK_THREAD_PIORITY, 0, 0);

K_THREAD_DEFINE(displayThread_id, DISPLAY_THREAD_STACKSIZE,
				displayThread_entry, NULL, NULL, NULL,
				DISPLAY_THREAD_PIORITY, 0, 0);
