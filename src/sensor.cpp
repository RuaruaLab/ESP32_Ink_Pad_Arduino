#include "sensor.hpp"
#include "SensirionI2cSht4x.h"
#include "imagedata.h"

DHT40 dht40;


void sensor_update_task(void *pvParameters)
{
    dht40.init();                               //初始化温湿度计
    memcpy(IMG_buffer, back_ground, 7750);      //设置背景
    // display.drawString(40, 0, "16:18  25\t  80%", display.PIXEL_BLACK);
    // display.refreshScreen();
    char info_text[60] = {0x00};

    while(1)
    {
        memcpy(IMG_buffer, back_ground, 7750);  //清空上一次的显示
        dht40.update();

        char timeStr[16];
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time");
            strcpy(timeStr, "--:--");
        } else {
            strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
        }


        sprintf(info_text, "%s  %d\t  %d%%", timeStr, (int)dht40.aTemperature, (int)dht40.aHumidity);
        info_text[14] = '%';
        display.drawString(40, 0, info_text, display.PIXEL_BLACK);
        display.refreshScreen();
        vTaskDelay(1000*60);        //一分钟一次
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