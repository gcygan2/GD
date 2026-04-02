#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#define PCF8575_ADDR 0x20
const char* ssid = "TWOJE_SSID";
const char* password = "TWOJE_HASLO";
const char* serverUrl = "http://gcygan.webd.pl/ob/ekran.php";
void writePCF8575(uint16_t value) {
  Wire.beginTransmission(PCF8575_ADDR);
  Wire.write(value & 0xFF);        // m³odszy bajt
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
  Serial.println("\nPo³¹czono!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.print("Odebrano: ");
      Serial.println(payload);
      uint16_t value = (uint16_t) payload.toInt();
      Serial.print("Wartoœæ: ");
      Serial.println(value);
      writePCF8575(value);
    } else {
      Serial.print("B³¹d HTTP: ");
      Serial.println(httpCode);
    }
    http.end();
  } else {
    Serial.println("Brak WiFi");
  }
  delay(2000); // odœwie¿anie co 2 sekundy
}
