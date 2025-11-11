#pragma once

#include "config.h"
#include "display.h"

namespace main {
    void init() {
        #if DEBUG
        Serial.begin(115200);
        #endif

        pinMode(PIN_LED, OUTPUT);
        digitalWrite(PIN_LED, LOW);
    }

    void buzz() {
        if (buzzing && millis() - buzzer_timer >= BUZZER_NOTIF_FREQ) {
            buzzing = false;
            buzzer_timer = millis();
            
            digitalWrite(PIN_BUZZER, LOW);
        }
    }

    void beat() {
        if (millis() - beat_timer >= BEAT_FREQ) {
            beat_timer = millis();
            
            // if (!buzzing && quickEspNow.readyToSendData()) {
                sending_laps = false;
                const String payload = ((String)device_id + ",BEAT");
                quickEspNow.send (espnow_gateway, (uint8_t*)payload.c_str (), payload.length ());
            // }
        }

        if (millis() - beat_back_timer >= BEAT_BACK_FREQ) {
            beat_back_timer = millis();

            connection_on = false;
            display::show_status();
        }
    }
}
