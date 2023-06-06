#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include "displayThread.h"
#include "fadingLed.h"
#include "oledDisplay.h"
#include "dataQueue.h"

LOG_MODULE_REGISTER(displayThread, LOG_LEVEL_DBG);

/* This thread is controlling displaying data taken from measurements */
void displayThread_entry(void)
{
	if(fadingLed_init())
	{
		k_sleep(K_FOREVER);
	}
	LOG_INF("Fading LED initialized");

	if(oledDisplay_init())
	{
		k_sleep(K_FOREVER);
	}
	LOG_INF("Display initialized");

	struct rangeDataFifoNode_t *dataBuff;
	while(1)
	{
		dataBuff = k_fifo_get(&rangeDataFifo, K_FOREVER);
		if(dataBuff->initError)
		{
			//TODO Display VL6180x init error on OLED
			LOG_ERR("VL6180x init error");
		}
		else
		{
			oledDisplay_clear();
			oledDisplay_showRange(dataBuff->rangeMeas, dataBuff->rangeError);
			oledDisplay_showAls(dataBuff->alsMeas, dataBuff->alsError);
			oledDisplay_update();
			//TODO Display measurements data on OLED and with PWM LED
			LOG_DBG("R: %d %u; A: %u %u",
					dataBuff->rangeMeas,
					dataBuff->rangeError,
					dataBuff->alsMeas,
					dataBuff->alsError);
		}
		k_free(dataBuff);
	}
}
