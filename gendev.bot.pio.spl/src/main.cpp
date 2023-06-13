// --- INCLUDES -----------------------------------------------------------------------------------

#include <Arduino.h>
#include <WiFi.h>
#include "TCPRequest.h"

// --- DEFINES ------------------------------------------------------------------------------------

#define USE_LAPTOP

#define L298N_IN3 26 // A-1A
#define L298N_IN4 27 // A1-B
#define L298N_IN2 25 // B-1A
#define L298N_IN1 33 // B-1B

// direction of motors -1 BWD, 0 STP, 1 FWD
int dirA = 0;
int dirB = 0;

// time in ms to wait between direction changes
const long waitDirChange = 100;

// See pin problems https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
#define TRIGGER_PIN 5
#define ECHO_PIN 18
#define MAX_DISTANCE 100 // Maximum distance

#define TOUCH_PIN 

#define STARTUP_DELAY 0 // always wait $ ms before going into the loop

#ifdef USE_LAPTOP
String WIFI_NAME = "jor-lap";
String WIFI_PWD = "********";
String SERVER_IP = "192.168.137.1";
uint16_t SERVER_PORT = 5000;
#else
String WIFI_NAME = "Testing";
String WIFI_PWD = "SE_2022_WLAN";
String SERVER_IP = "192.168.1.247";
uint16_t SERVER_PORT = 5000;
#endif

unsigned long previousMillis = 0;

uint16_t TCP_TIMEOUT = 500;
TCPRequest tcp_conn(SERVER_IP, SERVER_PORT);

#define DBG_DELAY 1000

// --- FUNCTIONS ----------------------------------------------------------------------------------

void wifi_setup(String wifi_name, String wifi_pwd)
{
  WiFi.mode(WIFI_MODE_STA); // set WiFi to station == client
  WiFi.begin(wifi_name.c_str(), wifi_pwd.c_str());

  Serial.print("Connecting WiFi ");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }

  Serial.print(" done. My IP is ");
  Serial.println(WiFi.localIP().toString());
}

void forwardA()
{
  digitalWrite(L298N_IN1, HIGH); // direction = forward
  digitalWrite(L298N_IN2, LOW);  // PWM speed = slow
}

void forwardB()
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

void backA()
{
  digitalWrite(L298N_IN1, LOW);  // direction = forward
  digitalWrite(L298N_IN2, HIGH); // PWM speed = slow
}

void backB()
{
  digitalWrite(L298N_IN3, LOW);  // direction = forward
  digitalWrite(L298N_IN4, HIGH); // PWM speed = slow
}

int getDistance()
{
  // pulse the USS
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // abort after 6 milliseconds ~ 100cm
  long duration = pulseIn(ECHO_PIN, HIGH, 6000);

  if (duration != 0)
  {
    // roughly cm
    return duration / 58;
  }
  else
  {
    return MAX_DISTANCE;
  }
}

// --- SETUP --------------------------------------------------------------------------------------

void setup()
{
  delay(STARTUP_DELAY);

  Serial.begin(115200);

  wifi_setup(WIFI_NAME, WIFI_PWD);

  tcp_conn.setTimeout(TCP_TIMEOUT);
  // tcp_conn.setTimeout(DBG_DELAY + TCP_TIMEOUT);
  tcp_conn.setReuseConnection(true);

  // USS
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // motor pins
  pinMode(L298N_IN1, OUTPUT);
  pinMode(L298N_IN2, OUTPUT);
  digitalWrite(L298N_IN1, LOW);
  digitalWrite(L298N_IN2, LOW);

  pinMode(L298N_IN3, OUTPUT);
  pinMode(L298N_IN4, OUTPUT);
  digitalWrite(L298N_IN3, LOW);
  digitalWrite(L298N_IN4, LOW);
}

// --- MAIN ---------------------------------------------------------------------------------------

void loop()
{
  // ---- MAIN SECTION ------------------------------------

  if (touchRead(32) < 20)
  {
    stopA();
    stopB();
    delay(100);
    return;
  }
  int distance = getDistance();

  /* TODO remove */ Serial.printf("DBG::distance = %d\n", distance); // TODO remove

  tcp_conn.setPayload(String(distance));
  tcp_conn.run();

  if (!tcp_conn.isTimedOut())
  {
    String response = tcp_conn.getResponse();
    Serial.println(response);

    char leftM = response.charAt(0);
    if (leftM == 'F')
    {
      backB();
    }
    else if (leftM == 'B')
    {
      forwardB();
    }
    else
    {
      stopB();
    }

    char rightM = response.charAt(2);
    if (rightM == 'F')
    {
      forwardA();
    }
    else if (rightM == 'B')
    {
      backA();
    }
    else
    {
      stopA();
    }
  }
  else
  {
    Serial.printf("DBG::tcp timeout\n"); // TODO remove
    stopA();
    stopB();
  }
}