#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //https://arduinojson.org
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

//#define USE_MOTORS
//#define USE_USS
#define USE_SERIAL


#ifdef USE_MOTORS
#define MOTOR_A_PWM 13 //D7 A-1A
#define MOTOR_A_DIR 5  //D1 A1-B
#define MOTOR_B_PWM 14 //D5 B-1A
#define MOTOR_B_DIR 12 //D6 B-1B
// next one depends a lot on battery power
#define PWM_SLOW 1023 //700  // slow speed PWM duty cycle
// direction of motors -1 BWD, 0 STP, 1 FWD
int dirA = 0;
int dirB = 0;
// time in ms to wait between direction changes
const long waitDirChange = 100;
#endif

#ifdef USE_USS
//See pin problems https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
#define TRIGGER_PIN  0 //D3 // DISCONNECT for FLASH and BOOT
#define ECHO_PIN     2 //D4 // DISCONNECT for FLASH and BOOT
#define MAX_DISTANCE 100 // Maximum distance
#endif



int distance = 0;

unsigned long previousMillis = 0;
// min time between requests in ms
const long interval = 50;

HTTPClient http;

void setup() {
  #ifdef USE_SERIAL
  Serial.begin(115200);
  #endif

  // create a portal if cannot connect to previous AP
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  if(!wm.autoConnect("GendevBotAP")) {
    #ifdef USE_SERIAL
    Serial.println("Failed to connect");
    #endif
    ESP.restart();
  } else {
    #ifdef USE_SERIAL
    Serial.println("connected");
    #endif
  }
  http.setReuse(true);

  #ifdef USE_USS
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  #endif
  
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
    // pulse the USS
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    // abort after 6 milliseconds ~ 100cm
    long duration = pulseIn(ECHO_PIN, HIGH, 6000);
    if (duration != 0) {
      // roughly cm
      distance = (duration / 58);
    }
    if (distance == 0) { //(0 = outside set distance range)
      distance = MAX_DISTANCE;
    }
    #ifdef USE_SERIAL
    Serial.println(distance);
    #endif
    #endif
      
    // check for WiFi connection
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;    
      #ifdef USE_SERIAL
      Serial.printf("[HTTP] trying to begin");
      #endif
      if (http.begin(client, "192.168.137.1", 8080, "/robot?distance=" + String(distance), false)) {        
        int httpCode = http.GET();        
        if (httpCode > 0) {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            #ifdef USE_SERIAL
            Serial.println(payload);
            #endif
            StaticJsonDocument<100> doc;
            deserializeJson(doc, payload);
                      
            const char* leftM = doc["leftM"];
            const char* rightM = doc["rightM"];
  
            #ifdef USE_MOTORS
            int dirChange = 0;
            if (strcmp(leftM, "FWD")==0) {
              if (dirA == -1) {
                dirChange++;
              }
              dirA = 1;
            } else if (strcmp(leftM, "BWD")==0) {
              if (dirA == 1) {
                dirChange++;
              }
              dirA = -1;
            } else {
              dirA = 0;
            }
            if (strcmp(rightM, "FWD")==0) {
              if (dirB == -1) {
                dirChange++;
              }
              dirB = 1;
            } else if (strcmp(rightM, "BWD")==0) {
              if (dirB == 1) {
                dirChange++;
              }
              dirB = -1;
            } else {
              dirB = 0;
            }
            // delay of one motor changes direction
            if (dirChange > 0) {
              // important to stop both to avoid asymmetric turns etc.
              stopA();
              stopB();
              delay(dirChange * waitDirChange);
            }
            // issue actual commands
            if (dirA == -1) {
              backA();
            } else if (dirA == 0) {
              stopA();
            } else {
              forwardA();
            }
            if (dirB == -1) {
              backB();
            } else if (dirB == 0) {
              stopB();
            } else {
              forwardB();
            }
            #endif
          }
        } else {
          #ifdef USE_SERIAL
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          #endif
        }
        http.end();
      } else {
        #ifdef USE_SERIAL
        Serial.printf("[HTTP] Unable to connect\n");
        #endif
      }
    } else {
      #ifdef USE_SERIAL
      Serial.printf("[WiFi] no wifi\n");
      #endif
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
