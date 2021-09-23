#include "iot_device.h"
#include <mbed.h>

PwmOut led(D6);
float brightness = 0.5f;
StateOnOff state = Off;

// Device functions

void Device::turnOn()
{
    state = On;
    led.write(brightness);
}

void Device::turnOff()
{
    state = Off;
    led.write(0);
}

void Device::setBrightness(float value)
{
    brightness = value;
    led.write(value);
}

// Device data

float Device::getBrightness()
{
    return brightness;
}

StateOnOff Device::getState()
{
    return state;
}

void Device::initialize()
{
    led.period(0.01f);
    turnOn();
}
