#ifndef IOT_DEVICE_H
#define IOT_DEVICE_H

// Enums
typedef enum {
    On, Off
} StateOnOff;

namespace Device
{

    // Basic device information
    inline const char *getName() { return "SmartLight"; }
    inline const char *getVendorId() { return "ETF"; }
    inline const char *getModel() { return "LX34-U6M"; }

    // Device functions
    void turnOn();
    void turnOff();
    void setBrightness(float value);
    
    // Device data
    float getBrightness();
    StateOnOff getState();
    
    void initialize();
};

#endif // IOT_DEVICE_H
