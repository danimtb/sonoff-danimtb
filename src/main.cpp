#include <map>

#include <ArduinoOTA.h>

#include "../lib/Relay/Relay.h"
#include "../lib/LED/LED.h"
#include "../lib/Button/Button.h"
#include "../lib/DataManager/DataManager.h"
#include "../lib/MqttManager/MqttManager.h"
#include "../lib/MqttManager/MqttDiscoveryComponent.h"
#include "../lib/WifiManager/WifiManager.h"
#include "../lib/WebServer/WebServer.h"
#include "../lib/UpdateManager/UpdateManager.h"
#include "../lib/TimeWatchdog/TimeWatchdog.h"

#ifdef ENABLE_SONOFF_POW
    #include "ArduinoJson.h"

    #include "../lib/PowManager/PowManager.h"
#endif

#ifdef ENABLE_SONOFF_SWITCH
    #include "../lib/ToggleSwitch/ToggleSwitch.h"
#endif


//################## FIRMWARE ##################

#define FIRMWARE "sonoff-danimtb"
#define FIRMWARE_VERSION "1.2.0"

//################## HARDWARE ##################

#ifdef ENABLE_SONOFF
    #define HARDWARE "sonoff"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
#endif

#ifdef ENABLE_SONOFF_BUTTON
    #define HARDWARE "sonoff-button"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
    #define EXTERNAL_BUTTON_PIN 14
#endif

#ifdef ENABLE_SONOFF_SWITCH
    #define HARDWARE "sonoff-switch"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
    #define SWITCH_PIN 14
#endif

#ifdef ENABLE_SONOFF_TOUCH
    #define HARDWARE "sonoff-touch"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
#endif

#ifdef ENABLE_SONOFF_POW
    #define HARDWARE "sonoff-pow"
    #define BUTTON_PIN 0
    #define LED_PIN 15
    #define LED_MODE LED_HIGH_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
#endif

#ifdef ENABLE_SONOFF_S20
    #define HARDWARE "sonoff-s20"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
#endif

#ifdef ENABLE_SONOFF_TOUCH_ESP01
    #define HARDWARE "sonoff-touch-esp01"
    #define BUTTON_PIN 0
    #define RELAY_PIN 2
    #define RELAY_MODE RELAY_HIGH_LVL
#endif

#ifdef ENABLE_EWELINK_TOUCH_SINGLE
    #define HARDWARE "ewelink-touch-single"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
#endif

#ifdef ENABLE_EWELINK_TOUCH_DOUBLE
    #define HARDWARE "ewelink-touch-double"
    #define BUTTON_PIN 0
    #define LED_PIN 13
    #define LED_MODE LED_LOW_LVL
    #define RELAY_PIN 12
    #define RELAY_MODE RELAY_HIGH_LVL
    #define BUTTON2_PIN 9
    #define RELAY2_PIN 5
#endif



#ifdef ENABLE_SONOFF_POW
    TimeWatchdog powWatchdog;
    PowManager powManager;
#endif

#ifdef ENABLE_SONOFF_SWITCH
    ToggleSwitch toggleSwitch;
#endif

#ifdef ENABLE_SONOFF_BUTTON
    Button externalButton;
#endif

UpdateManager updateManager;
DataManager dataManager;
WifiManager wifiManager;
MqttManager mqttManager;
Relay relay;

#ifdef RELAY2_PIN
   Relay relay2;
   Button button2;
#endif

Button button;
LED led;
TimeWatchdog connectionWatchdog;

String wifi_ssid = dataManager.get("wifi_ssid");
String wifi_password = dataManager.get("wifi_password");
String ip = dataManager.get("ip");
String mask = dataManager.get("mask");
String gateway = dataManager.get("gateway");
String ota_server = dataManager.get("ota_server");
String mqtt_server = dataManager.get("mqtt_server");
String mqtt_port = dataManager.get("mqtt_port");
String mqtt_username = dataManager.get("mqtt_username");
String mqtt_password = dataManager.get("mqtt_password");
String device_name = dataManager.get("device_name");
String discovery_prefix = dataManager.get("discovery_prefix");
String component_name1 = dataManager.get("component_name1");
String component_type1 = dataManager.get("component_type1");
String discovery_prefix1 = dataManager.get("discovery_prefix1");
String mqtt_status1 = dataManager.get("mqtt_status1");
String mqtt_command1 = dataManager.get("mqtt_command1");
String mqtt_secondary1 = dataManager.get("mqtt_secondary1");

