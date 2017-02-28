#include "WebServer.h"

ESP8266WebServer* m_server;

void handleRoot()
{
//    char temp[420];
//    snprintf(temp, 400, "<html>\
//                        <head>\
//                        <title>ESP8266 Demo</title>\
//                        <style>\
//                        body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
//                        </style>\
//                        </head>\
//                        <body>\
//                        <h1>Hello from ESP8266!</h1>\
//                        F_name: <input type='text' name='fname'><br>\
//                        <input type='submit' value='Submit'>\
//                        </body>\
//                        </html>"
//            );
//    m_server->send(200, "text/html", temp);
    m_server->send(200, "text/plain", "Hola mundo!");
}

WebServer::WebServer()
{
    m_server = new ESP8266WebServer(80);
}

WebServer::~WebServer()
{
    delete m_server;
}


void WebServer::setup()
{
    m_server->on("/", handleRoot);
}

void WebServer::start()
{
    m_server->begin();
}

void WebServer::stop()
{
    m_server->stop();
}

void WebServer::loop()
{
    m_server->handleClient();
}
