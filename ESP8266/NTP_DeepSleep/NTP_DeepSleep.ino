// Pour activer le deep-sleep : connecter la broche RST à D0
// Source https://projetsdiy.fr/esp8266-activer-mode-deep-sleep-wakeup-pir/
// NTP Client : https://github.com/arduino-libraries/NTPClient

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define NB_TRYWIFI        10    // Number of try to connect to WiFi network

const char* ssid = "*****";
const char* password = "*****";

WiFiClient espClient;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Startup reason: ");Serial.println(ESP.getResetReason());

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi .");
  int _try = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    _try++;
    if ( _try >= NB_TRYWIFI ) {
        Serial.println("\nImpossible to connect WiFi network, go to deep sleep");
        ESP.deepSleep(10e6);
    }
  }
  Serial.println("\nConnected to the WiFi network");
  
  // Démarrage du client NTP - Start NTP client
  timeClient.begin();

  // NTP Update & Display
  timeClient.update();
  Serial.print("Formatted Time: ");Serial.println(timeClient.getFormattedTime());
  // Dodo
  Serial.println("Deep Sleep ...");
  ESP.deepSleep(5e6);
}

void loop() {
}

