// #ifndef AUTO_BLINDS_H
// #define AUTO_BLINDS_H
#pragma once

#include <ESP8266WebServer.h>
#include <Ticker.h>

/**
 * Header file for defining constants, webpage template, etc
 */

enum BlindsState
{
    STOPPED,
    GOING_UP,
    GOING_DOWN,
    FULLY_UP,
    FULLY_DOWN
};

static const uint8_t pin_D0 = 16;
static const uint8_t pin_D1 = 5;
static const uint8_t pin_D2 = 4;
static const uint8_t pin_D3 = 0;
static const uint8_t pin_D4 = 2;
static const uint8_t pin_D5 = 14;
static const uint8_t pin_D6 = 12;
static const uint8_t pin_D7 = 13;
static const uint8_t pin_D8 = 15;
static const uint8_t pin_D9 = 3;
static const uint8_t pin_D10 = 1;

const int led_pin = LED_BUILTIN;
const int step_pin = pin_D1;
const int dir_pin = pin_D0;
const int stopSwitch_pin = pin_D2;
const int disableMotor_pin = pin_D3;

const int stepper_mode_pin0 = pin_D7;
const int stepper_mode_pin1 = pin_D6;
const int stepper_mode_pin2 = pin_D5;

uint32_t motor_ticker_period_ms = 200;

const String postForms = "<html>\
    <head>\
        <title>ESP8266 Web Server POST handling</title>\
        <style>\
            body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
        </style>\
    </head>\
    <body>\
        <h1>POST plain text to /postplain/</h1><br>\
            <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
            <input type=\"text\" name=\'{\"hello\": \"world\", \"trash\": \"\' value=\'\"}\'><br>\
            <input type=\"submit\" value=\"Submit\">\
        </form>\
        <h1>POST form data to /postform/</h1><br>\
            <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
            <input type=\"text\" name=\"hello\" value=\"world\"><br>\
            <input type=\"submit\" value=\"Submit\">\
        </form>\
        <h1>POST form (command) data to /postformCommand/</h1><br>\
        <form method=\"post\" enctype=\"application/x-www-form-urlencode\" action=\"/postformCommand/\">\
            <label for=\"command\">Command:</label><br>\
                <select id=\"command\" name=\"command\">\
                    <option value=\"goUp\">Go up</option>\
                    <option value=\"goDown\">Go down</option>\
                    <option value=\"stop\">Stop</option>\
                    <option value=\"setMotorPeriod\">Set motor period</option>\
                    <option value=\"setMotorMode\">Set motor mode</option>\
                </select><br>\
            <label for=\"value\">Command value:</label><br>\
            <input type=\"text\" id=\"value\" name=\"value\" value=\"420\"><br><br>\
            <input type=\"submit\" value=\"Submit\">\
        </form>\
        <p>Note: Motor modes are 0:Full, 1:Half, 2:Quarter, 3:Eighth, 4:Sixteenth</p><br>\
    </body>\
</html>";

// Other useful stuff:
// <input type=\"text\" id=\"command\" name=\"command\" value=\"goUp\"><br>\


ESP8266WebServer server(80);

float motorSpeed = 1.0;

BlindsState blindsState;

Ticker motorTicker;

// #endif /* AUTO_BLINDS_H */