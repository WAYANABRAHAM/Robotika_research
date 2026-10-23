#include <esp_arduino_version.h>

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t MOTOR_IN1_PIN = 27;
constexpr uint8_t MOTOR_IN2_PIN = 26;
constexpr uint8_t MOTOR_PWM_PIN = 12;
constexpr uint8_t PWM_CHANNEL = 0;  // ESP32 Arduino core 2.x
constexpr uint32_t PWM_FREQUENCY_HZ = 30000;
constexpr uint8_t PWM_RESOLUTION_BITS = 8;
constexpr uint8_t PWM_STEP = 25;
constexpr uint8_t DEFAULT_DUTY = 205;

uint8_t dutyCycle = 0;

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

void setMotorDuty(int requestedDuty)
{
  if (requestedDuty < 0) requestedDuty = 0;
  if (requestedDuty > 255) requestedDuty = 255;
  dutyCycle = static_cast<uint8_t>(requestedDuty);

  if (dutyCycle == 0) {
    writeMotorPwm(0);
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(MOTOR_IN1_PIN, HIGH);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    writeMotorPwm(dutyCycle);
    digitalWrite(LED_PIN, HIGH);
  }

  Serial.printf("PWM duty = %u/255 (%.1f%%)\n", dutyCycle,
                (100.0f * dutyCycle) / 255.0f);
}

void handleCommand(char command)
{
  switch (command) {
    case '1':
      setMotorDuty(DEFAULT_DUTY);
      break;
    case '0':
      setMotorDuty(0);
      break;
    case '+':
    case 'u':
    case 'U':
      setMotorDuty(static_cast<int>(dutyCycle) + PWM_STEP);
      break;
    case '-':
    case 'd':
    case 'D':
      setMotorDuty(static_cast<int>(dutyCycle) - PWM_STEP);
      break;
    case 'p':
    case 'P':
      Serial.printf("Current PWM duty = %u/255\n", dutyCycle);
      break;
    case '\r':
    case '\n':
      break;
    default:
      Serial.println("Commands: u/+ up, d/- down, 1=205, 0=OFF, p=status");
      break;
  }
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
  setMotorDuty(0);
  Serial.println("PWM serial control ready (open-loop; no RPM feedback).");
  Serial.println("Commands: u/+ up, d/- down, 1=205, 0=OFF, p=status");
}

void loop()
{
  while (Serial.available() > 0) {
    handleCommand(static_cast<char>(Serial.read()));
  }
}
