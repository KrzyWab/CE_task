#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include "blinkThread.h"

#define SLEEP_MSEC	3000

/* build error on this step means there is no driver for PWM on LED0 for this board */
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

void blinkThread_entry(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led1)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	printk("Blinking LED thread ready\n");

	while (1) {
		ret = gpio_pin_toggle_dt(&led1);
		if (ret < 0) {
			return;
		}
		k_msleep(SLEEP_MSEC);
	}

	return;
}

