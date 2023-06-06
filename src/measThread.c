#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "measThread.h"
#include "rangeMeter.h"
#include "dataQueue.h"

static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

LOG_MODULE_REGISTER(measThread, LOG_LEVEL_DBG);

void measThread_entry(void)
{
	struct rangeDataFifoNode_t rangeDataQNode = {0};
	struct rangeDataFifoNode_t *dataBuff = NULL;

	if(i2c_dev == NULL || !device_is_ready(i2c_dev))
	{
		LOG_ERR("i2c bus is not ready");
		dataBuff = k_malloc(sizeof(struct rangeDataFifoNode_t));
		__ASSERT_NO_MSG(dataBuff != NULL);
		rangeDataQNode.initError = -1;
		memcpy(dataBuff, &rangeDataQNode, sizeof(struct rangeDataFifoNode_t));
		k_fifo_put(&rangeDataFifo, dataBuff);
		k_sleep(K_FOREVER);
	}

	k_msleep(100);

	if(rangemeter_init())
	{
		LOG_ERR("vl6180x init error occured");
		dataBuff = k_malloc(sizeof(struct rangeDataFifoNode_t));
		__ASSERT_NO_MSG(dataBuff != NULL);
		rangeDataQNode.initError = -1;
		memcpy(dataBuff, &rangeDataQNode, sizeof(struct rangeDataFifoNode_t));
		k_fifo_put(&rangeDataFifo, dataBuff);
		k_sleep(K_FOREVER);
	}
	LOG_INF("measThread initialized");

	dataBuff = NULL;
	VL6180x_RangeData_t rangeData;
	VL6180x_AlsData_t alsData;

	while(1)
	{
		rangemeter_rangeMeas(&rangeData);
		rangemeter_alsMeas(&alsData);

		
		LOG_DBG("R: %d %u; A: %u %u",
				rangeData.range_mm,
				rangeData.errorStatus,
				alsData.lux,
				alsData.errorStatus);
		

		/* add measurements to FIFO */
		dataBuff = k_malloc(sizeof(struct rangeDataFifoNode_t));
		__ASSERT_NO_MSG(dataBuff != NULL);
		rangeDataQNode.rangeMeas = rangeData.range_mm;
		rangeDataQNode.rangeError = rangeData.errorStatus;
		rangeDataQNode.alsMeas = alsData.lux;
		rangeDataQNode.alsError = alsData.errorStatus;
		memcpy(dataBuff, &rangeDataQNode, sizeof(struct rangeDataFifoNode_t));
		k_fifo_put(&rangeDataFifo, dataBuff);

		k_msleep(100);
	}
}
