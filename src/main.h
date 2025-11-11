#pragma once

#include "config.h"
#include "display.h"

namespace main {
    uint32_t beat_timer = 0;
    uint32_t buzzer_timer = 0;
    uint32_t beat_back_timer = 0;

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
            if (!buzzing) {
                sending_laps = false;
                const String payload = ((String)device_id + ",BEAT");
                quickEspNow.send(espnow_gateway, (uint8_t*)payload.c_str(), 6);
            }

            beat_timer = millis();
        }

        if (millis() - beat_back_timer >= BEAT_BACK_FREQ) {
            connection_on = false;
            display::show_status();
        }
    }
}
