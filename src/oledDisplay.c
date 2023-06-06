#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

#include "oledDisplay.h"

#define MAX_FONTS  5

#define SELECTED_FONT_INDEX  0

#define RANGE_TEXT_SIZE		15
#define RANGE_TEXT_POS_X	0
#define RANGE_TEXT_POS_Y	0

#define ALS_TEXT_SIZE		15
#define ALS_TEXT_POS_X		0
#define ALS_TEXT_POS_Y		20

LOG_MODULE_REGISTER(display, LOG_LEVEL_DBG);

static const struct device *displayDev = DEVICE_DT_GET(DT_NODELABEL(ssd1306));
static uint8_t font_width;
static uint8_t font_height;

int oledDisplay_init(void)
{
	if(NULL == displayDev)
	{
		LOG_ERR("Display device pointer is NULL");
		return -1;
	}
	if(!device_is_ready(displayDev))
	{
		LOG_ERR("Display device is not ready");
		return -1;
	}
	if (display_set_pixel_format(displayDev, PIXEL_FORMAT_MONO10) != 0)
	{
		LOG_ERR("Failed to set required pixel format to the display device");
		return -1;
    }

	LOG_INF("Display initialized successfully");

	struct display_capabilities capabilities;
	display_get_capabilities(displayDev, &capabilities);

	LOG_DBG("x_resolution: %d", capabilities.x_resolution);
	LOG_DBG("y_resolution: %d", capabilities.y_resolution);
	LOG_DBG("supported pixel formats: %d", capabilities.supported_pixel_formats);
	LOG_DBG("screen_info: %d", capabilities.screen_info);
	LOG_DBG("current_pixel_format: %d", capabilities.current_pixel_format);
	LOG_DBG("current_orientation: %d", capabilities.current_orientation);

	if (cfb_framebuffer_init(displayDev))
	{
		LOG_ERR("Framebuffer initialization failed");
		return -1;
	}

	LOG_INF("Framebuffer initialized successfully");

	cfb_framebuffer_invert(displayDev);
	cfb_framebuffer_clear(displayDev, true);
	display_blanking_off(displayDev);

	uint8_t maxFonts = cfb_get_numof_fonts(displayDev);
	for (int idx = 0; idx < maxFonts; idx++)
	{
		cfb_get_font_size(displayDev, idx, &font_width, &font_height);
		LOG_DBG("index[%d] font width %d, font height %d",
				idx, font_width, font_height);
	}

	cfb_framebuffer_set_font(displayDev, SELECTED_FONT_INDEX);
	LOG_DBG("selected font: index[%d]", SELECTED_FONT_INDEX);

	LOG_DBG("Framebuffer params: x_res %d, y_res %d, ppt %d, rows %d, cols %d",
			cfb_get_display_parameter(displayDev, CFB_DISPLAY_WIDTH),
			cfb_get_display_parameter(displayDev, CFB_DISPLAY_HEIGH),
			cfb_get_display_parameter(displayDev, CFB_DISPLAY_PPT),
			cfb_get_display_parameter(displayDev, CFB_DISPLAY_ROWS),
			cfb_get_display_parameter(displayDev, CFB_DISPLAY_COLS));

	return 0;
}

void oledDisplay_clear(void)
{
	cfb_framebuffer_clear(displayDev, false);
}

void oledDisplay_update(void)
{
	cfb_framebuffer_finalize(displayDev);
}

void oledDisplay_showRange(int32_t range_mm, uint32_t errorCode)
{
	char text[RANGE_TEXT_SIZE]={0};
	if(errorCode)
		snprintfcb(text, RANGE_TEXT_SIZE, "Range: E%u", errorCode);
	else
		snprintfcb(text, RANGE_TEXT_SIZE, "Range: %d", range_mm);

	if(cfb_print(displayDev, text, RANGE_TEXT_POS_X, RANGE_TEXT_POS_Y))
	{
		LOG_ERR("Cannot display text that has been sent");
	}
}

void oledDisplay_showAls(uint32_t lux, uint32_t errorCode)
{
	char text[ALS_TEXT_SIZE]={0};
	if(errorCode)
		snprintfcb(text, ALS_TEXT_SIZE, "ALS: E%u", errorCode);
	else
		snprintfcb(text, ALS_TEXT_SIZE, "ALS: %d", lux);

	if(cfb_print(displayDev, text, ALS_TEXT_POS_X, ALS_TEXT_POS_Y))
	{
		LOG_ERR("Cannot display text that has been sent");
	}
}

void oledDisplay_test(void)
{
	cfb_framebuffer_clear(displayDev, false);
	if (cfb_print(displayDev, "Hello OLED", 0, 0))
	{
		LOG_ERR("Cannot display text that has been sent");
	}
	cfb_framebuffer_finalize(displayDev);
}
