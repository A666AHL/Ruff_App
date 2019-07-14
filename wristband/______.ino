#include<Arduino.h>
#include<Servo.h>
#include "KBIot.h"

Servo myservo;
int pos=0;
KBIot iot(&Serial);

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

#define WiDo_IRQ   7
#define WiDo_VBAT  5
#define WiDo_CS    10
Adafruit_CC3000 WiDo = Adafruit_CC3000(WiDo_CS, WiDo_IRQ, WiDo_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "HUAWEI nova 3e"           // cannot be longer than 32 characters!
#define WLAN_PASS       "00000000"

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define TIMEOUT_MS  1000

int temp,light,wet;//全局变量下的温湿光照

void wifisetup()
{
  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!WiDo.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while(1);
  }
    
  /* NOTE: Secure connections are not available in 'Tiny' mode!
     By default connectToAP will retry indefinitely, however you can pass an
     optional maximum number of retries (greater than zero) as the fourth parameter.
  */
  
  Serial.println(F("Connecting Router/AP"));
  if (!WiDo.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  
  Serial.println(F("Router/AP Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
}
 
int servo_write(int a)
{
  for(pos=0;pos<=180;pos++)
  {
    myservo.write(pos);
    delay(10*a);
  }
}


void setup() {
  Serial.begin(115200);
  wifisetup();
  myservo.attach(8);
  pinMode(10,INPUT);
  iot.init();
  iot.mqttConect("192.168.43.210", "robot01");
  iot.subscribe("IOT");
  iot.subscribe("IOTmessage");
}


void loop() {
  iot.loop();
  for(int a=0;a<4;a++)
  {
    int IO=digitalRead(10);
    if(IO!=1)
    {
      break;
    }
    delay(10);
    if(a==4)
    {
      servo_write(1);
      iot.publish("IOTmessage","update");
    }
  }
}

