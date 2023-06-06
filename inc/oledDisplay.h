#ifndef __OLED_DISPLAY_H
#define __OLED_DISPLAY_H

int oledDisplay_init(void);
void oledDisplay_clear(void);
void oledDisplay_update(void);
void oledDisplay_showRange(int32_t range_mm, uint32_t errorCode);
void oledDisplay_showAls(uint32_t lux, uint32_t errorCode);
void oledDisplay_test(void);

#endif /* __OLED_DISPLAY_H */
