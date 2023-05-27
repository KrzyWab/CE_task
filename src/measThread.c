#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "measThread.h"

LOG_MODULE_REGISTER(measThread);

void measThread_entry(void)
{
	k_sleep(K_FOREVER);
}
