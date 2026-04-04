#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#define PCF8575_ADDR 0x20
const char* ssid = "SSID";
const char* password = "KLUCZ";
const char* serverUrl = "http://gcygan.webd.pl/ob/ekran.php";

void writePCF8575(uint16_t value) {
  Wire.beginTransmission(PCF8575_ADDR);
  Wire.write(value & 0xFF);        // mlodszy bajt
  Wire.write((value >> 8) & 0xFF); // starszy bajt  
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  WiFi.begin(ssid, password);
  Serial.print("Lczenie z WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPolaczono!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpCode = http.GET();    
    if (httpCode == HTTP_CODE_OK) {
      String s = http.getString();
      uint16_t value = (uint16_t) s.toInt();
      Serial.println(value);
      writePCF8575(~value);
    } else {
      Serial.print("Blad HTTP: ");
      Serial.println(httpCode);
    }
    http.end();
  } else {
    Serial.println("Brak WiFi");
  }
  delay(2000);
}
