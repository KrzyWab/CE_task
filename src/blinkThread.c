#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "blinkThread.h"

#define TEST_SLEEP_MSEC		3000

LOG_MODULE_REGISTER(blinkngLED);

/* build error on this step means there is no driver for PWM on LED1 for this board */
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

static int blinkLed_init(void);
static void blinkLed_test(void);

void blinkThread_entry(void)
{
	if(blinkLed_init())
	{
		k_sleep(K_FOREVER);
	}

	blinkLed_test();
}

static int blinkLed_init(void)
{
	if (!gpio_is_ready_dt(&led1)) {
		LOG_ERR("GPIO for LED1 is not ready");
		return -1;
	}
	if (gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE) < 0) {
		LOG_ERR("Cannot active GPIO for LED1");
		return -1;
	}
	LOG_INF("LED1 initialized successfully");

	return 0;
}

static void blinkLed_test(void)
{
	while (1) {
		if (gpio_pin_toggle_dt(&led1))
		{
			LOG_ERR("Cannot toggle GPIO state for LED1");
			k_sleep(K_FOREVER);
		}
		k_msleep(TEST_SLEEP_MSEC);
	}
}
