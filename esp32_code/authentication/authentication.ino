#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

// WiFi Credentials
const char* ssid = "izhaan";
const char* password = "12345678";
const char* serverUrl = "http://192.168.137.195:5000/auth";

// RFID Pins
#define SS_PIN 5
#define RST_PIN 4

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define SDA_PIN 26
#define SCL_PIN 27

// Relay Pin
#define RELAY_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// State variables
bool IS_ON = false;
String lastRFID = "";
unsigned long rideStartTime = 0;
unsigned long rideEndTime = 0;
unsigned long lastTimerUpdate = 0;

// Format milliseconds to MM:SS
String formatDuration(unsigned long durationMs) {
  unsigned long totalSeconds = durationMs / 1000;
  unsigned int minutes = totalSeconds / 60;
  unsigned int seconds = totalSeconds % 60;
  char buffer[10];
  sprintf(buffer, "%02u:%02u", minutes, seconds);
  return String(buffer);
}

void connectWiFi() {
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
  } else {
    Serial.println("\nWiFi Connection Failed!");
  }
}

void displayMessage(String message) {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(message);
  display.display();
}

void displayRunningTimer(unsigned long elapsedMs) {
  String elapsed = formatDuration(elapsedMs);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.println("Ride in Progress");
  display.setCursor(0, 30);
  display.println("Time: " + elapsed);
  display.display();
}

void displayWelcomeMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("Welcome to");
  display.setCursor(0, 40);
  display.println("Prayana Electric");
  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED Initialization Failed");
    while (1);
  }

  display.setRotation(2);  // Flip screen upside down
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  SPI.begin();
  mfrc522.PCD_Init();

  connectWiFi();

  // Display the welcome message initially
  displayWelcomeMessage();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Handle RFID scan
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String rfidTag = "";
    Serial.print("\nScanned RFID HEX: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.printf("%02X", mfrc522.uid.uidByte[i]);
      rfidTag += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    if (IS_ON && rfidTag != lastRFID) {
      Serial.println("System is ON. New RFID ignored.");
      return;
    }

    // Stop ride if same RFID scanned again
    if (IS_ON && rfidTag == lastRFID) {
      digitalWrite(RELAY_PIN, LOW);
      IS_ON = false;
      rideEndTime = millis();
      unsigned long duration = rideEndTime - rideStartTime;
      String durationStr = formatDuration(duration);

      display.clearDisplay();
      display.setCursor(0, 10);
      display.setTextSize(1);
      display.println("Ride Ended");
      display.setCursor(0, 30);
      display.println("Duration: " + durationStr);
      display.display();

      Serial.println("Access Ended, Ride Over, Relay OFF");
      Serial.println("Duration: " + durationStr);

      delay(10000); // Let user see end screen before clearing
      displayWelcomeMessage(); // Return to initial message
      return;
    }

    // Authenticate with server
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<100> jsonDoc;
    jsonDoc["token"] = rfidTag;
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: " + response);

      if (httpResponseCode == 201) {
        digitalWrite(RELAY_PIN, HIGH);
        IS_ON = true;
        lastRFID = rfidTag;
        rideStartTime = millis();
        lastTimerUpdate = 0;
        displayMessage("Ride Started");
        Serial.println("Access Granted, Timer Started");
        delay(2000);
      } else {
        digitalWrite(RELAY_PIN, LOW);
        displayMessage("Access Denied");
        Serial.println("Access Denied");
        delay(2000);
        display.clearDisplay();
      }
    } else {
      digitalWrite(RELAY_PIN, LOW);
      Serial.print("HTTP Request Failed, Code: ");
      Serial.println(httpResponseCode);
      displayMessage("Server Error");
      delay(2000);
      display.clearDisplay();
    }

    http.end();
  }

  // If ride is active, update the timer every 500ms
  if (IS_ON && millis() - lastTimerUpdate >= 500) {
    unsigned long elapsed = millis() - rideStartTime;
    displayRunningTimer(elapsed);
    lastTimerUpdate = millis();
  }
}
