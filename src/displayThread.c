#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include "displayThread.h"
#include "fadingLed.h"
#include "oledDisplay.h"

LOG_MODULE_REGISTER(displayThread);

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

	oledDisplay_test();
	fadingLed_test();
}
