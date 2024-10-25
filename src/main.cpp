#include <Arduino.h>
#include "PINOUT.h"
#include "display.hpp"
#include "esp_partition.h"
#include "spi_flash_mmap.h"
#include "sensor.hpp"
#include "net.h"
#include <WiFi.h>

uint8_t recive_buff[256] = {0};

char ssid[32] = {0};
char password[64] = {0};

/*系统状态*/
enum SystemStatus {
    NORMAL = 0x00,
    USB_EXTRACT = 0x01
};
uint8_t sys_status = NORMAL;

void check_flash_space();

void serial_ubpack(uint8_t *pack);

void check_serial_data();

void check_usb_status();

void flash_write_img(unsigned char *img_data);

void flash_read_img(unsigned char *img_data);

void flash_read_wifi_cfg(char *ssid, char *password);

void flash_write_wifi_cfg(char *ssid, char *password);

void check_wifi();

void setup() {
    /*GPIO初始化*/
    //LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    //USB拔出检测
    pinMode(V_3125_PIN, INPUT);
    // 初始化串口
    Serial.begin(115200);
    // sleep
    delay(1000);

    flash_read_wifi_cfg(ssid, password);

    // // wifi尝试连接
    // connect_wifi(ssid, password);

     config_tz();

    /*墨水屏初始化*/
    display.begin();
    display.clearScreen(EPaperDisplay::PIXEL_WHITE);

    //从flash中读取图片
    flash_read_img(gImage_2in13g);

    //启动状态更新线程
    xTaskCreate(sensor_update_task, "sensor update task", 2048, NULL, 1, NULL);
}

void loop() {
    /*USB拔出处理*/

    check_usb_status();
    check_serial_data();
    check_wifi();
    vTaskDelay(1);

}


