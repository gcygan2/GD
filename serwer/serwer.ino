/*
Sterownik pracuje w trybie AP.
Na sterowniku działa serwer z formularzem do wpisania hasła i przyciskiem OK.
Po kiknięciu przycisku hasło jest zapisywane w pamięci flash, a serwer przechodzi w tryb klienta sieci WiFi logując się za pomocą hasła podanego w formularzu, 
o którym mowa wcześniej. SSID jest stały (w kodzie programu).
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

Preferences preferences;
WebServer server(80);

// ======== KONFIGURACJA ========
const char* AP_SSID = "serwerGD";
const char* AP_PASS = "12345678";   // min. 8 znaków
const char* STA_SSID = "TP-Link_3541";  // STAŁY SSID
// ===============================

String savedPassword = "";

// ======== STRONA HTML ========
String htmlPage() {
  return R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <title>Konfiguracja WiFi</title>
    </head>
    <body>
      <h2>Podaj hasło do sieci WiFi</h2>
      <form action="/save" method="POST">
        Hasło: <input type="password" name="password">
        <input type="submit" value="OK">
      </form>
    </body>
    </html>
  )rawliteral";
}

// ======== OBSŁUGA STRONY GŁÓWNEJ ========
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

// ======== ZAPIS HASŁA ========
void handleSave() {
  if (server.hasArg("password")) {
    savedPassword = server.arg("password");

    // Zapis do pamięci flash (NVS)
    preferences.begin("wifi", false);
    preferences.putString("pass", savedPassword);
    preferences.end();

    server.send(200, "text/html", "<h3>Haslo zapisane. Restart...</h3>");
    delay(2000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Brak hasla");
  }
}

// ======== TRYB AP ========
void startAPMode() {
  Serial.println("Start trybu AP...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  Serial.print("IP AP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
}

// ======== TRYB STA ========
void startSTAMode(String pass) {
  Serial.println("Laczenie z WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, pass.c_str());

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nPolaczono!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNie udalo sie polaczyc. Powrot do AP.");
    startAPMode();
  }
}

void setup() {
  Serial.begin(115200);

  preferences.begin("wifi", true);
  savedPassword = preferences.getString("pass", "");
  preferences.end();

  if (savedPassword.length() > 0) {
    startSTAMode(savedPassword);
  } else {
    startAPMode();
  }
}

void loop() {
  server.handleClient();
}
