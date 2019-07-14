#include "KBIot.h"

KBIot iot(&Serial);

void hello(String data){
  Serial.print("data=");
  Serial.println(data);
}

void setup(){
  Serial.begin(115200);
  iot.init();
  iot.subscribe("/hello");
  iot.regGot("/hello", &hello);
}

static uint32_t last;
void loop(){
  iot.loop();
  /*
  if((millis()-last) > 4000){
    iot.publish("/hello", "world:"+String(last));
    last = millis();
  }
  */
}



