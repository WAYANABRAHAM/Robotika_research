#define BLYNK_TEMPLATE_ID "TMPL60ZzIP8B9"
#define BLYNK_TEMPLATE_NAME "myRobot"
#define BLYNK_PRINT Serial

#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <esp_arduino_version.h>
#include <ArduinoOTA.h>

const char* wifiSsid = WIFI_SSID;
const char* wifiPassword = WIFI_PASSWORD;

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t MOTOR_IN1_PIN = 27;
constexpr uint8_t MOTOR_IN2_PIN = 26;
constexpr uint8_t MOTOR_PWM_PIN = 12;
constexpr uint8_t PWM_CHANNEL = 0;  // ESP32 Arduino core 2.x
constexpr uint32_t PWM_FREQUENCY_HZ = 30000;
constexpr uint8_t PWM_RESOLUTION_BITS = 8;
constexpr uint8_t MOTOR_DUTY = 205;

bool otaStarted = false;
bool blynkWasConnected = false;
unsigned long lastBlynkAttemptMs = 0;

bool initMotorPwm()
{
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  return ledcAttach(MOTOR_PWM_PIN, PWM_FREQUENCY_HZ, PWM_RESOLUTION_BITS);
#else
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY_HZ, PWM_RESOLUTION_BITS);
  ledcAttachPin(MOTOR_PWM_PIN, PWM_CHANNEL);
  return true;
#endif
}

void writeMotorPwm(uint8_t duty)
{
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(MOTOR_PWM_PIN, duty);
#else
  ledcWrite(PWM_CHANNEL, duty);
#endif
}

void motorOff()
{
  writeMotorPwm(0);
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void motorOn()
{
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  writeMotorPwm(MOTOR_DUTY);
  digitalWrite(LED_PIN, HIGH);
}

BLYNK_WRITE(V0)
{
  if (param.asInt() == 1) {
    motorOn();
    Serial.println("V0=1: Motor ON");
  } else {
    motorOff();
    Serial.println("V0=0: Motor OFF");
  }
}

BLYNK_CONNECTED()
{
  motorOff();
  Blynk.virtualWrite(V0, 0);
}

void startOta()
{
  ArduinoOTA.setHostname("imclab-esp32");
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    motorOff();  // Stop the actuator before flash begins.
    Serial.println("OTA update started; motor forced OFF.");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA update finished.");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    if (total > 0) {
      Serial.printf("OTA progress: %u%%\r", (progress * 100U) / total);
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA error[%u]\n", static_cast<unsigned int>(error));
  });

  ArduinoOTA.begin();
  otaStarted = true;
  Serial.print("ArduinoOTA ready at IP ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);

  if (!initMotorPwm()) {
    Serial.println("PWM initialization failed; motor will remain OFF.");
  }
  motorOff();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(wifiSsid, wifiPassword);
  Blynk.config(BLYNK_AUTH_TOKEN);
  Serial.println("Connecting to Wi-Fi; OTA starts when an IP is available.");
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    if (blynkWasConnected) {
      motorOff();
      blynkWasConnected = false;
    }
    delay(10);
    return;
  }

  if (!otaStarted) {
    startOta();
  }

  if (!Blynk.connected() &&
      (lastBlynkAttemptMs == 0 || millis() - lastBlynkAttemptMs >= 5000)) {
    lastBlynkAttemptMs = millis();
    Blynk.connect(500);
  }

  Blynk.run();
  const bool connectedNow = Blynk.connected();
  if (blynkWasConnected && !connectedNow) {
    motorOff();
    Serial.println("Blynk disconnected; motor forced OFF.");
  }
  blynkWasConnected = connectedNow;

  if (otaStarted) {
    ArduinoOTA.handle();
  }
}