#ifdef RELAY2_PIN
    String component_name2 = dataManager.get("component_name2");
    String component_type2 = dataManager.get("component_type2");
    String discovery_prefix2 = dataManager.get("discovery_prefix2");
    String mqtt_status2 = dataManager.get("mqtt_status2");
    String mqtt_command2 = dataManager.get("mqtt_command2");
    String mqtt_secondary2 = dataManager.get("mqtt_secondary2");
#endif

#ifdef ENABLE_SONOFF_POW
    MqttDiscoveryComponent* currentSensor;
    MqttDiscoveryComponent* voltageSensor;
    MqttDiscoveryComponent* activePowerSensor;
    MqttDiscoveryComponent* apparentPowerSensor;
    MqttDiscoveryComponent* reactivePowerSensor;
    MqttDiscoveryComponent* powerFactorSensor;

    void powWatchdogCallback()
    {
        mqttManager.publishMQTT(currentSensor->getStateTopic(), powManager.getCurrent());
        mqttManager.publishMQTT(voltageSensor->getStateTopic(), powManager.getVoltage());
        mqttManager.publishMQTT(activePowerSensor->getStateTopic(), powManager.getActivePower());
        mqttManager.publishMQTT(apparentPowerSensor->getStateTopic(), powManager.getApparentPower());
        mqttManager.publishMQTT(reactivePowerSensor->getStateTopic(), powManager.getReactivePower());
        mqttManager.publishMQTT(powerFactorSensor->getStateTopic(), powManager.getPowerFactor());

        powWatchdog.feed();
    }
#endif

std::vector<std::pair<String, String>> getWebServerData()
{
    std::vector<std::pair<String, String>> webServerData;

    std::pair<String, String> generic_pair;

    generic_pair.first = "wifi_ssid";
    generic_pair.second = wifi_ssid;
    webServerData.push_back(generic_pair);

    generic_pair.first = "wifi_password";
    generic_pair.second = wifi_password;
    webServerData.push_back(generic_pair);

    generic_pair.first = "ip";
    generic_pair.second = ip;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mask";
    generic_pair.second = mask;
    webServerData.push_back(generic_pair);

    generic_pair.first = "gateway";
    generic_pair.second = gateway;
    webServerData.push_back(generic_pair);

    generic_pair.first = "ota_server";
    generic_pair.second = ota_server;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_server";
    generic_pair.second = mqtt_server;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_port";
    generic_pair.second = mqtt_port;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_username";
    generic_pair.second = mqtt_username;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_password";
    generic_pair.second = mqtt_password;
    webServerData.push_back(generic_pair);

    generic_pair.first = "device_name";
    generic_pair.second = device_name;
    webServerData.push_back(generic_pair);

    generic_pair.first = "discovery_prefix";
    generic_pair.second = discovery_prefix;
    webServerData.push_back(generic_pair);

    generic_pair.first = "component_name1";
    generic_pair.second = component_name1;
    webServerData.push_back(generic_pair);

    generic_pair.first = "component_type1";
    generic_pair.second = component_type1;
    webServerData.push_back(generic_pair);

    generic_pair.first = "discovery_prefix1";
    generic_pair.second = discovery_prefix1;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_status1";
    generic_pair.second = mqtt_status1;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_command1";
    generic_pair.second = mqtt_command1;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_secondary1";
    generic_pair.second = mqtt_secondary1;
    webServerData.push_back(generic_pair);

    #ifdef RELAY2_PIN
        generic_pair.first = "component_name2";
        generic_pair.second = component_name2;
        webServerData.push_back(generic_pair);

        generic_pair.first = "component_type2";
        generic_pair.second = component_type2;
        webServerData.push_back(generic_pair);

        generic_pair.first = "discovery_prefix2";
        generic_pair.second = discovery_prefix2;
        webServerData.push_back(generic_pair);

        generic_pair.first = "mqtt_status2";
        generic_pair.second = mqtt_status2;
        webServerData.push_back(generic_pair);

        generic_pair.first = "mqtt_command2";
        generic_pair.second = mqtt_command2;
        webServerData.push_back(generic_pair);

        generic_pair.first = "mqtt_secondary2";
        generic_pair.second = mqtt_secondary2;
        webServerData.push_back(generic_pair);
    #endif

    generic_pair.first = "firmware_version";
    generic_pair.second = FIRMWARE_VERSION;
    webServerData.push_back(generic_pair);

    generic_pair.first = "hardware";
    generic_pair.second = HARDWARE;
    webServerData.push_back(generic_pair);

    return webServerData;
}

