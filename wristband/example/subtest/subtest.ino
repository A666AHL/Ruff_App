#include <Arduino.h>
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

void loop(){
  iot.loop();
}



