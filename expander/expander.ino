#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#define PCF8575_ADDR 0x20
const char* ssid = "TWOJE_SSID";
const char* password = "TWOJE_HASLO";
const char* serverUrl = "http://gcygan.webd.pl/ob/ekran.php";
void writePCF8575(uint16_t value) {
  Wire.beginTransmission(PCF8575_ADDR);
  Wire.write(value & 0xFF);        // mlodszy bajt
  Wire.write((value >> 8) & 0xFF); // starszy bajt  
  Wire.endTransmission();
}
void setup() {
  Serial.begin(115200);
  Wire.begin(); // I2C start
  WiFi.begin(ssid, password);
  Serial.print("£¹czenie z WiFi");
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
    
    if (httpCode > 0) {
      String payload = http.getString();
      uint16_t value = (uint16_t) payload.toInt();
      writePCF8575(value);
    } else {
      Serial.print("Blad HTTP: ");
      Serial.println(httpCode);
    }
    http.end();
  } else {
    Serial.println("Brak WiFi");
  }
  delay(2000); // odswiezanie co 2 sekundy
}
