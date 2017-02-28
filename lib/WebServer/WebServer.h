#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>

#include <ESP8266WebServer.h>


class WebServer
{
public:
    WebServer();
    ~WebServer();
    void setup();
    void start();
    void stop();
    void loop();
};

#endif
