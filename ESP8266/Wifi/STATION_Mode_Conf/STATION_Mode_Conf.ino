// Wifi mode Station - Des displays et de la configuration Wifi
// Wifi Keyword : https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/keywords.txt
// Station Class : https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-class.html?highlight=getAutoConnect#reconnect
#include <ESP8266WiFi.h>

const char* ssid = "*****";
const char* password = "*****";

void setup()
{ 
  Serial.begin(115200);
  Serial.println();

  // Parametrages Wifi
  WiFi.hostname("Station_Tester");
  WiFi.setPhyMode(WIFI_PHY_MODE_11B);   // On force le 802.11b
  WiFi.setOutputPower(20.5); // In dBm Multiple of 0.25 (max 20.5)

  // Connection
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Exemples de display
  Serial.println("");
  Serial.printf("Connected to %s, IP adress: %s, Physical Mode: %s \n", ssid, WiFi.localIP().toString().c_str(), WifiPhyMode(WiFi.getPhyMode()));

  Serial.println("Diag :");
  WiFi.printDiag(Serial);

  Serial.println("Au détail :");
  Serial.printf("Mode: %s\n", WifiMode(WiFi.getMode()));
  Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("Adresse IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Adresse MAC: %s\n", WiFi.macAddress().c_str());
  Serial.printf("Masque : %s\n", WiFi.subnetMask().toString().c_str());
  Serial.printf("Passerelle : %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.printf("RSSI (force du signal): %i\n", WiFi.RSSI());
  Serial.printf("Canal: %i\n", WiFi.channel());
  Serial.printf("Hostname: %s\n", WiFi.hostname().c_str());
  Serial.printf("DNS 1: %s\n", WiFi.dnsIP().toString().c_str());
  Serial.printf("DNS 2: %s\n", WiFi.dnsIP(1).toString().c_str());
}

void loop()
{
  Serial.println("On fit des choses ...");
  delay(10000);
}

char* WifiPhyMode(int thisPhyMode) {
  // read the encryption type and print out the name:
  switch (thisPhyMode) {
    case WIFI_PHY_MODE_11B:
      return "802.11b";
    case WIFI_PHY_MODE_11G:
      return "802.11g";
    case WIFI_PHY_MODE_11N:
      return "802.11n";
    default:
      return "Unknown";
  }
}
char* WifiMode(int thisMode) {
  // read the encryption type and print out the name:
  switch (thisMode) {
    case WIFI_AP:
      return "Access Point";
    case WIFI_STA:
      return "Station";
    case WIFI_AP_STA:
      return "Station + Access Point";
    default:
      return "Unknown";
  }
}
