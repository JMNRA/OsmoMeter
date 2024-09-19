#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define OLED_SDA 4
#define OLED_SCL 5
#define OLED_RST -1
#define TdsSensorPin 1
#define ONE_WIRE_BUS 2
#define LED_GREEN 6
#define LED_YELLOW 10
#define LED_RED 3

// EEPROM configuration
#define EEPROM_SIZE 512

// Water quality thresholds
#define GOOD_QUALITY_THRESHOLD 250
#define MEDIUM_QUALITY_THRESHOLD 350

#endif // CONFIG_H