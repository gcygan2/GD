#include <Wire.h>

#define PCF8583_ADDR 0x50
#define DS3231_ADDR  0x68

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  delay(1000);
}

// Funkcja konwertuj¹ca BCD na decimal
byte bcdToDec(byte val) {
  return (val / 16 * 10) + (val % 16);
}

// Funkcja odczytu sekund dla RTC
byte readSeconds(uint8_t rtc_addr, uint8_t reg_sec) {
  Wire.beginTransmission(rtc_addr);
  Wire.write(reg_sec);
  Wire.endTransmission();
  Wire.requestFrom(rtc_addr, (uint8_t)1);

  byte sec = 0;
  if (Wire.available()) sec = Wire.read() & 0x7F; // maska bit7
  return bcdToDec(sec);
}

void loop() {
  // Odczyt sekund
  byte secPCF  = readSeconds(PCF8583_ADDR, 0x02);
  byte secDS   = readSeconds(DS3231_ADDR, 0x00);

  Serial.print("PCF8583 sekundy: ");
  Serial.print(secPCF);
  Serial.print(" | DS3231 sekundy: ");
  Serial.println(secDS);

  delay(1000);
}
