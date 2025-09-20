/* ----------------------------------------------
ESP32 Based Digital Clock with 
Set Hour, Minute & Alarm feature using Web-Server
---------------------------------------------- */

#include <WiFi.h>
#include <WebServer.h>
#include <TM1637Display.h>
#include "time.h"

/* ---- According to Adafruit HUZZAH32 - ESP32 Feather Pin Setup ---- */

// --- TM1637 connections ---
#define CLK  18
#define DIO  19
TM1637Display seg7display(CLK, DIO);

// --- Alarm Pin ---
#define ALARM_PIN 16

/* ------------------------------------------------------------------ */

// --- WiFi ---
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

// --- Web server ---
WebServer server(80);

// --- NTP ---
const char* ntpServer = "pool.ntp.org";

// Adjust to your timezone
const long gmtOffset_sec = 21600;   // Bangladesh = GMT+6:00
const int daylightOffset_sec = 0;

// --- Time ---
int mHour = 0, mMinute = 0, mSecond = 0;
int curTimeInt = 0;
long preMillis = 0;
uint8_t blink_status = 0x40; 

// --- Alarm variables (user will set) ---
int alarmHour = 0;
int alarmMinute = 0;
bool alarmEnabled = false;
bool alarmTriggered = false;

// --- HTML Page ---
String htmlPage() {
  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<title>ESP32 Alarm Clock</title></head><body style='font-family:sans-serif;text-align:center;'>";
  page += "<h2>ESP32 Clock</h2>";

  page += "<p><b>Current Time:</b> " + String(mHour) + ":" + (mMinute < 10 ? "0" : "") + String(mMinute) + "</p>";

  // Alarm set form
  page += "<h3>Set Alarm</h3>";
  page += "<form action='/setAlarm' method='GET'>";
  page += "Hour: <input type='number' name='ahour' min='0' max='23' value='" + String(alarmHour) + "'><br><br>";
  page += "Minute: <input type='number' name='aminute' min='0' max='59' value='" + String(alarmMinute) + "'><br><br>";
  page += "Enable: <input type='checkbox' name='enable' " + String(alarmEnabled ? "checked" : "") + "><br><br>";
  page += "<input type='submit' value='Save Alarm'>";
  page += "</form>";

  page += "<p><b>Alarm Status:</b> " + (alarmEnabled ? "ON" : "OFF") +
          " at " + String(alarmHour) + ":" + (alarmMinute < 10 ? "0" : "") + String(alarmMinute) + "</p>";

  page += "</body></html>";
  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleSetAlarm() {
  if (server.hasArg("ahour") && server.hasArg("aminute")) {
    alarmHour = server.arg("ahour").toInt();
    alarmMinute = server.arg("aminute").toInt();
    alarmEnabled = server.hasArg("enable");
    alarmTriggered = false; // reset trigger
    Serial.printf("Alarm set: %02d:%02d, Enabled: %s\n", 
                  alarmHour, alarmMinute, alarmEnabled ? "YES" : "NO");
  }
  server.send(200, "text/html", htmlPage());
}

// --- Update current time from ESP32 RTC ---
void updateLocalTime() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    mHour = timeinfo.tm_hour;
    mMinute = timeinfo.tm_min;
  } else {
    Serial.println("Failed to obtain time from NTP");
  }
}

// --- Main Setup Code ---
void setup() {
  Serial.begin(115200);
  seg7display.setBrightness(0x0f);

  // Alarm 
  pinMode(ALARM_PIN, OUTPUT);
  digitalWrite(ALARM_PIN, LOW);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(2000);
  updateLocalTime();

  curTimeInt = (mHour * 100) + mMinute;
  seg7display.showNumberDecEx(curTimeInt, 0x40, true, 4, 0);

  // Start server
  server.on("/", handleRoot);
  server.on("/setAlarm", handleSetAlarm);
  server.begin();
}

void loop() {
  server.handleClient();

  // Update time every second
  static unsigned long lastUpdate = millis();
  if (millis() - lastUpdate >= 1000) {
    lastUpdate += 1000;
    updateLocalTime();
  }

  // --- Alarm check ---
  if (alarmEnabled && !alarmTriggered) {
    if (mHour == alarmHour && mMinute == alarmMinute && mSecond == 0) {
      alarmTriggered = true;
      Serial.println("ALARM!!! It's time.");

      digitalWrite(ALARM_PIN, HIGH);
    }
  }

  // Reset alarm 
  if (mMinute != alarmMinute) {
    alarmTriggered = false;
    digitalWrite(ALARM_PIN, LOW); // stop alarm automatically after 1 min
  }

  // Update display every 500ms (blink colon)
  if ((millis() - preMillis) > 499) {
    preMillis = millis();
    blink_status = (blink_status == 0) ? 0x40 : 0;
    curTimeInt = (mHour * 100) + mMinute;
    seg7display.showNumberDecEx(curTimeInt, blink_status, true, 4, 0);
  }
}




