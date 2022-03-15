#include "AutoBlinds.h"
#include "FrontEndHandling.h"
#include "StepperHandling.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>

#ifndef STASSID
#define STASSID "Adibas"
#define STAPSK  "SlozhniPar0l?"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

void setup(void) {
  pinMode(stopSwitch_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(disableMotor_pin, OUTPUT);
  pinMode(stepper_mode_pin0, OUTPUT);
  pinMode(stepper_mode_pin1, OUTPUT);
  pinMode(stepper_mode_pin2, OUTPUT);

  digitalWrite(led_pin, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/postplain/", handlePlain);

  server.on("/postform/", handleForm);
  
  server.on("/postformCommand/", handleFormCommand);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  motorTicker.attach_ms(motor_ticker_period_ms, updateMove);
}

void loop(void) {
  server.handleClient();
  handleStopSwitch();
}
