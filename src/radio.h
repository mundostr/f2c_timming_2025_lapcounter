#pragma once

#include "config.h"
#include "main.h"
#include "display.h"

namespace radio {
    void wf_reset() {
        WiFi.persistent(false);
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
    }

    void dataReceived (uint8_t* address, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
        char incoming[16];

        connection_on = true;
        snprintf(incoming, len + 1, "%s", data);
        
        if (strcmp(incoming, "4,CONN") == 0) {
            main::beat_back_timer = millis();
            display::show_status();
        }

        if (strcmp(incoming, "4,START") == 0) {
            laps = 0;
            
            display::big.setBigNumberCursor(0, 0);
            display::big.print(laps);
            display::lcd.setCursor(0, 1);
            display::lcd.print(laps);
        }

        #if DEBUG
        Serial.printf ("ESPNOW received: %s\n", incoming);
        #endif
    }

    void dataSent(uint8_t* address, uint8_t status) {
        if (status == ESP_NOW_SEND_SUCCESS && sending_laps) {
            buzzing = true;
            sending_laps = false;
            main::buzzer_timer = millis();
            
            digitalWrite(PIN_BUZZER, HIGH);
        }
    }

    void init_espnow() {
        wf_reset();
        WiFi.mode(WIFI_STA);

        uint8_t primary_channel = ESPNOW_CHANNEL;
        wifi_second_chan_t secondary_channel = WIFI_SECOND_CHAN_NONE;
        esp_wifi_set_promiscuous(true);
        if (esp_wifi_set_channel(primary_channel, secondary_channel) == ESP_OK) {
            #if DEBUG
            Serial.printf("Canal ESPNOW cambiado a %i\n", ESPNOW_CHANNEL);
            #endif
        } else {
            #if DEBUG
            Serial.printf("ERROR al cambiar canal\n");
            #endif
        }
        esp_wifi_set_promiscuous(false);

        quickEspNow.onDataRcvd(dataReceived);
        quickEspNow.onDataSent(dataSent);
        quickEspNow.begin(ESPNOW_CHANNEL);

        #if DEBUG
        Serial.println("ESP-NOW inicializado");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.channel());
        Serial.println(WiFi.macAddress());
        // Serial.println(WiFi.softAPmacAddress());
        // Serial.printf("AP %s inicializado en %u.%u.%u.%u\n", WIFI_SSID, ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
        #endif
    }

    void send_laps() {
        sending_laps = true;
        const String payload = ((String)device_id + "," + (String)laps);
        quickEspNow.send(espnow_gateway, (uint8_t*)payload.c_str(), 5);
    }
}
