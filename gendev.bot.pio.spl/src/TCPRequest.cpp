#include "TCPRequest.h"
#include <Arduino.h>

// ----- CONSTRUCTORS -----------------------------------------------------------------------------

TCPRequest::TCPRequest(String host, uint16_t port) : RequestTmpl(host, port),
                                                     _wifi()
{}

// ----- PROTECTED --------------------------------------------------------------------------------

/* virtual */ bool TCPRequest::read(String* s) {
    uint8_t buffer[64];

    // wait for the answer to become available
    uint32_t t = millis();

    while(true) {
        delay(1);
        
        if(_wifi.available() || (_timeout > 0 && millis() - t > _timeout)) {
            break;
        }
    }

    // either the answer is available now or a time is over
    int avail = _wifi.available();

    if(!avail) {
        _timed_out = true;
        return false;
    }

    // answer is available
    int num_b = _wifi.read(buffer, 64);
    buffer[num_b] = '\0';

    *s = String((char*) buffer);

    return true;
}

/* virtual */ void TCPRequest::init() {
    if(_wifi.connect(_host.c_str(), _port)) {
        _init_state = true;
    }
}

/* virtual */ void TCPRequest::request() {
    if(_init_state) {
        uint32_t t = millis(); // 0
        
        _wifi.write(_payload.c_str());
        _req_dur = millis() - t;
        bool success = read(&_response);
        _res_dur = millis() - t - _req_dur;
    }
}

/* virtual */ void TCPRequest::cleanup() {
    _wifi.stop();
    _init_state = false;
}
