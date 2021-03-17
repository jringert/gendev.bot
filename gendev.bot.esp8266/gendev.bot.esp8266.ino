#include <Arduino.h>
//See available PINs and possible problems https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //https://arduinojson.org
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#define USE_MOTORS

#ifdef USE_MOTORS
#define MOTOR_A_PWM 13 //D7 A-1A
#define MOTOR_A_DIR 5  //D1 A1-B
#define MOTOR_B_PWM 14 //D5 B-1A
#define MOTOR_B_DIR 16 //D0 B-1B
#define PWM_SLOW 700  // slow speed PWM duty cycle
// direction of motors -1 BWD, 0 STP, 1 FWD
int dirA = 0;
int dirB = 0;
// time in ms to wait between direction changes
const long waitDirChange = 100;
#endif

#define USE_USS

#ifdef USE_USS
#include <NewPing.h> //https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home
#define TRIGGER_ECHO_PIN     12 //D6
#define MAX_DISTANCE 100 // Maximum distance
NewPing sonar(TRIGGER_ECHO_PIN, TRIGGER_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#endif

int distance = 0;

unsigned long previousMillis = 0;
// min time between requests in ms
const long interval = 50;

HTTPClient http;

void setup() {

  Serial.begin(115200);

  // create a portal if cannot connect to previous AP
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  if(!wm.autoConnect("GendevBotAP")) {
    Serial.println("Failed to connect");
    ESP.restart();
  } else {
    Serial.println("connected");
  }
  http.setReuse(true);
  
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
      Serial.printf("[HTTP] trying to begin");
      if (http.begin(client, "TODO IP or host name", 8080, "/robot?distance=" + String(distance), false)) {
        int httpCode = http.GET();
        if (httpCode > 0) {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
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
