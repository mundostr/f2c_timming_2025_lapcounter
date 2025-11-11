#pragma once

#include "config.h"

namespace display {
    char buffer[4];

    LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
    LCDBigNumbers big(&lcd, BIG_NUMBERS_FONT_3_COLUMN_2_ROWS_VARIANT_2);

    void init() {
        lcd.begin(LCD_COLS, LCD_ROWS);
        big.begin();
    }

    void show_laps(bool turnOff = false) {
        if (turnOff) lcd.noDisplay();
        snprintf(buffer, sizeof(buffer), "%03d", laps);
        big.setBigNumberCursor(0, 0);
        big.print(buffer);
        if (turnOff) lcd.display();

        #if DEBUG
        Serial.printf("vueltas: %i\n", laps);
        #endif
    }

    void show_status() {
        lcd.setCursor(10, 1);
        lcd.print(connection_on ? "EN RED": "  SOLO");

        #if DEBUG
        Serial.printf("conexion on: %i\n", connection_on);
        #endif
    }
}
