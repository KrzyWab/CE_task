#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

#include "oledDisplay.h"

#define MAX_FONTS  42

#define SELECTED_FONT_INDEX  0

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

void oledDisplay_test(void)
{
	cfb_framebuffer_clear(displayDev, false);
	if (cfb_print(displayDev, "Hello OLED", 0, 0))
	{
		LOG_ERR("Cannot display text that has been sent");
	}
	cfb_framebuffer_finalize(displayDev);
}
