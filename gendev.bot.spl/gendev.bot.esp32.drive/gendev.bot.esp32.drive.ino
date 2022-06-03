#include <Arduino.h>

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

void setup() {
  Serial.begin(115200);

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

  if (distance > 30) {
    going();    
  } else {
    turning();
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
