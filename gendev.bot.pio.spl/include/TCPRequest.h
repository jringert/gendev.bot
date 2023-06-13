#ifndef TCPRequest_h
#define TCPRequest_h

#include "RequestTmpl.h"
#include <Arduino.h>
#include <WiFiClient.h>

class TCPRequest : public RequestTmpl {
protected:

    WiFiClient _wifi;

    virtual bool read(String* s);

    virtual void init() override;
    virtual void request() override;
    virtual void cleanup() override;

public:
    TCPRequest(String host, uint16_t port);
};

#endif