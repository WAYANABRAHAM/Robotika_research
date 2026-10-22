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
#include <ArduinoOTA.h>

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

  // Konfigurasi Arduino OTA
  ArduinoOTA.setHostname("myRobot-ESP32");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // Matikan motor demi keamanan saat firmware diperbarui
    MotorOff();
    Serial.println("Mulai update OTA: " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Selesai!");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("OTA Siap digunakan!");
}

void loop()
{
  Blynk.run();
  ArduinoOTA.handle();

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