void webServerSubmitCallback(std::map<String, String> inputFieldsContent)
{
    //Save config to dataManager
    Serial.println("webServerSubmitCallback()");

    dataManager.set("wifi_ssid", inputFieldsContent["wifi_ssid"]);
    dataManager.set("wifi_password", inputFieldsContent["wifi_password"]);
    dataManager.set("ip", inputFieldsContent["ip"]);
    dataManager.set("mask", inputFieldsContent["mask"]);
    dataManager.set("gateway", inputFieldsContent["gateway"]);
    dataManager.set("ota_server", inputFieldsContent["ota_server"]);
    dataManager.set("mqtt_server", inputFieldsContent["mqtt_server"]);
    dataManager.set("mqtt_port", inputFieldsContent["mqtt_port"]);
    dataManager.set("mqtt_username", inputFieldsContent["mqtt_username"]);
    dataManager.set("mqtt_password", inputFieldsContent["mqtt_password"]);
    dataManager.set("device_name", inputFieldsContent["device_name"]);
    dataManager.set("discovery_prefix", inputFieldsContent["discovery_prefix"]);
    dataManager.set("component_name1", inputFieldsContent["component_name1"]);
    dataManager.set("component_type1", inputFieldsContent["component_type1"]);
    dataManager.set("discovery_prefix1", inputFieldsContent["discovery_prefix1"]);
    dataManager.set("mqtt_status1", inputFieldsContent["mqtt_status1"]);
    dataManager.set("mqtt_command1", inputFieldsContent["mqtt_command1"]);
    dataManager.set("mqtt_secondary1", inputFieldsContent["mqtt_secondary1"]);
    #ifdef RELAY2_PIN
        dataManager.set("component_name2", inputFieldsContent["component_name2"]);
        dataManager.set("component_type2", inputFieldsContent["component_type2"]);
        dataManager.set("discovery_prefix2", inputFieldsContent["discovery_prefix2"]);
        dataManager.set("mqtt_status2", inputFieldsContent["mqtt_status2"]);
        dataManager.set("mqtt_command2", inputFieldsContent["mqtt_command2"]);
        dataManager.set("mqtt_secondary2", inputFieldsContent["mqtt_secondary2"]);
    #endif
    ESP.restart(); // Restart device with new config
}

void MQTTcallback(String topicString, String payloadString)
{
    Relay *the_relay;
    String the_status_topic;

    if (topicString == mqtt_command1)
    {
        the_relay = &relay;
        the_status_topic = mqtt_status1;
    }
    #ifdef RELAY2_PIN
    else if(topicString == mqtt_command2)
    {
        the_relay = &relay2;
        the_status_topic = mqtt_status2;
    }
    #endif
    else
    {
        Serial.print("MQTT topic unknown:");
        Serial.println(topicString.c_str());
        return;
    }

    if (payloadString == "ON")
    {
        Serial.println("ON");
        the_relay->on();
        mqttManager.publishMQTT(the_status_topic, "ON");
    }
    else if (payloadString == "OFF")
    {
        Serial.println("OFF");
        the_relay->off();
        mqttManager.publishMQTT(the_status_topic, "OFF");
    }
    else if (payloadString == "TOGGLE")
    {
        Serial.println("TOGGLE");
        the_relay->commute();
        mqttManager.publishMQTT(the_status_topic, the_relay->getState() ? "ON" : "OFF");
    }
    else
    {
        Serial.print("MQTT payload unknown: ");
        Serial.println(payloadString.c_str());
    }
}

void shortPress()
{
    Serial.println("button.shortPress()");
    relay.commute();

    mqttManager.publishMQTT(mqtt_status1, relay.getState() ? "ON" : "OFF");
    mqttManager.publishMQTT(mqtt_command1, relay.getState() ? "ON" : "OFF");
}

void longPress()
{
    Serial.println("button.longPress()");

    Serial.println("Secondary topic: TOGGLE");
    mqttManager.publishMQTT(mqtt_secondary1, "TOGGLE");
}


