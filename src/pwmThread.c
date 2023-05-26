#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>

#include "pwmThread.h"
#include "oledDisplay.h"

#define NUM_STEPS	50U
#define SLEEP_MSEC	30U

#define PWM_DIRECTION_BRIGHTEN	1U
#define PWM_DIRECTION_DARKEN	0U

/* Build error on this step means there is no driver for PWM on LED0 for this board */
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

/* This thread is controlling LED0 and make if fading by PWM */
void pwmThread_entry(void)
{
	uint32_t pulseWidth = 0U;
	uint32_t step = pwm_led0.period / NUM_STEPS;
	uint8_t direction = PWM_DIRECTION_BRIGHTEN;
	int ret;

	if(!device_is_ready(pwm_led0.dev))
	{
		return;
	}

	oledDisplay_init();
	oledDisplay_test();

	printk("PWM thread entry\n");

	while(1)
	{
		ret = pwm_set_pulse_dt(&pwm_led0, pulseWidth);
		if (ret)
		{
			return;
		}

		/* increase pulse width */
		if(direction == PWM_DIRECTION_BRIGHTEN)
		{
			pulseWidth += step;
			if(pulseWidth >= pwm_led0.period)
			{
				pulseWidth = pwm_led0.period;
				direction = PWM_DIRECTION_DARKEN;
			}
		}
		/* decrease pulse width */
		else
		{
			if(pulseWidth >= step)
			{
				pulseWidth -= step;
			}
			else
			{
				pulseWidth = step;
				direction = PWM_DIRECTION_BRIGHTEN;
			}
		}
		k_msleep(SLEEP_MSEC);
	}

	return;
}
