#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

#define USE_MOTORS

#ifdef USE_MOTORS
#define MOTOR_A_PWM 13 //D7 A-1A
#define MOTOR_A_DIR 5  //D1 A1-B
#define MOTOR_B_PWM 14 //D5 B-1A
#define MOTOR_B_DIR 12 //D6 B-1B
#define PWM_SLOW 700  // arbitrary slow speed PWM duty cycle
#endif

#define USE_USS

#ifdef USE_USS
#include <NewPing.h>
#define TRIGGER_PIN  0 //D3 // SHOULD NOT BE USED -- fails to boot sometimes
#define ECHO_PIN     2 //D4 // SHOULD NOT BE USED -- fails to boot sometimes and prints garbage
#define MAX_DISTANCE 100 // Maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#endif

int distance = 0;

unsigned long previousMillis = 0;
const long interval = 50;    


void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin("TODO SSID name of WiFi", "TODO WiFi password");
  
  #ifdef USE_MOTORS
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );
  #endif
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
  #ifdef USE_USS
  distance = sonar.ping_cm();
  if (distance == 0) { //(0 = outside set distance range)
    distance = MAX_DISTANCE;
  }
  Serial.println(distance);
  #endif
    
  // check for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    Serial.printf("[HTTP] trying to begin");
    if (http.begin(client, "TODO IP or host name", 8080, "/robot?distance=" + String(distance), false)) {
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          StaticJsonDocument<200> doc;
          deserializeJson(doc, payload);
                    
          const char* leftM = doc["leftM"];
          const char* rightM = doc["rightM"];

          #ifdef USE_MOTORS
          if (strcmp(leftM, "FWD")==0) {
            forwardA();
          } else if (strcmp(leftM, "BWD")==0) {
            backA();
          } else {
            stopA();
          }
          if (strcmp(rightM, "FWD")==0) {
            forwardB();
          } else if (strcmp(rightM, "BWD")==0) {
            backB();
          } else {
            stopB();
          }
          #endif
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
  } else {
    Serial.printf("[WiFi] no wifi\n");
    #ifdef USE_MOTORS
    stopA();
    stopB();
    #endif
  }
  
  }// millis checker
}

#ifdef USE_MOTORS
void forwardA() {
  digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
  analogWrite( MOTOR_A_PWM, 1023-PWM_SLOW ); // PWM speed = slow
}
void forwardB() {
  digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
  analogWrite( MOTOR_B_PWM, 1023-PWM_SLOW ); // PWM speed = slow
}

void stopA() {
  digitalWrite( MOTOR_A_DIR, LOW ); // direction = forward
  digitalWrite( MOTOR_A_PWM, LOW ); // PWM speed = slow
}
void stopB() {
  digitalWrite( MOTOR_B_DIR, LOW ); // direction = forward
  digitalWrite( MOTOR_B_PWM, LOW ); // PWM speed = slow
}

void backA() {
  digitalWrite( MOTOR_A_DIR, LOW ); // direction = forward
  analogWrite( MOTOR_A_PWM, PWM_SLOW ); // PWM speed = slow
}
void backB() {
  digitalWrite( MOTOR_B_DIR, LOW ); // direction = forward
  analogWrite( MOTOR_B_PWM, PWM_SLOW ); // PWM speed = slow
}
#endif
