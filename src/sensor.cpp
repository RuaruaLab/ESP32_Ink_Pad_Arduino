#include "sensor.hpp"
#include "SensirionI2cSht4x.h"


DHT40 dht40;


void sensor_update_task(void *pvParameters)
{
    dht40.init();
    display.drawString(10, 50, "0123456789%\t", display.PIXEL_YELLOW);
    display.drawString(10, 30, "RuaRuaLab", display.PIXEL_RED);
    display.drawString(10, 10, "Fish - SK - Mind", display.PIXEL_BLACK);
    display.refreshScreen();
    while(1)
    {
        dht40.update();
        printf("tempreature:%f,\thumidity:%f\n", dht40.aTemperature, dht40.aHumidity);
        vTaskDelay(100);
    }
}

void DHT40::update()
{
    sensor.measureLowestPrecision(aTemperature, aHumidity);
}

void DHT40::init()
{
    int16_t error;
    uint32_t serialNumber = 0;

    Wire.begin(IIC_SDA, IIC_SCL);
    sensor.begin(Wire, SHT40_I2C_ADDR_44);
    sensor.softReset();
    vTaskDelay(10);
    error = sensor.serialNumber(serialNumber);

    /*debug*/
    if (error != 0) {
        printf("init error\n");
    }
    Serial.print("serialNumber: ");
    Serial.print(serialNumber);
    Serial.println();
}