#include <Arduino.h>

#ifdef WIFI
#include <ArduinoJson.h> //https://arduinojson.org
#include <WiFi.h>
#endif

#define MOTOR_A_PWM 26 //A-1A
#define MOTOR_A_DIR 27 //A1-B
#define MOTOR_B_PWM 33 //B-1A
#define MOTOR_B_DIR 25 //B-1B
// next one depends a lot on battery power
#define PWM_SLOW 1023 //700  // slow speed PWM duty cycle

#define TRIGGER_PIN  5
#define ECHO_PIN     18
#define MAX_DISTANCE 100 // Maximum distance

int distance = 0;

// features
#define TRACING
#define EMG_HW
//#define WIFI

#ifdef WIFI
#include <HTTPClient.h>
HTTPClient http;
#endif

void setup() {
  #ifdef TRACING
  Serial.begin(115200);
  #endif

  #ifdef WIFI
  // create a portal if cannot connect to previous AP
  WiFi.mode(WIFI_STA);
  WiFi.begin("jor", "00000000");
  http.setReuse(true);
  #endif

  
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
  distance = getDistance();
  #ifdef TRACING
  Serial.println(distance);
  #endif

  #ifdef EMG_HW
  int touch = touchRead(4);
  #endif
  #ifdef TRACING
  Serial.println(touch);
  #endif


  #ifdef WIFI
  #ifdef TRACING
  Serial.printf("[HTTP] trying to begin");
  #endif
  #endif

  #ifdef WIFI
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
  #endif
  
  #ifdef EMG_HW
  if (touch > 30) {
  #endif
    if (distance > 30) {
      going();    
    } else {
      turning();
    }
  #ifdef EMG_HW  
  } else {
    stopping();
  }
  #endif

  
}

int getDistance() {
  // pulse the USS
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // abort after 6 milliseconds ~ 100cm
  long duration = pulseIn(ECHO_PIN, HIGH, 6000);
  #ifdef TRACING
  Serial.printf("duration: %ld\n", duration);
  #endif
  if (duration != 0) {
    // roughly cm
    return (duration / 58);
  } else {
    return MAX_DISTANCE;
  }
}

void going() {
  digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
  analogWrite( MOTOR_A_PWM, 1023-PWM_SLOW ); // PWM speed = slow
  digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
  analogWrite( MOTOR_B_PWM, 1023-PWM_SLOW ); // PWM speed = slow
}

void turning() {
  digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
  analogWrite( MOTOR_A_PWM, 1023-PWM_SLOW ); // PWM speed = slow
  digitalWrite( MOTOR_B_DIR, LOW ); // direction = forward
  analogWrite( MOTOR_B_PWM, PWM_SLOW ); // PWM speed = slow
}

void stopping() {
  digitalWrite( MOTOR_A_DIR, LOW ); // direction = forward
  digitalWrite( MOTOR_A_PWM, LOW ); // PWM speed = slow
  digitalWrite( MOTOR_B_DIR, LOW ); // direction = forward
  digitalWrite( MOTOR_B_PWM, LOW ); // PWM speed = slow
}

void backing() {
  digitalWrite( MOTOR_A_DIR, LOW ); // direction = forward
  analogWrite( MOTOR_A_PWM, PWM_SLOW ); // PWM speed = slow
  digitalWrite( MOTOR_B_DIR, LOW ); // direction = forward
  analogWrite( MOTOR_B_PWM, PWM_SLOW ); // PWM speed = slow
}
