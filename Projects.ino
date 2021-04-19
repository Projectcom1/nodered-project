#define BLYNK_PRINT Serial

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

//Servo
Servo petGate;
#define GATE_PIN D6
int gateStart     = 75;
int gateEnd       = 45;
int gateOpenTime  = 500;

int Relay1 = D0;
int Relay2 = D1;
int Relay3 = D2;
int Relay4 = D3;

int sensorValue = analogRead(A0);

int setTemp;
int setHumid;
int setWarter;


bool _button_ma = false;
bool _button1 = false;
bool _button2 = false;
bool _button3 = false;
bool _button4 = false;

WidgetLED led1(V30);
WidgetLED led2(V31);
WidgetLED led3(V32);
WidgetLED led4(V33);

BLYNK_WRITE(V22) //temperature slider in app on pin V22
{
  setTemp = param.asInt();
}
BLYNK_WRITE(V23) //temperature slider in app on pin V23
{
  setHumid = param.asInt();
}

BLYNK_WRITE(V24) //temperature slider in app on pin V23
{
  setWarter = param.asInt();
}

BLYNK_WRITE(V7)
{
  petGate.write(gateEnd);
  delay(gateOpenTime);
  petGate.write(gateStart);
}

BLYNK_WRITE(V0)
{
  if (param.asInt() == 1) {

    _button_ma = true;

  } else {

    _button_ma = false;

  }
}

BLYNK_WRITE(V40) {

  if (param.asInt() == 1) {
    _button1 = true;
  } else {
    _button1 = false;
  }
}

BLYNK_WRITE(V41) {

  if (param.asInt() == 1) {
    _button2 = true;
    digitalWrite(Relay2, LOW);
    led2.on();
  } else {
    _button2 = false;
    digitalWrite(Relay2, HIGH);
    led2.off();
  }
}

BLYNK_WRITE(V42) {

  if (param.asInt() == 1) {
    _button3 = true;
  } else {
    _button3 = false;
  }
}

BLYNK_WRITE(V43) {

  if (param.asInt() == 1) {
    _button4 = true;
    digitalWrite(Relay4, LOW);
    led4.on();
  } else {
    _button4 = false;
    digitalWrite(Relay4, HIGH);
    led4.off();
  }
}


char auth[] = "OvCeNY-dO55Dg-prKT2J97eNAPYIfTJL";
char ssid[] = "TUTA";
char pass[] = "0800005272";

//char server[] = "oasiskit.com";
//int port = 8080;

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

#define DHT1PIN D4
#define DHT1TYPE DHT11   // probobly not needed to do twice, clean later


DHT dht1(DHT1PIN, DHT1TYPE);

BlynkTimer timer;

void sendsensor1() {
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  t1 = dht1.readTemperature();
  t1 = t1;
  h1 = dht1.readHumidity();
  h1 = h1;
  Blynk.virtualWrite(V11, h1);
  Blynk.virtualWrite(V10, t1);
  Blynk.virtualWrite(V15, sensorValue);
}

void setup() {
  Serial.begin(115200);
  pinMode (Relay1, OUTPUT);
  pinMode (Relay2, OUTPUT);
  pinMode (Relay3, OUTPUT);
  pinMode (Relay4, OUTPUT);
  digitalWrite (Relay1, HIGH);
  digitalWrite (Relay2, HIGH);
  digitalWrite (Relay3, HIGH);
  digitalWrite (Relay4, HIGH);

  petGate.attach(GATE_PIN);
  petGate.write(gateStart);

  Blynk.begin(auth, ssid, pass);

  dht1.begin();

  timer.setInterval(2000L, sendsensor1); // probobly not needed twice, timers are wierd things... wiki..
}

void loop() {
  //delay(2000);
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float f1 = dht1.readTemperature(true);
  if (isnan(h1) || isnan(t1) || isnan(f1)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // print out the value you read:
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);


  //auto
  if ( _button_ma == true) {
    if (sensorValue >= setWarter )
    {
      digitalWrite (Relay1, HIGH);
      //    digitalWrite (Relay2,LOW);
      led1.off();
      //      led2.on();
    }
    else if (sensorValue < setWarter)
    {
      digitalWrite (Relay1, LOW);
      //    digitalWrite (Relay2,HIGH);
      led1.on();
      //      led2.off();
    }
    if (h1 <= setHumid)
    {
      digitalWrite (Relay3, LOW);
      //    digitalWrite (Relay4,LOW);
      led3.on();
      //      led4.on();
    }
    else if (h1 > setHumid)
    {
      digitalWrite (Relay3, HIGH);
      //    digitalWrite (Relay4,HIGH);
      led3.off();
      //      led4.off();
    }
  } else {  //manual
    if (_button1 == true) {
      digitalWrite(Relay1, LOW);
      led1.on();
    } else {
      digitalWrite(Relay1, HIGH);
      led1.off();
    }
    //                if (_button2 == true){
    //            digitalWrite(Relay2, LOW);
    //            led2.on();
    //        }else{
    //            digitalWrite(Relay2, HIGH);
    //            led2.off();
    //        }
    if (_button3 == true) {
      digitalWrite(Relay3, LOW);
      led3.on();
    } else {
      digitalWrite(Relay3, HIGH);
      led3.off();
    }
    //                if (_button4 == true){
    //            digitalWrite(Relay4, LOW);
    //            led4.on();
    //        }else{
    //            digitalWrite(Relay4, HIGH);
    //            led4.off();
    //        }
  }

  ////ควบคุม PUM แบบ manual ไหลตลอด ควบคุม เปิด/ปิด ได้
  //   if (_button2 == true){
  //            digitalWrite(Relay2, LOW);
  //            led2.on();
  //        }else{
  //            digitalWrite(Relay2, HIGH);
  //            led2.off();
  //        }
  //
  ////ควบคุมLIGHT(ไฟ) แบบ manual ติดตลอด ควบคุม เปิด/ปิด ได้
  //        if (_button4 == true){
  //            digitalWrite(Relay4, LOW);
  //            led4.on();
  //        }else{
  //            digitalWrite(Relay4, HIGH);
  //            led4.off();
  //        }

  float hif = dht1.computeHeatIndex(f1, h1);
  float hic = dht1.computeHeatIndex(t1, h1, false);
  Serial.print(F("Humidity: "));
  Serial.print(h1);
  Serial.print(F("% Temperature: "));
  Serial.print(t1);
  Serial.print(F(" C\n"));
  Blynk.run();
  timer.run();
}
