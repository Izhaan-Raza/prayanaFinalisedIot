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

// RFID Pins (for ESP32)
#define SS_PIN 21      // RFID SDA
#define RST_PIN 22     // RFID RST

// OLED Display (I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define SDA_PIN 25    // Changed from 18
#define SCL_PIN 26    // Changed from 19

// Relay Pin
#define RELAY_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// State variables
bool IS_ON = false;
String lastRFID = "";

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
  display.println(message);
  display.display();
}

void setup() {
  Serial.begin(115200);

  // Start I2C before OLED with custom SDA and SCL pins
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED Initialization Failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  SPI.begin(); // Uses default SPI pins
  mfrc522.PCD_Init();

  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

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

    if (IS_ON && rfidTag == lastRFID) {
      digitalWrite(RELAY_PIN, LOW);
      IS_ON = false;
      displayMessage("Access Ended");
      Serial.println("Access Ended, Relay OFF");
      delay(2000);
      return;
    }

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
        displayMessage("Access Granted");
        Serial.println("Access Granted");
      } else {
        digitalWrite(RELAY_PIN, LOW);
        displayMessage("Access Denied");
        Serial.println("Access Denied");
      }
    } else {
      digitalWrite(RELAY_PIN, LOW);
      Serial.print("HTTP Request Failed, Code: ");
      Serial.println(httpResponseCode);
      displayMessage("Server Error");
    }

    http.end();
    delay(2000);
  }
}