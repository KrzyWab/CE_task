#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "vl6180x_api.h"
#include "rangeMeter.h"

LOG_MODULE_REGISTER(rangeMeter);

static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
static VL6180xDev_t rangeMeterDev;
static rangeMeterData_t rangeMeterData;

/*
 * Wrapper used by vl6180x_api as a part of porting it to other projects
 * This function is a basic i2c write call for whole vl6180x_api
 */
int VL6180x_I2CWrite(VL6180xDev_t dev, uint8_t  *buff, uint8_t len)
{
	if(i2c_write(i2c_dev,
				(const uint8_t*)buff,
				(uint32_t)len,
				0x29))
	{
		LOG_ERR("cannot write to rangeMeter");
		return -1;
	}
	return 0;
}

/*
 * Wrapper used by vl6180x_api as a part of porting it to other projects
 * This function is a basic i2c read call for whole vl6180x_api
 */
int VL6180x_I2CRead(VL6180xDev_t dev, uint8_t *buff, uint8_t len)
{
	if(i2c_read(i2c_dev,
				(uint8_t*)buff,
				(uint32_t)len,
				0x29))
	{
		LOG_ERR("cannot read from rangeMeter");
		return -1;
	}
	return 0;

}

/*
 * Wrapper used by vl6180x_api as a part of porting it to other projects
 * This function is a basic delay task used by vl6180x_api blocking functions
 */
void VL6180x_PollDelay(VL6180xDev_t dev)
{
	k_msleep(1);
}

int rangemeter_init()
{
	int ret;
	ret = VL6180x_InitData(rangeMeterDev); /* ret>0 warning, ret<0 error, ret=0, success */
	if(ret < 0)
	{
		LOG_ERR("Cannot init rangemeter, err: %d", ret);
		return -1;
	}
	ret = VL6180x_Prepare(rangeMeterDev);
	if(ret < 0)
	{
		LOG_ERR("Rangemeter not prepared, err: %d", ret);
		return -1;
	}
	return 0;
}

void rangemeter_rangeMeas()
{
	int ret = VL6180x_RangePollMeasurement(rangeMeterDev, &(rangeMeterData.rangeData));
	LOG_INF("Range measurement function ret=%d; distance=%u; error=%u", ret, (uint32_t)(rangeMeterData.rangeData.range_mm), (uint32_t)(rangeMeterData.rangeData.errorStatus));
}
