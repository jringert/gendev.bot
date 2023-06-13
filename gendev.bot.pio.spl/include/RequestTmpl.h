#ifndef RequestTmpl_h
#define RequestTmpl_h

#include <Arduino.h>

class RequestTmpl
{
protected:
    String _host;
    uint16_t _port;
    bool _reuse_connection;
    bool _init_state;
    uint32_t _timeout;
    String _payload;
    String _response;
    int _full_req_dur;
    int _req_dur;
    int _res_dur;
    bool _timed_out;

    virtual void init();
    virtual void request();
    virtual void cleanup();

public:
    RequestTmpl(String host, uint16_t port);

    virtual void run();

    void setReuseConnection(bool r);
    void setTimeout(uint32_t t);
    void setPayload(String p);

    uint32_t getTimeout() const;
    const String& getResponse() const;
    int getDuration() const;
    int getRequestDuration() const;
    int getResponseDuration() const;
    bool isTimedOut() const;
};

#endif