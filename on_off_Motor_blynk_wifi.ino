#define BLYNK_TEMPLATE_ID "TMPL60ZzIP8B9"
#define BLYNK_TEMPLATE_NAME "myRobot"
#define BLYNK_PRINT Serial

#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

#include <BlynkSimpleEsp32.h>
#include <esp_arduino_version.h>

// Isi dengan nama dan password Wi-Fi yang dipakai ESP32.
const char* ssid = WIFI_SSID;
const char* pass = WIFI_PASSWORD;
const int ledPin = 2;
char myData = 0;

const int motor1Pin1 = 27;
const int motor1Pin2 = 26;
const int enable1Pin = 12;

// PWM motor
const int freq = 30000;
const int pwmChannel = 0;  // Dipakai ESP32 Arduino core 2.x
const int resolution = 8;
int dutyCycle = 0;

const byte pin_rpm = 13;
volatile int rev = 0;

void writeMotorPwm(int value)
{
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  // Core 3.x memakai GPIO pin, bukan channel, untuk ledcWrite().
  ledcWrite(enable1Pin, value);
#else
  ledcWrite(pwmChannel, value);
#endif
}

void MotorOn()
{
  dutyCycle = 205;
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  writeMotorPwm(dutyCycle);
  digitalWrite(ledPin, HIGH);
}

void MotorOff()
{
  dutyCycle = 0;
  writeMotorPwm(dutyCycle);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(ledPin, LOW);
}

// Datastream Blynk V0: Integer, nilai 0 atau 1.
BLYNK_WRITE(V0)
{
  if (param.asInt() == 1) {
    MotorOn();
    Serial.println("Motor ON dari Blynk");
  } else {
    MotorOff();
    Serial.println("Motor OFF dari Blynk");
  }
}

// Pastikan motor tetap OFF saat ESP32 tersambung/tersambung ulang.
BLYNK_CONNECTED()
{
  MotorOff();
  Blynk.virtualWrite(V0, 0);
}

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(pin_rpm, INPUT_PULLUP);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  if (!ledcAttach(enable1Pin, freq, resolution)) {
    Serial.println("Gagal mengaktifkan PWM pada pin motor");
  }
#else
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
#endif

  MotorOff();
  Serial.println("Menghubungkan ke Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();

  // Kontrol serial lama tetap tersedia: kirim '1' atau '0'.
  if (Serial.available() > 0) {
    myData = Serial.read();
    if (myData == '1') {
      MotorOn();
      Serial.println("Motor ON dari Serial");
    } else if (myData == '0') {
      MotorOff();
      Serial.println("Motor OFF dari Serial");
    }
  }
}
