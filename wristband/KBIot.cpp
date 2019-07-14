#include "KBIot.h"

struct CallbackDict{
    String topic;
    Callback fun;
};

CallbackDict mqttCallback[MAX_TOPIC];
Callback wifiCb = 0;
uint8_t connectState; // 0: no conn, 1: reconn, 2: connecting, 3: connected
uint8_t wifistate;
unsigned long timer;
String ipaddr;

KBIot::KBIot(Stream* serial): _ser(serial) {
    
}



void KBIot::mqttParseWifiState(char * buf){
    wifistate = atoi(buf);
    ipaddr = String(buf+2);
    if (wifistate == 5){
        mqttInstallCallback();
        if (wifiCb) wifiCb(ipaddr);
    }
    
}

void KBIot::mqttParseData(char * buf)
{
    int i;
    // _ser->println(buf);
    for (i=0;i<strlen(buf);i++){
        if (buf[i] == ' '){
            buf[i] = '\0';
        }
    }
    String mqttTopic = String(buf);
    String mqttData = String(buf+i);

    
    for(int i=0;i<MAX_TOPIC;i++){
        if(mqttCallback[i].topic == mqttTopic){
            mqttCallback[i].fun(mqttData);
            return;
        }
    }
}

void KBIot::parseCommand(char * buf){
    int cmd, argc, cb;
    
    sscanf(buf, "%d %d %d", &cmd, &argc, &cb);
    // _ser->printf("wf %d %d %d\n", cmd, argc, cb);
    if (cmd == 3){ // callback 
        if (cb == IOT_CB_WIFISTATE){
            mqttParseWifiState(buf+6);
        } else if (cb == IOT_CB_MQTTDATA){
            mqttParseData(buf+6);
        } else if (cb == IOT_CB_MQTTCONN){
            for(int i=0;i<MAX_TOPIC;i++){
                if(mqttCallback[i].fun){
                    subscribe(mqttCallback[i].topic);
                }
            }
        }
    } else if (cmd == 42){ // socket data
        
        
    } else if (cmd == 23){ // reset request return
        
        
    }
}

void KBIot::mqttInstallCallback(){
    _ser->println("WF 10 4 0 2 3 4 5"); // mqtt callback install
}

void KBIot::init() {
    _ser->write("\r\n");
    _ser->println("WF 1 0 1"); // sync cmd, wifi status callback
    delay(1000);
    mqttInstallCallback();
    delay(200);
}

void KBIot::loop() {
    while(_ser->available()){
        char c = _ser->read();
        serBuf[bufindex++] = c;
        if (c == '\n') {
          serBuf[bufindex] = '\0';
          // parse input command
          if (serBuf[0] == 'W' && serBuf[1] == 'F'){
              parseCommand(serBuf+3);
          }
          memset(serBuf, 0, 128);
          bufindex = 0;
        }
        if (bufindex >= 128) {
          bufindex = 0;
        }
    }
}

int KBIot::regGot(String topic, Callback fun) {
    for(int i=0;i<MAX_TOPIC;i++){
        if(mqttCallback[i].fun == 0){
            mqttCallback[i].topic = topic;
            mqttCallback[i].fun = fun;
            return i;
        }
    }
    return -1;
}

void KBIot::regWifiCb(Callback cb){
    wifiCb = cb;
}

void KBIot::publish(String topic, String data) {
    _ser->print("WF 11 4 11 0 0 ");
    _ser->print(topic);
    _ser->print(" ");
    _ser->println(data);
}

void KBIot::publish(String topic, int data) {
    _ser->print("WF 11 4 11 0 0 ");
    _ser->print(topic);
    _ser->print(" ");
    _ser->println(data, 10);
}

void KBIot::publish(String topic, const char* data) {
    _ser->print("WF 11 4 11 0 0 ");
    _ser->print(topic);
    _ser->print(" ");
    _ser->println(data);
}

void KBIot::subscribe(String topic){
    _ser->print("WF 12 2 0 ");
    _ser->print(topic);
    _ser->println(" 0");
    delay(500);
}

void KBIot::connectAP(String host, String pass){
    _ser->print("WF 52 2 52 ");
    _ser->print(host);
    _ser->print(" ");
    _ser->println(pass);
    delay(500);

}

void KBIot::mqttConect(String server, String cid){
    _ser->print("WF 15 2 15 ");
    _ser->print(server);
    _ser->print(" ");
    _ser->println(cid);
    delay(1000);
    mqttInstallCallback();
}

void KBIot::mqttConect(String server, String cid, String user, String pass){
    _ser->print("WF 15 4 15 ");
    _ser->print(server);
    _ser->print(" ");
    _ser->print(cid);
    _ser->print(" ");
    _ser->print(user);
    _ser->print(" ");
    _ser->println(pass);
    delay(1000);
    mqttInstallCallback();
}





