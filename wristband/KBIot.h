#ifndef KITTENBOTIOT_H
#define KITTENBOTIOT_H

#include <avr/pgmspace.h>
#include <HardwareSerial.h>
#include <Arduino.h>

#define IOT_CB_WIFISTATE 1
#define IOT_CB_MQTTCONN 2
#define IOT_CB_MQTTDISCON 3
#define IOT_CB_MQTTPUB 4
#define IOT_CB_MQTTDATA 5

#define MAX_TOPIC 8
typedef void (*Callback)(String data);

class KBIot
{
    public:
        KBIot(Stream* serial);
        void init();
        void publish(String topic, String data);
        void publish(String topic, int data);
        void publish(String topic, const char* data);
        void subscribe(String topic);
        void connectAP(String host, String pass);
        void mqttConect(String server, String cid);
        void mqttConect(String server, String cid, String user, String pass);
        int regGot(String topic, Callback fun);
        void regWifiCb(Callback fun);
        void loop();

    private:
        char serBuf[128];
        int bufindex;
        Stream * _ser;
        void mqttInstallCallback(void);
        void parseCommand(char * buf);
        void mqttParseData(char * buf) __attribute__((__optimize__("O2"))); // fix for compiler error
        void mqttParseWifiState(char * buf) __attribute__((__optimize__("O2"))); // fix for compiler error
};

#endif
