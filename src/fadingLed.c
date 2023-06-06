#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include "fadingLed.h"

#define TEST_NUM_STEPS		100U
#define TEST_SLEEP_MSEC		20U

#define RANGE_MEAS_NUM_STEPS	200		// Maximum range_mm value possible for VL6180x

#define PWM_DIRECTION_BRIGHTEN	1U
#define PWM_DIRECTION_DARKEN	0U

LOG_MODULE_REGISTER(fadingLed);

/* Build error on this step means there is no driver for PWM on LED0 for this board */
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

static uint32_t pulseWidth;
static uint32_t step;
static uint8_t direction = PWM_DIRECTION_BRIGHTEN;

int fadingLed_init()
{
	if(!device_is_ready(pwm_led0.dev))
	{
		LOG_ERR("PWM fading led is not ready");
		return -1;
	}
	return 0;
}

void fadingLed_showRange(int32_t range_mm, uint32_t errorCode)
{
	pulseWidth = 0;
	if(errorCode)
	{
		if (pwm_set_pulse_dt(&pwm_led0, pulseWidth))
		{
			LOG_ERR("Cannot set PWM width value to fading LED");
		}
		return;
	}

	step = pwm_led0.period / RANGE_MEAS_NUM_STEPS;
	pulseWidth = (RANGE_MEAS_NUM_STEPS - range_mm) * step;

	if (pwm_set_pulse_dt(&pwm_led0, pulseWidth))
	{
		LOG_ERR("Cannot set PWM width value to fading LED");
	}
}

void fadingLed_test()
{
	pulseWidth = 0;
	step = pwm_led0.period / TEST_NUM_STEPS;

	while(1)
	{
		if (pwm_set_pulse_dt(&pwm_led0, pulseWidth))
		{
			LOG_ERR("Cannot set PWM width value to fading LED");
			k_sleep(K_FOREVER);
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
		k_msleep(TEST_SLEEP_MSEC);
	}
}
