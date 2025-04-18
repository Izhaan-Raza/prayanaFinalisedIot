#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h> 

#define SS_PIN 5     
#define RST_PIN 4    

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define SDA_PIN 25  
#define SCL_PIN 27  

const char* ssid = "izhaan";  
const char* password = "12345678"; 
const char* serverUrl = "http://192.168.137.195:5000/token";

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
        Serial.println("\nWiFi Connection Failed! Retrying...");
        delay(5000);
    }
}

void checkWiFiReconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi Disconnected! Reconnecting...");
        WiFi.disconnect();
        WiFi.reconnect();

        int reconnectAttempts = 0;
        while (WiFi.status() != WL_CONNECTED && reconnectAttempts < 15) {
            delay(500);
            Serial.print(".");
            reconnectAttempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi Reconnected Successfully!");
        } else {
            Serial.println("\nWiFi Still Not Connected!");
        }
    }
}

void displayMessage(String line1, String line2) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println(line1);
    display.setCursor(0, 30);
    display.println(line2);
    display.display();
}

void setup() {
    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();

    Wire.begin(SDA_PIN, SCL_PIN);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("OLED Initialization Failed"));
        while (1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();

    connectWiFi();
}

void loop() {
    checkWiFiReconnect();

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String rfidTag = "";
        Serial.print("\nScanned RFID HEX: "); 

        for (byte i = 0; i < mfrc522.uid.size; i++) {
            Serial.printf("%02X", mfrc522.uid.uidByte[i]);
            rfidTag += String(mfrc522.uid.uidByte[i], HEX);
        }

        Serial.println();
        displayMessage("RFID Scanned:", rfidTag);

        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        // Prepare JSON body
        StaticJsonDocument<100> jsonDoc;
        jsonDoc["token"] = rfidTag;
        String requestBody;
        serializeJson(jsonDoc, requestBody);

        int httpResponseCode = http.POST(requestBody);

        String statusMessage = "Unknown";
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Server Response: " + response);

            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, response);

            if (!error && doc.containsKey("status")) {
                statusMessage = doc["status"].as<String>();
            }
        } else {
            Serial.print("HTTP Error: ");
            Serial.println(httpResponseCode);
            statusMessage = "Server Error";
        }

        http.end();
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("RFID: " + rfidTag);
        display.setCursor(0, 30);
        display.println("Status: " + statusMessage);
        display.display();

        Serial.println("Status: " + statusMessage);

        delay(20000); 
    }
}
