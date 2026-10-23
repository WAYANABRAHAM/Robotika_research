#include <esp_arduino_version.h>

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t MOTOR_IN1_PIN = 27;
constexpr uint8_t MOTOR_IN2_PIN = 26;
constexpr uint8_t MOTOR_PWM_PIN = 12;
constexpr uint8_t PWM_CHANNEL = 0;  // ESP32 Arduino core 2.x
constexpr uint32_t PWM_FREQUENCY_HZ = 30000;
constexpr uint8_t PWM_RESOLUTION_BITS = 8;
constexpr uint8_t MOTOR_ON_DUTY = 205;

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
  writeMotorPwm(MOTOR_ON_DUTY);
  digitalWrite(LED_PIN, HIGH);
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
  Serial.println("Serial motor control ready: send 1=ON, 0=OFF.");
}

void loop()
{
  while (Serial.available() > 0) {
    const char command = static_cast<char>(Serial.read());
    if (command == '1') {
      motorOn();
      Serial.println("Motor ON");
    } else if (command == '0') {
      motorOff();
      Serial.println("Motor OFF");
    }
    // Ignore CR/LF and any other characters.
  }
}
