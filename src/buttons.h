#pragma once

#include "config.h"
#include "main.h"

namespace buttons {
    Button2 btn;
    /* hw_timer_t *timer = NULL;

    void IRAM_ATTR onTimer() {
        btn.loop();
    } */

    void activate_buzz() {
        buzzing = true;
        buzzer_timer = millis();
        digitalWrite(PIN_BUZZER, HIGH);
    }

    void count_lap(Button2 &b) {
        if (!buzzing) {
            laps++;
            beat_back_timer = millis();
            // if (connection_on) activate_buzz();
        }

        update_laps = true;
    }

    void reset_laps(Button2 &b) {
        laps = 0;
        update_laps = true;
    }

    void init() {
        btn.begin(PIN_BUTTON);
        btn.setClickHandler(count_lap);
        btn.setLongClickDetectedHandler(reset_laps);
        btn.setDebounceTime(BTN_DEBOUNCE);
        btn.setLongClickTime(BTN_LONGPRESS_TIME);

        /* timer = timerBegin(0, 80, true);
        timerAttachInterrupt(timer, &onTimer, true);
        timerAlarmWrite(timer, BTN_TIMER_FREQ, true);
        timerAlarmEnable(timer); */
    }
}
