#include <Arduino.h>

// See pin problems https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
#define L298N_IN3 26 // A-1A
#define L298N_IN4 27 // A1-B
#define L298N_IN2 25 // B-1A
#define L298N_IN1 33 // B-1B

void backA()
{
  digitalWrite(L298N_IN1, LOW);  // direction = forward
  digitalWrite(L298N_IN2, HIGH); // PWM speed = slow

}

void backB()
{
  digitalWrite(L298N_IN3, HIGH); // direction = forward
  digitalWrite(L298N_IN4, LOW);  // PWM speed = slow
}

void stopA()
{
  digitalWrite(L298N_IN1, LOW); // direction = forward
  digitalWrite(L298N_IN2, LOW); // PWM speed = slow
}

void stopB()
{
  digitalWrite(L298N_IN3, LOW); // direction = forward
  digitalWrite(L298N_IN4, LOW); // PWM speed = slow
}

void forwardA()
{
  digitalWrite(L298N_IN1, HIGH); // direction = forward
  digitalWrite(L298N_IN2, LOW);  // PWM speed = slow
}

void forwardB()
{
  digitalWrite(L298N_IN3, LOW);  // direction = forward
  digitalWrite(L298N_IN4, HIGH); // PWM speed = slow
}


void setup()
{
  // set up motor pins
  pinMode(L298N_IN1, OUTPUT);
  pinMode(L298N_IN2, OUTPUT);
  digitalWrite(L298N_IN1, LOW);
  digitalWrite(L298N_IN2, LOW);

  pinMode(L298N_IN3, OUTPUT);
  pinMode(L298N_IN4, OUTPUT);
  digitalWrite(L298N_IN3, LOW);
  digitalWrite(L298N_IN4, LOW);
}
void loop()
{
  // stop and wait 5 sec before going again
  stopA();
  stopB();
  delay(5000);
}