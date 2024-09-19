#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include "../config.h"

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void updateDisplay(float temperature, float tdsValue);

private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
    void drawBaseLayout();
    void drawGoodState();
    void drawMediumState();
    void drawBadState();
};

#endif // DISPLAY_MANAGER_H