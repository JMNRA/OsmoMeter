#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "../config.h"

class SensorManager {
public:
    SensorManager();
    void begin();
    void update();
    float getTemperature();
    float getTdsValue();

private:
    GravityTDS gravityTds;
    OneWire oneWire;
    DallasTemperature sensors;
    float temperature;
    float tdsValue;
};

#endif // SENSOR_MANAGER_H