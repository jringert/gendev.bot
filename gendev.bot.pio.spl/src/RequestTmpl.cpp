#include "RequestTmpl.h"
#include <Arduino.h>

// ----- CONSTRUCTORS -----------------------------------------------------------------------------

RequestTmpl::RequestTmpl(String host, uint16_t port) : _host(host),
                                                       _port(port),
                                                       _reuse_connection(false),
                                                       _init_state(false),
                                                       _timeout(0),
                                                       _payload(),
                                                       _response(),
                                                       _full_req_dur(-1),
                                                       _req_dur(-1),
                                                       _res_dur(-1),
                                                       _timed_out(false)
{
}

// ----- PROTECTED --------------------------------------------------------------------------------

/* virtual */ void RequestTmpl::init() {}

/* virtual */ void RequestTmpl::request() {}

/* virtual */ void RequestTmpl::cleanup() {}

// ----- PUBLIC -----------------------------------------------------------------------------------

void RequestTmpl::run()
{
    uint32_t t = millis();

    if (!_reuse_connection || !_init_state)
    {
        init();
    }

    request();

    if (!_reuse_connection)
    {
        cleanup();
    }

    _full_req_dur = millis() - t;
}

void RequestTmpl::setReuseConnection(bool r)
{
    _reuse_connection = r;
}

void RequestTmpl::setTimeout(uint32_t t)
{
    _timeout = t;
}

void RequestTmpl::setPayload(String p)
{
    _payload = p;
}

uint32_t RequestTmpl::getTimeout() const
{
    return _timeout;
}

const String &RequestTmpl::getResponse() const
{
    return _response;
}

int RequestTmpl::getDuration() const
{
    return _full_req_dur;
}

int RequestTmpl::getRequestDuration() const {
    return _req_dur;
}

int RequestTmpl::getResponseDuration() const {
    return _res_dur;
}

bool RequestTmpl::isTimedOut() const
{
    return _timed_out;
}
