/*
F2C CONTROL LINE RACING TIMMING CONTROL
ESP32 client to send ESPNOW messages to ESP32 AP. Laps counter with local display.

https://github.com/m1cr0lab-esp32/esp-now-network-and-wifi-gateway
https://www.survivingwithandroid.com/esp-now-esp32-esp8266/
https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino
https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
https://github.com/gmag11/QuickEspNow

- esptool --port PORT erase_flash in case of wrong operation
- voltage divider for battery control: R1 (+) 27k, R2 (-) 100k
*/

#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <Button2.h>
#include <QuickEspNow.h>
#include <LiquidCrystal.h>
#include "LCDBigNumbers.hpp"

#define DEBUG true

#define PIN_LED GPIO_NUM_2
#define PIN_BUTTON GPIO_NUM_12
#define PIN_LCD_RS GPIO_NUM_22
#define PIN_LCD_EN GPIO_NUM_21
#define PIN_LCD_D4 GPIO_NUM_5
#define PIN_LCD_D5 GPIO_NUM_3
#define PIN_LCD_D6 GPIO_NUM_23
#define PIN_LCD_D7 GPIO_NUM_19
#define PIN_BUZZER GPIO_NUM_18

#define LCD_COLS 16
#define LCD_ROWS 2
#define BTN_DEBOUNCE 50 // ms
#define BTN_TIMER_FREQ 1000 // us
#define BTN_LONGPRESS_TIME 2000 // ms
#define ESPNOW_CHANNEL 1
#define BUZZER_NOTIF_FREQ 100
#define BEAT_FREQ 5000
#define BEAT_BACK_FREQ 5100

int laps = 0;
int device_id = 1;
bool buzzing = false;
bool update_laps = false;
bool connection_on = false;
bool sending_laps = false;
uint8_t espnow_gateway[] = { 0xB4, 0xE6, 0x2D, 0xE9, 0xFE, 0x6E }; // we are using custom mac
