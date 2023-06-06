#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "measThread.h"
#include "rangeMeter.h"

static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

LOG_MODULE_REGISTER(measThread);

void measThread_entry(void)
{
	if(i2c_dev == NULL || !device_is_ready(i2c_dev))
	{
		LOG_ERR("i2c bus is not ready");
		k_sleep(K_FOREVER);
	}

	k_msleep(100);

	if(rangemeter_init())
	{
		LOG_INF("vl6180x init error occured");
		k_sleep(K_FOREVER);
	}
	LOG_INF("vl6180x init successfull");

	while(1)
	{
		rangemeter_rangeMeas();
		k_msleep(500);
	}

	k_sleep(K_FOREVER);

}
