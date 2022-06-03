#include <Arduino.h>

#include <ArduinoJson.h> //https://arduinojson.org
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#define MOTOR_A_PWM 26 //A-1A
#define MOTOR_A_DIR 27 //A1-B
#define MOTOR_B_PWM 33 //B-1A
#define MOTOR_B_DIR 25 //B-1B
// next one depends a lot on battery power
#define PWM_SLOW 1023 //700  // slow speed PWM duty cycle
// direction of motors -1 BWD, 0 STP, 1 FWD
int dirA = 0;
int dirB = 0;
// time in ms to wait between direction changes
const long waitDirChange = 100;

//See pin problems https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
#define TRIGGER_PIN  5
#define ECHO_PIN     18
#define MAX_DISTANCE 100 // Maximum distance



int distance = 0;

#include <HTTPClient.h>
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

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );
}

void loop() {
  // pulse the USS
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // abort after 6 milliseconds ~ 100cm
  long duration = pulseIn(ECHO_PIN, HIGH, 6000);
  Serial.printf("duration: %ld\n", duration);
  if (duration != 0) {
    // roughly cm
    distance = (duration / 58);
  } else {
    distance = MAX_DISTANCE;
  }
  Serial.println(distance);
    
  // check for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[HTTP] trying to begin");
    if (http.begin("192.168.137.1", 8080, "/robot?distance=" + String(distance))) {
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          StaticJsonDocument<100> doc;
          deserializeJson(doc, payload);
                    
          const char* leftM = doc["leftM"];
          const char* rightM = doc["rightM"];

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
    stopA();
    stopB();
  }
}

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
