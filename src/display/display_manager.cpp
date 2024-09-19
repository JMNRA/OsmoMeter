#include "display_manager.h"
#include "../images/images.h"
#include "../config.h"

DisplayManager::DisplayManager() : u8g2(U8G2_R0, OLED_RST, OLED_SCL, OLED_SDA) {}

void DisplayManager::begin() {
    u8g2.begin();
}

void DisplayManager::updateDisplay(float temperature, float tdsValue) {
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    
    drawBaseLayout();
    
    // Draw temperature
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(90, 30, String(temperature, 1).c_str());
    
    // Draw TDS value
    u8g2.drawStr(90, 51, String(tdsValue, 0).c_str());
    
    // Determine which state to draw based on TDS value
    if (tdsValue < GOOD_QUALITY_THRESHOLD) {
        drawGoodState();
    } else if (tdsValue < MEDIUM_QUALITY_THRESHOLD) {
        drawMediumState();
    } else {
        drawBadState();
    }
    
    u8g2.sendBuffer();
}

void DisplayManager::drawBaseLayout() {
    // Draw frame
    u8g2.drawFrame(1, 1, 126, 62);
    
    u8g2.drawXBMP(69, 5, 54, 55, image_cuadro_1_bits);
    u8g2.drawXBMP(74, 17, 16, 16, image_temp_icon_bits);
    u8g2.drawXBMP(78, 44, 7, 7, image_tds_icon_bits);
    
    // Draw labels
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(74, 15, "Temp:");
    u8g2.drawStr(74, 39, "TDS:");
}

void DisplayManager::drawGoodState() {
    u8g2.drawXBMP(3, 29, 62, 31, image_pingu_bien_bits);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(20, 22, "Bien!");
}

void DisplayManager::drawMediumState() {
    u8g2.drawXBMP(5, 18, 45, 42, image_pingu_medio_bits);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(17, 14, "Cuidado!");
}

void DisplayManager::drawBadState() {
    u8g2.drawXBMP(3, 29, 62, 31, image_pingu_mal_bits);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(5, 22, "Peligro!!!");
}