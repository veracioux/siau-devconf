
class Device
{
public:
    Device();
    virtual ~Device();
    
    // FACTORY PARAMETERS

    static const char *getVendorId() const;
    static const char *getDeviceId() const;
    static const char *getDescription() const;

    // USER PARAMETERS


protected:
};
