//
// Created by yueluo on 2024/10/23.
//

#include "net.h"

#include <Arduino.h>
#include <WiFi.h>

const char *TZ = "CST-8";
const char *ntpServer1 = "cn.ntp.org.cn";
const char *ntpServer2 = "ntp1.aliyun.com";
const char *ntpServer3 = "pool.ntp.org";

void print_time() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
}

void config_tz() {
    setenv("TZ", tz, 1);
}

void connect_wifi(char *ssid, char *password) {
    // print ssid
    Serial.print("SSID: ");
    Serial.println(ssid);

    if (strlen(ssid) == 0)
    {
        Serial.println("No saved wifi found!");
        return;
    }
    
    WiFi.begin(ssid, password);

    // wait until timeout
    unsigned long startAttemptTime = millis();
    const unsigned long connectionTimeout = 5000; // 5s

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < connectionTimeout) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected.");
        // config time
        configTzTime(TZ, ntpServer1, ntpServer2, ntpServer3);
        print_time();
    } else {
        Serial.println("Failed to connect to WiFi!");
    }
}