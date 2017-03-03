#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <vector>
#include <map>

#include <ESP8266WebServer.h>
#include <FS.h>
#include "ArduinoJson.h"


class WebServer
{
private:
    WebServer();              // Singleton
    WebServer(WebServer const&);              // Singleton: Don't Implement
    void operator=(WebServer const&); // Singleton: Don't implement

    std::map<std::string, std::string> m_inputFieldsContent;
    void (*m_submitCallback)(std::map<std::string, std::string>);
    std::string m_htmlPagePath;
    File m_htmlPage;

    void setSubmitCallback(void (*submitCallback)(std::map<std::string, std::string>));
    void setHtmlPagePath(std::string htmlPagePath);

    String m_jsonDataString;

public:
    static WebServer& getInstance();
    ~WebServer();

    void webServerHandleRoot();
    void webServerHandleData();
    void webServerHandleSubmit();

    std::map<std::string, std::string> getInputFieldsContent();
    void setup(std::string htmlPagePath, void (*submitCallback)(std::map<std::string, std::string>));
    void setData(std::vector<std::pair<std::string, std::string>> data);
    void start();
    void stop();
    void loop();
};

#endif
