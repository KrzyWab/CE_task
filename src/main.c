#include <zephyr/kernel.h>

#include "blinkThread.h"
#include "pwmThread.h"

K_THREAD_DEFINE(blinkThread_id, BLINK_THREAD_STACKSIZE,
				blinkThread_entry, NULL, NULL, NULL,
				BLINK_THREAD_PIORITY, 0, 0);

K_THREAD_DEFINE(pwmThread_id, PWM_THREAD_STACKSIZE,
				pwmThread_entry, NULL, NULL, NULL,
				PWM_THREAD_PIORITY, 0, 0);
