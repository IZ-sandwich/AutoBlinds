#pragma once

#include "AutoBlinds.h"

// Must have correct current limiting, otherwise microstepping will not work!
// 1/32 is also available but likely not useful
enum StepperMode
{
    FULL,
    HALF,
    QUARTER,
    EIGHTH,
    SIXTEENTH
};
static const char *stepperModeStrings[] = {"Full", "Half", "Quarter", "Eighth", "Sixteenth"};
const char *getTextForStepperMode(int enumVal)
{
    if (enumVal >= 0 && enumVal < 5)
    {
        return stepperModeStrings[enumVal];
    }
    else
    {
        return stepperModeStrings[0];
    }
}

boolean stepHigh = false;
boolean isMoving = false;

void updateMode(StepperMode mode)
{
    switch (mode)
    {
    case FULL:
    {
        digitalWrite(stepper_mode_pin0, LOW);
        digitalWrite(stepper_mode_pin1, LOW);
        digitalWrite(stepper_mode_pin2, LOW);
        break;
    }
    case HALF:
    {
        digitalWrite(stepper_mode_pin0, HIGH);
        digitalWrite(stepper_mode_pin1, LOW);
        digitalWrite(stepper_mode_pin2, LOW);
        break;
    }
    case QUARTER:
    {
        digitalWrite(stepper_mode_pin0, LOW);
        digitalWrite(stepper_mode_pin1, HIGH);
        digitalWrite(stepper_mode_pin2, LOW);
        break;
    }
    case EIGHTH:
    {
        digitalWrite(stepper_mode_pin0, HIGH);
        digitalWrite(stepper_mode_pin1, HIGH);
        digitalWrite(stepper_mode_pin2, LOW);
        break;
    }
    case SIXTEENTH:
    {
        digitalWrite(stepper_mode_pin0, LOW);
        digitalWrite(stepper_mode_pin1, LOW);
        digitalWrite(stepper_mode_pin2, HIGH);
        break;
    }
    default: // default to FULL to be safe
    {
        digitalWrite(stepper_mode_pin0, LOW);
        digitalWrite(stepper_mode_pin1, LOW);
        digitalWrite(stepper_mode_pin2, LOW);
        break;
    }
    }
}

/**
 * This method is called when you want to set a new state.
 */
void controlMove(BlindsState newDesiredState)
{
    switch (newDesiredState)
    {
    case STOPPED:
    case FULLY_UP:
    case FULLY_DOWN:
    {
        Serial.println("Stopping motor");
        isMoving = false;
        blindsState = newDesiredState;
        break;
    }
    case GOING_UP:
    {
        if (blindsState == FULLY_UP)
        {
            Serial.println("Blinds already fully up!");
        }
        else if (blindsState == FULLY_DOWN)
        {
            Serial.println("Going up from fully down!");
            digitalWrite(dir_pin, LOW);
            isMoving = true;
        }
        else
        {
            Serial.println("Going up from middle!");
            digitalWrite(dir_pin, LOW);
            isMoving = true;
            blindsState = newDesiredState;
        }
        break;
    }
    case GOING_DOWN:
    {
        if (blindsState == FULLY_DOWN)
        {
            Serial.println("Blinds already fully down!");
        }
        else if (blindsState == FULLY_UP)
        {
            Serial.println("Going down from fully up!");
            digitalWrite(dir_pin, HIGH);
            isMoving = true;
        }
        else
        {
            Serial.println("Going down from middle!");
            digitalWrite(dir_pin, HIGH);
            isMoving = true;
            blindsState = newDesiredState;
        }
        break;
    }
    }
}

/**
 * This method is called every loop and controls the stepper steps.
 */
void updateMove()
{
    if (isMoving)
    {
        digitalWrite(disableMotor_pin, LOW);
        if (stepHigh)
        {
            stepHigh = false;
            digitalWrite(step_pin, LOW);
        }
        else
        {
            stepHigh = true;
            digitalWrite(step_pin, HIGH);
        }
    }
    else
    {
        // No need for holding torque
        digitalWrite(disableMotor_pin, HIGH);
    }
}

/**
 * Method to update motor ticker period, must be after the updateMove method.
 */
void updateTickerPeriod(int value)
{
    motor_ticker_period_ms = value;
    motorTicker.detach();
    motorTicker.attach_ms(motor_ticker_period_ms, updateMove);
}

/**
 * This method is called when the stop switch is pressed
 */
void handleStopSwitch()
{
    bool stopSwitch = digitalRead(stopSwitch_pin);
    if (stopSwitch == LOW)
    {
        if (blindsState == GOING_UP)
        {
            Serial.println("Detected as fully up!");
            controlMove(FULLY_UP);
        }
        else if (blindsState == GOING_DOWN)
        {
            Serial.println("Detected as fully down!");
            controlMove(FULLY_DOWN);
        }
        // Only set to STOPPED when outside of fully up or fully down
        else if (blindsState != FULLY_UP && blindsState != FULLY_DOWN)
        {
            Serial.println("Detected mid-move button stop!");
            controlMove(STOPPED);
        }
    }
    else
    {
        // Reset state when outside of switch
        if (blindsState == FULLY_UP)
        {
            Serial.println("Set state to going down after switch release!");
            blindsState = GOING_DOWN;
        }
        else if (blindsState == FULLY_DOWN)
        {
            Serial.println("Set state to going up after switch release!");
            blindsState = GOING_UP;
        }
    }
}