#ifdef RELAY2_PIN
    void shortPress2()
    {
        Serial.println("button2.shortPress()");
        relay2.commute();

        mqttManager.publishMQTT(mqtt_status2, relay2.getState() ? "ON" : "OFF");
        mqttManager.publishMQTT(mqtt_command2, relay2.getState() ? "ON" : "OFF");
    }

    void longPress2()
    {
        Serial.println("button2.longPress()");
        Serial.println("Secondary topic: TOGGLE");
        mqttManager.publishMQTT(mqtt_secondary2, "TOGGLE");
    }
#endif


void veryLongPress()
{
    //Disconnect and Restart device
    mqttManager.stopConnection();
    wifiManager.disconnectStaWifi();
    ESP.restart();
}

void ultraLongPress()
{
    Serial.println("button.longlongPress()");

    if(wifiManager.apModeEnabled())
    {
        WebServer::getInstance().stop();
        wifiManager.destroyApWifi();

        ESP.restart();
    }
    else
    {
        mqttManager.stopConnection();
        wifiManager.createApWifi();
        WebServer::getInstance().start();
    }
}

void connectionWatchdogCallback()
{
    ESP.restart();
}

void configureMQTTDiscovery(String component_name, String component_type, String discovery_prefix, String mqtt_command, String mqtt_status)
{
    MqttDiscoveryComponent* discoveryComponent;
    discoveryComponent = new MqttDiscoveryComponent(component_type, component_name);
    discoveryComponent->discovery_prefix = discovery_prefix;
    discoveryComponent->setConfigurtionVariable("command_topic", mqtt_command);
    discoveryComponent->setConfigurtionVariable("state_topic", mqtt_status);
    discoveryComponent->setConfigurtionVariable("qos", "1");
    discoveryComponent->setConfigurtionVariable("retain", "true");
    mqttManager.addDiscoveryComponent(discoveryComponent);
}


