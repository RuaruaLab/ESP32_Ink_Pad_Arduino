#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "PINOUT.h"
#include "display.hpp"
#include "SensirionI2cSht4x.h"

class DHT40
{
private:
    SensirionI2cSht4x sensor;
    char errorMessage[64];

public:
    float aTemperature = 0.0;
    float aHumidity = 0.0;

    void update();
    void init();
};



void sensor_update_task(void *pvParameters);


#endif