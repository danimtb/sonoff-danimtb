#include "WebServer.h"

ESP8266WebServer* m_server;

void handleRoot()
{
    WebServer::getInstance().webServerHandleRoot();
}

void handleSubmit()
{
    WebServer::getInstance().webServerHandleSubmit();
}

void handleData()
{
    WebServer::getInstance().webServerHandleData();
}

void WebServer::webServerHandleRoot()
{
    m_server->streamFile(m_htmlPage, "text/html");
}

void WebServer::webServerHandleSubmit()
{
    if (m_server->args() > 0 )
    {
        for (uint8_t i = 0; i < m_server->args(); i++)
        {
            std::string inputField(m_server->argName(i).c_str());
            std::string inputContent(m_server->arg(i).c_str());
            m_inputFieldsContent[inputField] = inputContent;
        }

        m_server->send(200, "text/plain", "Changes saved!");
        this->m_submitCallback(m_inputFieldsContent);
    }
}

void WebServer::webServerHandleData()
{
    m_server->send(200, "application/json", m_jsonDataString);
}

WebServer& WebServer::getInstance()
{
    static WebServer webServer;
    return webServer;
}

WebServer::WebServer()
{
    m_server = new ESP8266WebServer(80);
    SPIFFS.begin();
}

WebServer::~WebServer()
{
    delete m_server;
}


void WebServer::setup(std::string htmlPagePath, void (*submitCallback)(std::map<std::string, std::string>))
{
    m_server->on("/", handleRoot);
    m_server->on("/data", handleData);
    m_server->on("/submit", handleSubmit);

    this->setHtmlPagePath(htmlPagePath);
    this->setSubmitCallback(submitCallback);
}

void WebServer::setData(std::vector<std::pair<std::string, std::string>> data)
{
    StaticJsonBuffer<500> jsonDataBuffer; // TODO: Check buffer size
    JsonObject& jsonDataObject = jsonDataBuffer.createObject();

    for(uint16_t i = 0; i < data.size(); i++)
    {
        jsonDataObject[data[i].first.c_str()] = data[i].second.c_str();
    }

    jsonDataObject.printTo(m_jsonDataString);
}

void WebServer::start()
{
    m_htmlPage = SPIFFS.open(m_htmlPagePath.c_str(), "r");
    m_server->begin();
}

void WebServer::stop()
{
    m_server->stop();
    m_htmlPage.close();
}

void WebServer::loop()
{
    m_server->handleClient();
}

void WebServer::setHtmlPagePath(std::string htmlPagePath)
{
    m_htmlPagePath = htmlPagePath;
}

void WebServer::setSubmitCallback(void (*submitCallback)(std::map<std::string, std::string>))
{
    m_submitCallback = submitCallback;
}