void setup()
{
    // Init serial comm
    Serial.begin(115200);

    // Configure Relay
    relay.setup(RELAY_PIN, RELAY_MODE);
    relay.off();

    #ifdef RELAY2_PIN
        relay2.setup(RELAY2_PIN, RELAY_MODE);
        relay2.off();
    #endif

    // Configure Button
    button.setup(BUTTON_PIN, ButtonType::PULLUP);
    button.setShortPressCallback(shortPress);
    button.setLongPressCallback(longPress);
    button.setVeryLongPressCallback(veryLongPress);
    button.setUltraLongPressCallback(ultraLongPress);

    #ifdef RELAY2_PIN
        button2.setup(BUTTON2_PIN, ButtonType::PULLUP);
        button2.setShortPressCallback(shortPress2);
        button2.setLongPressCallback(longPress2);
        button2.setVeryLongPressCallback(veryLongPress);
        button2.setUltraLongPressCallback(ultraLongPress);
    #endif

    // Initial flash LED
    #ifdef LED_PIN
        led.setup(LED_PIN, LED_MODE);
        led.on();
        delay(300);
        led.off();
    #endif

    // Configure and connect Wifi
    wifiManager.setup(wifi_ssid, wifi_password, ip, mask, gateway, HARDWARE);
    wifiManager.connectStaWifi();

    // Configure MQTT
    mqttManager.setCallback(MQTTcallback);
    mqttManager.setup(mqtt_server, mqtt_port.c_str(), mqtt_username, mqtt_password, true);
    mqttManager.setDeviceData(device_name, HARDWARE, ip, FIRMWARE, FIRMWARE_VERSION, discovery_prefix);

    // Configure External Button
    #ifdef ENABLE_SONOFF_BUTTON
        externalButton.setup(EXTERNAL_BUTTON_PIN, ButtonType::PULLUP_INTERNAL);
        externalButton.setShortPressCallback(shortPress);
        externalButton.setLongPressCallback(longPress);
        externalButton.setVeryLongPressCallback(veryLongPress);
        externalButton.setUltraLongPressCallback(ultraLongPress);
    #endif

    // Configure Pow
    #ifdef ENABLE_SONOFF_POW
        powManager.setup();

        currentSensor = new MqttDiscoveryComponent("sensor", component_name1 + " Current");
        voltageSensor = new MqttDiscoveryComponent("sensor", component_name1 + " Voltage");
        activePowerSensor = new MqttDiscoveryComponent("sensor", component_name1 + " Active Power");
        apparentPowerSensor = new MqttDiscoveryComponent("sensor", component_name1 + " Apparent Power");
        reactivePowerSensor = new MqttDiscoveryComponent("sensor", component_name1 + " Reactive Power");
        powerFactorSensor = new MqttDiscoveryComponent("sensor", component_name1 + " Power Factor");

        currentSensor->discovery_prefix = discovery_prefix1;
        voltageSensor->discovery_prefix = discovery_prefix1;
        activePowerSensor->discovery_prefix = discovery_prefix1;
        apparentPowerSensor->discovery_prefix = discovery_prefix1;
        reactivePowerSensor->discovery_prefix = discovery_prefix1;
        powerFactorSensor->discovery_prefix = discovery_prefix1;

        currentSensor->setConfigurtionVariable("unit_of_measurement", "A");
        voltageSensor->setConfigurtionVariable("unit_of_measurement", "V");
        activePowerSensor->setConfigurtionVariable("unit_of_measurement", "W");
        apparentPowerSensor->setConfigurtionVariable("unit_of_measurement", "VA");
        reactivePowerSensor->setConfigurtionVariable("unit_of_measurement", "VAr");
        powerFactorSensor->setConfigurtionVariable("unit_of_measurement", "%");

        mqttManager.addDiscoveryComponent(currentSensor);
        mqttManager.addDiscoveryComponent(voltageSensor);
        mqttManager.addDiscoveryComponent(activePowerSensor);
        mqttManager.addDiscoveryComponent(apparentPowerSensor);
        mqttManager.addDiscoveryComponent(reactivePowerSensor);
        mqttManager.addDiscoveryComponent(powerFactorSensor);

        powWatchdog.setup(120000, powWatchdogCallback);
        powWatchdog.init();
        powWatchdog.feed();
    #endif

    // Configure External Switch
    #ifdef ENABLE_SONOFF_SWITCH
        toggleSwitch.setup(SWITCH_PIN, ToggleSwitchType::PULLUP_INTERNAL);
        toggleSwitch.setSingleToggleCallback(shortPress);
        toggleSwitch.setDoubleToggleCallback(longPress);
        toggleSwitch.setTripleToggleCallback(veryLongPress);
        toggleSwitch.setQuadrupleToggleCallback(ultraLongPress);
    #endif

    // Configure MQTT Discovery
    configureMQTTDiscovery(component_name1, component_type1, discovery_prefix1, mqtt_command1, mqtt_status1);
    #ifdef RELAY2_PIN
        configureMQTTDiscovery(component_name2, component_type2, discovery_prefix2, mqtt_command2, mqtt_status2);
    #endif

    // Connect MQTT
    mqttManager.startConnection();

    //Configure WebServer
    WebServer::getInstance().setup("/index.html.gz", webServerSubmitCallback);
    WebServer::getInstance().setData(getWebServerData());

    // OTA setup
    ArduinoOTA.setHostname(device_name.c_str());
    ArduinoOTA.begin();

    // UpdateManager setup
    updateManager.setup(ota_server, FIRMWARE, FIRMWARE_VERSION, HARDWARE);

    // ConnectionWatchdog setup
    connectionWatchdog.setup(1200000, connectionWatchdogCallback); //20 min
}

void loop()
{
    // Process Button events
    button.loop();

    // Process External Button events
    #ifdef ENABLE_SONOFF_BUTTON
        externalButton.loop();
    #endif

    // Process ToggleSwitch events
    #ifdef ENABLE_SONOFF_SWITCH
        toggleSwitch.loop();
    #endif

    // Process 2nd Button events
    #ifdef RELAY2_PIN
        button2.loop();
    #endif

    // Check Wifi status
    wifiManager.loop();

    // Check MQTT status and OTA Updates
    if (wifiManager.connected())
    {
        mqttManager.loop();
        updateManager.loop();
        ArduinoOTA.handle();
    }

    // Handle WebServer connections
    if (wifiManager.apModeEnabled())
    {
        WebServer::getInstance().loop();
    }

    // LED Status and ConnectionWatchdog
    connectionWatchdog.loop();

    if (mqttManager.connected())
    {
        connectionWatchdog.init();
        connectionWatchdog.feed();

        #ifdef LED_PIN
            led.on();
        #endif
    }
    else if(wifiManager.apModeEnabled())
    {
        connectionWatchdog.deinit();

        #ifdef LED_PIN
            led.blink(1000);
        #endif
    }
    else
    {
        #ifdef LED_PIN
            led.off();
        #endif
    }

    // Pow Status
    #ifdef ENABLE_SONOFF_POW
        powManager.loop();
        powWatchdog.loop();
    #endif
}
