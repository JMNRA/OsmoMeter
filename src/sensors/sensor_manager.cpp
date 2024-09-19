#include "sensor_manager.h"

SensorManager::SensorManager() : oneWire(ONE_WIRE_BUS), sensors(&oneWire) {}

void SensorManager::begin() {
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(3.3);
    gravityTds.setAdcRange(4096);
    gravityTds.begin();
    sensors.begin();
}

void SensorManager::update() {
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
    gravityTds.setTemperature(temperature);
    gravityTds.update();
    tdsValue = gravityTds.getTdsValue();
}

float SensorManager::getTemperature() {
    return temperature;
}

float SensorManager::getTdsValue() {
    return tdsValue;
}