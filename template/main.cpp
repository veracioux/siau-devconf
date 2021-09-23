#include <mbed.h>

#include "iot_device.h"
#include "autogen/user_device.h"
#include "autogen/mqtt_wrapper.h"

int main(void)
{
    ethernetConnect(IP_ADDR, SUBNET_MASK, DEFAULT_GW);
    mqttConnect(BROKER_HOST);
    mqttAutoSubscribe();
    Device::initialize();
    while (true) mqttYield(100);
    return 0;
}
