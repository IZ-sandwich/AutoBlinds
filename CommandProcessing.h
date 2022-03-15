#pragma once

#include "AutoBlinds.h"
#include "StepperHandling.h"

void processCommand(String input, int value)
{
    if (input == "goUp")
    {
        Serial.println("Recieved go up command");
        controlMove(GOING_UP);
    }
    else if (input == "goDown")
    {
        Serial.println("Recieved go down command");
        controlMove(GOING_DOWN);
    }
    else if (input == "stop")
    {
        Serial.println("Recieved stop command");
        controlMove(STOPPED);
    }
    else if (input == "setMotorPeriod")
    {
        Serial.print("Recieved command to set motor period to: ");
        Serial.println(value);
        updateTickerPeriod(value);
    }
    else if (input == "setMotorMode")
    {
        Serial.print("Recieved command to set motor mode to: ");
        Serial.println(getTextForStepperMode(value));
        updateMode((StepperMode)value);
    }
    else
    {
        Serial.print("Recieved unrecognized command: ");
        Serial.println(input);
    }
}
