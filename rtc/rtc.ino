#include <Wire.h>
#define RTC_ADDR 0x68/*pcf8583 0x50, ds3231 0x68*/
#define RTC_SEC 0x00/*pcf8583 0x02, ds3231 0x00*/
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  delay(1000);
}
byte bcdToDec(byte val) {
  return (val / 16 * 10) + (val % 16);
}
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
  byte sec  = readSeconds(RTC_ADDR, RTC_SEC);  // Odczyt sekund
  Serial.println(sec);
  delay(500);
}