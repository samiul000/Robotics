/* To check receiver MAC Address */

#include "WiFi.h"

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress());
}

void loop()
{
}