// --- CONFIG -------------------------------------------------------------------------------------
//#define STARTUP_DELAY // wait 5 seconds at every startup to connect to serial monitor
#define USE_LAPTOP // use hotspot on laptop as access point
//#define USE_ROUTER // use router as access point
#define TRACE // print debug messages to serial monitor
#define USE_MOTORS // use motors
#define USE_USS // use ultrasonic sensor
#define USE_TOUCH // use touch sensor as emergency stop
#define USE_WIFI // use WiFi to connect to server

// --- INCLUDES -----------------------------------------------------------------------------------
#include <Arduino.h>
#ifdef USE_WIFI
#include <WiFi.h>
#include "TCPRequest.h"
#endif


// --- DEFINES ------------------------------------------------------------------------------------
// See pin problems https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

#ifdef USE_MOTORS
#define L298N_IN3 26 // A-1A
#define L298N_IN4 27 // A1-B
#define L298N_IN2 25 // B-1A
#define L298N_IN1 33 // B-1B
#endif

#ifdef USE_USS
#define TRIGGER_PIN 5
#define ECHO_PIN 18
#endif
#define MAX_DISTANCE 100 // Maximum distance

#ifdef USE_TOUCH
#define TOUCH_PIN 32
#endif

#define DBG_DELAY 0   // wait $ ms longer for timeouts to debug

#ifdef USE_WIFI
#ifdef USE_LAPTOP
String WIFI_NAME = "jor-lap";
String WIFI_PWD = "********";
String SERVER_IP = "192.168.137.1";
uint16_t SERVER_PORT = 5000;
#endif
#ifdef USE_ROUTER
String WIFI_NAME = "Testing";
String WIFI_PWD = "SE_2022_WLAN";
String SERVER_IP = "192.168.1.247";
uint16_t SERVER_PORT = 5000;
#endif

uint16_t TCP_TIMEOUT = 500;
TCPRequest tcp_conn(SERVER_IP, SERVER_PORT);

// --- FUNCTIONS ----------------------------------------------------------------------------------

void wifi_setup(String wifi_name, String wifi_pwd)
{
  WiFi.mode(WIFI_MODE_STA); // set WiFi to station == client
  WiFi.begin(wifi_name.c_str(), wifi_pwd.c_str());

  #ifdef TRACE
  Serial.print("Connecting WiFi "); 
  #endif

  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef TRACE
    Serial.print(".");
    #endif
    delay(250);
  }

  #ifdef TRACE
  Serial.print(" done. My IP is ");
  Serial.println(WiFi.localIP().toString());
  #endif
}
#endif

#ifdef USE_MOTORS
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
#endif

int getDistance()
{
  #ifdef USE_USS
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
  #endif
    return MAX_DISTANCE;
  #ifdef USE_USS
  }
  #endif
}


void setup()
{
  #ifdef STARTUP_DELAY
  delay(5000); // wait 5 seconds for serial monitor
  #endif

  #ifdef TRACE
  Serial.begin(115200);
  #endif

  #ifdef USE_WIFI
  wifi_setup(WIFI_NAME, WIFI_PWD);

  tcp_conn.setTimeout(TCP_TIMEOUT);
  tcp_conn.setTimeout(DBG_DELAY + TCP_TIMEOUT);
  tcp_conn.setReuseConnection(true);
  #endif

  #ifdef USE_USS  
  // set up USS pins
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  #endif

  #ifdef USE_MOTORS
  // set up motor pins
  pinMode(L298N_IN1, OUTPUT);
  pinMode(L298N_IN2, OUTPUT);
  digitalWrite(L298N_IN1, LOW);
  digitalWrite(L298N_IN2, LOW);

  pinMode(L298N_IN3, OUTPUT);
  pinMode(L298N_IN4, OUTPUT);
  digitalWrite(L298N_IN3, LOW);
  digitalWrite(L298N_IN4, LOW);
  #endif
}

void loop()
{
  #ifdef USE_TOUCH
  if (touchRead(TOUCH_PIN) < 20)
  {
    #ifdef USE_MOTORS
    stopA();
    stopB();
    #endif
    delay(100);
    return;
  }
  #endif

  int distance = getDistance();

  #ifdef TRACE
  Serial.printf("DBG::distance = %d\n", distance);
  #endif

  #ifdef USE_WIFI
  tcp_conn.setPayload(String(distance));
  tcp_conn.run();

  if (!tcp_conn.isTimedOut())
  {
    String response = tcp_conn.getResponse();
    #ifdef TRACE
    Serial.println(response);
    #endif

    #ifdef USE_MOTORS
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
    #endif
  }
  else
  {
    #ifdef TRACE
    Serial.printf("DBG::tcp timeout\n");
    #endif

    #ifdef USE_MOTORS
    stopA();
    stopB();
    #endif
  }
  #endif
}