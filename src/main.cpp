#include "main.h"
#include "radio.h"
#include "display.h"
#include "buttons.h"

void setup() {
    main::init();
    display::init();
    buttons::init();
    radio::init_espnow();

    display::show_laps();
    display::show_status();

    #if DEBUG
    Serial.println("SYSTEM STARTED");
    #endif
}

void loop() {
    main::beat();
    main::buzz();
    
    if (update_laps) {
        update_laps = false;
        radio::send_laps();
        display::show_laps();
    }
}
