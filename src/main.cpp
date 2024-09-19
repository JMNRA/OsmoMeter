#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "sensors/sensor_manager.h"
#include "display/display_manager.h"

SensorManager sensorManager;
DisplayManager displayManager;

void updateLEDs(float tds) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);

    if (tds < GOOD_QUALITY_THRESHOLD) {
        digitalWrite(LED_GREEN, HIGH);
    } else if (tds < MEDIUM_QUALITY_THRESHOLD) {
        digitalWrite(LED_YELLOW, HIGH);
    } else {
        digitalWrite(LED_RED, HIGH);
    }
}

void setup() {
    Serial.begin(115200);
    
    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("Fallo al inicializar EEPROM");
        Serial.println("Reiniciando...");
        delay(1000);
        ESP.restart();
    }

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    sensorManager.begin();
    displayManager.begin();

    Serial.println("Configuración completa");
}

void loop() {
    sensorManager.update();
    float temperature = sensorManager.getTemperature();
    float tdsValue = sensorManager.getTdsValue();

    updateLEDs(tdsValue);
    displayManager.updateDisplay(temperature, tdsValue);

    // Imprimimos los datos en el monitor serial
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print(" C, Valor TDS: ");
    Serial.print(tdsValue, 1);
    Serial.print(" ppm, LED: ");
    if (tdsValue < GOOD_QUALITY_THRESHOLD) {
        Serial.println("Verde");
    } else if (tdsValue < MEDIUM_QUALITY_THRESHOLD) {
        Serial.println("Amarillo");
    } else {
        Serial.println("Rojo");
    }

    delay(1000);
}