/*--------FLASH操作---------*/
void flash_read_img(unsigned char *img_data) {
    // 找到标签为 "img_data" 的分区
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS,
                                                           "img_data");
    if (part == NULL) {
        Serial.println("Partition not found!");
        return;
    }

    // 从分区读取数据
    esp_err_t err = esp_partition_read(part, 0, img_data, 7750);
    if (err != ESP_OK) {
        Serial.print("Read failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    /*DEBUG*/
    // Serial.println("Read completed successfully!");
    // Serial.println("Data read from the partition:");

    // 打印数据到串口监视器，以十六进制格式显示
    // for (int i = 0; i < 7750; ++i) {
    //   if (i % 16 == 0) {
    //     Serial.println();
    //   }
    //   Serial.print(img_data[i], HEX);
    //   Serial.print(" ");
    // }
}

void flash_read_wifi_cfg(char *ssid, char *password) {
    // 找到标签为 "wifi_cfg" 的分区
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS,
                                                           "wifi_cfg");
    if (part == NULL) {
        Serial.println("Partition not found!");
        return;
    }

    // 从分区读取数据
    esp_err_t err = esp_partition_read(part, 0, ssid, 32);
    if (err != ESP_OK) {
        Serial.print("Read failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    err = esp_partition_read(part, 32, password, 64);
    if (err != ESP_OK) {
        Serial.print("Read failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    // // 模拟清空，可以加一个串口命令做这个
    // uint8_t zero_buffer[4096];
    // memset(zero_buffer, 0, sizeof(zero_buffer));
    // esp_partition_write(part, 0, zero_buffer, sizeof(zero_buffer));
}


void flash_write_img(unsigned char *img_data) {
    // 找到标签为 "img_data" 的分区
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS,
                                                           "img_data");
    if (part == NULL) {
        Serial.println("Partition not found!");
        return;
    }

    // 首先擦除分区
    esp_err_t err = esp_partition_erase_range(part, 0, (7750 + SPI_FLASH_SEC_SIZE - 1) & ~(SPI_FLASH_SEC_SIZE - 1));
    if (err != ESP_OK) {
        Serial.print("Erase failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    // 向分区写入数据
    err = esp_partition_write(part, 0, img_data, 7750);
    if (err != ESP_OK) {
        Serial.print("Write failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    Serial.println("Write completed successfully!");
}

void flash_write_wifi_cfg(char *ssid, char *password) {
    // 找到标签为 "wifi_cfg" 的分区
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS,
                                                           "wifi_cfg");
    if (part == NULL) {
        Serial.println("Partition not found!");
        return;
    }

    // 首先擦除分区
    esp_err_t err = esp_partition_erase_range(part, 0, (96 + SPI_FLASH_SEC_SIZE - 1) & ~(SPI_FLASH_SEC_SIZE - 1));
    if (err != ESP_OK) {
        Serial.print("Erase failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    // 向分区写入数据
    err = esp_partition_write(part, 0, ssid, 32);
    if (err != ESP_OK) {
        Serial.print("Write failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    err = esp_partition_write(part, 32, password, 64);
    if (err != ESP_OK) {
        Serial.print("Write failed: ");
        Serial.println(esp_err_to_name(err));
        return;
    }

    Serial.println("Write completed successfully!");
}

/*--------轮询处理USB拔出---------*/
void check_usb_status() {
    if (digitalRead(V_3125_PIN) == LOW) {
        // 断开wifi多苟一会
        WiFi.disconnect();
        display.set(gImage_2in13g);
        while (1) {
            delay(100);
            if (digitalRead(V_3125_PIN) == HIGH) break;
        }
    }
}

/*--------串口接收&解包部分---------*/
//没时间就暂时先不整理

/*
通讯协议：
第一位  帧头 0x5C
第二位  数据长度，不含帧头(256以内)
第三位  CMD_ID
<数据段>
末位    帧尾 0x7A
*/

//轮询串口数据
void check_serial_data() {
    if (Serial.available() > 0) {
        /*识别帧头*/
        uint8_t read_data;
        read_data = Serial.read();
        if (read_data == 0x5C) {
            memset(recive_buff, 0, sizeof(recive_buff));  //清零缓冲区

            uint8_t data_length = 0;
            data_length = Serial.read();
            printf("length = %d\n", data_length);
            Serial.readBytes(recive_buff, data_length);
            if (recive_buff[data_length - 1] == 0x7A) {
                serial_ubpack(recive_buff);
            }
        }
    }
}

//解析数据包
void serial_ubpack(uint8_t *pack) {
    switch (pack[0]) {
        case 0x01: {
            int frame_id = pack[1]; //当前帧id
            for (int i = 0; i < 64; i++) gImage_2in13g[frame_id * 64 + i] = pack[i + 2];
            if (frame_id >= 0x79) {
                flash_write_img(gImage_2in13g);   //将接收到的图片写入FLASH
                // display.set(gImage_2in13g);
                // 调试flag
                // while(1){
                //   printf("called refresh!\n");
                // }
            }
            break;
        }
        case 0x02: {
            int ssid_length = pack[1];
            int password_length = pack[2];

            if (ssid_length > 0 && ssid_length < sizeof(ssid)) {
                memcpy(ssid, &pack[3], ssid_length);
            }
            if (password_length > 0 && password_length < sizeof(password)) {
                memcpy(password, &pack[3 + ssid_length], password_length);
            }

            // save ssid and password
            flash_write_wifi_cfg(ssid, password);

            // Connect to WiFi using the received SSID and password
            connect_wifi(ssid, password);
            break;
        }


        default:
            break;
    }

    return;
}

// wifi连接检查，很弱智，连接阶段会阻断串口，先这样吧
unsigned long last = 0;
void check_wifi() {
    unsigned long current = millis();
    if (WiFi.status() != WL_CONNECTED && current - last >= 10000) {
        connect_wifi(ssid, password);
        last = current;
    }
}

/*--------调试工具---------*/

void check_flash_space() {
    delay(3000);
    /*-----------------查看FLASH空间-----------------*/
    // 初始化partition迭代器
    const esp_partition_t *partition;
    esp_partition_iterator_t iter = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    if (!iter) {
        Serial.println("Failed to find partitions");
        return;
    }

    // 遍历所有partition
    while (iter != NULL) {
        partition = esp_partition_get(iter);
        Serial.print("Type: ");
        Serial.print(partition->type, HEX);
        Serial.print(", Subtype: ");
        Serial.print(partition->subtype, HEX);
        Serial.print(", Address: 0x");
        Serial.print(partition->address, HEX);
        Serial.print(", Size: ");
        Serial.print(partition->size);
        Serial.print(" bytes, Label: ");
        Serial.println(partition->label);

        // 移动到下一个partition
        iter = esp_partition_next(iter);
    }

    // 释放迭代器
    esp_partition_iterator_release(iter);
}