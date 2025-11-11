#pragma once

#include "config.h"
#include "main.h"
#include "display.h"

namespace radio {
    void on_data_recv(uint8_t* address, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
        char incoming[16];

        connection_on = true;
        snprintf(incoming, len + 1, "%s", data);
        
        if (strcmp(incoming, "4,CONN") == 0) {
            beat_back_timer = millis();
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

    void on_data_sent(uint8_t* address, uint8_t status) {
        if (status == ESP_NOW_SEND_SUCCESS && sending_laps) {
            buzzing = true;
            sending_laps = false;
            buzzer_timer = millis();
            
            digitalWrite(PIN_BUZZER, HIGH);
        }
    }

    void init_espnow() {
        WiFi.mode(WIFI_MODE_STA);
        delay(500);

        // esp_wifi_set_promiscuous(true);
        if (esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE) == ESP_OK) {
            #ifdef DEBUG
            Serial.printf("ESPNOW channel moved to %i\n", ESPNOW_CHANNEL);
            #endif
        } else {
            delay(500);
            ESP.restart();
        }
        // esp_wifi_set_promiscuous(false);

        quickEspNow.onDataSent (on_data_sent);
        quickEspNow.onDataRcvd (on_data_recv);
        quickEspNow.begin (ESPNOW_CHANNEL);

        esp_now_peer_info_t peer = {};
        peer.channel = ESPNOW_CHANNEL;
        peer.encrypt = false;
        peer.ifidx = WIFI_IF_STA;
        memcpy(peer.peer_addr, espnow_gateway, 6);

        if (esp_now_add_peer(&peer) != ESP_OK) {
            #if DEBUG
            Serial.println("Failed to add peer");
            #endif

            while (1);
        }
        
        #if DEBUG
        Serial.println("ESPNOW init OK");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.channel());
        Serial.println(WiFi.macAddress());
        // Serial.println(WiFi.softAPmacAddress());
        // Serial.printf("AP %s inicializado en %u.%u.%u.%u\n", WIFI_SSID, ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
        #endif
    }

    void send_laps() {
        // if (quickEspNow.readyToSendData()) {           
            sending_laps = true;
            const String payload = ((String)device_id + "," + (String)laps);
            quickEspNow.send (espnow_gateway, (uint8_t*)payload.c_str (), payload.length ());
            
            #if DEBUG
            Serial.printf("Sending laps: %i\n", laps);
            #endif
		// }
    }
}
