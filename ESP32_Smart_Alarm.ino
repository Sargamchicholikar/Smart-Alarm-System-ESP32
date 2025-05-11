#include <WiFi.h>
#include <BluetoothSerial.h>
#include "esp_bt.h"
#include "time.h"

// Wi-Fi credentials - Update these with your network details
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";

BluetoothSerial SerialBT;
const int buzzerPin = 25;  // GPIO pin for buzzer

// Default alarm time - can be changed via Bluetooth
int alarmHour = 7;
int alarmMinute = 30;
bool alarmTriggered = false;
bool buzzerActive = false;

// Math challenge variables
int correctAnswer = 0;
int correctStreak = 0;
bool waitingForAnswer = false;

// NTP Server settings for time synchronization
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST (GMT+5:30)
const int daylightOffset_sec = 0;

// Buzzer control variables
unsigned long lastBuzzTime = 0;
bool buzzState = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize buzzer pin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Configure time using NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);
  } else {
    Serial.println("\nFailed to connect to Wi-Fi. Time functions won't work.");
  }
  
  // Start Bluetooth
  esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
  SerialBT.begin("ESP32_Alarm");
  Serial.println("Bluetooth Ready. Waiting for commands...");
}

void loop() {
  // Handle Bluetooth commands
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();
    
    if (waitingForAnswer) {
      // Process math answer
      int userAnswer = cmd.toInt();
      if (userAnswer == correctAnswer) {
        correctStreak++;
        if (correctStreak >= 2) {
          stopAlarm();
        } else {
          SerialBT.println("Correct! One more to go...");
          generateMathQuestion();
        }
      } else {
        correctStreak = 0;
        SerialBT.println("Wrong answer. Streak reset. Try again:");
        generateMathQuestion();
      }
    } else {
      // Process other commands
      cmd.toUpperCase();
      if (cmd.startsWith("SET")) {
        int colonIndex = cmd.indexOf(':');
        if (colonIndex > 4) {
          int hour = cmd.substring(4, colonIndex).toInt();
          int minute = cmd.substring(colonIndex + 1).toInt();
          if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59) {
            alarmHour = hour;
            alarmMinute = minute;
            alarmTriggered = false;
            SerialBT.printf("Alarm set to %02d:%02d\n", alarmHour, alarmMinute);
          } else {
            SerialBT.println("Invalid time format.");
          }
        } else {
          SerialBT.println("Invalid SET command format.");
        }
      } else if (cmd == "ON") {
        triggerAlarm();
      } else if (cmd == "TIME") {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
          SerialBT.printf("Time: %02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min);
        } else {
          SerialBT.println("Failed to get time.");
        }
      } else {
        SerialBT.println("Unknown command.");
      }
    }
  }
  
  // Check current time and trigger alarm if needed
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    int currentHour = timeinfo.tm_hour;
    int currentMinute = timeinfo.tm_min;
    
    if (currentHour == alarmHour && currentMinute == alarmMinute && !alarmTriggered) {
      triggerAlarm();
      alarmTriggered = true;
    }
  }
  
  // Handle buzzer patterns if alarm is active
  if (buzzerActive) {
    buzzLoop();
  }
  
  delay(500);
}

// Trigger the alarm and start the math challenge
void triggerAlarm() {
  Serial.println("Alarm Triggered!");
  SerialBT.println("Alarm Triggered! Solve 2 math problems to stop it!");
  buzzerActive = true;
  waitingForAnswer = true;
  correctStreak = 0;
  generateMathQuestion();
}

// Generate a random math problem
void generateMathQuestion() {
  int a = random(1, 10);
  int b = random(1, 10);
  int op = random(0, 3); // 0=+, 1=-, 2=*
  String question = "";
  
  switch (op) {
    case 0:
      correctAnswer = a + b;
      question = String(a) + " + " + String(b);
      break;
    case 1:
      correctAnswer = a - b;
      question = String(a) + " - " + String(b);
      break;
    case 2:
      correctAnswer = a * b;
      question = String(a) + " * " + String(b);
      break;
  }
  
  SerialBT.print("Question: ");
  SerialBT.println(question);
  Serial.print("Expected: ");
  Serial.println(correctAnswer);
}

// Control the buzzer sound pattern
void buzzLoop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBuzzTime >= 500) {
    lastBuzzTime = currentMillis;
    buzzState = !buzzState;
    digitalWrite(buzzerPin, buzzState ? HIGH : LOW);
  }
}

// Stop the alarm when challenge is complete
void stopAlarm() {
  buzzerActive = false;
  waitingForAnswer = false;
  correctStreak = 0;
  digitalWrite(buzzerPin, LOW);
  SerialBT.println("All answers correct! Alarm turned off.");
  Serial.println("Alarm cleared.");
}
