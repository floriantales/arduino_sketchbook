// Source : https://www.fais-le-toi-meme.fr/fr/electronique/tutoriel/esp8266-arduinoota-mise-a-jour-logiciel-esp8266-wifi
// includes necessaires au fonctionnement de l'OTA :
//  * ajouter #include <WiFiUdp.h> et <ArduinoOTA.h> aux includes
//  * ajouter ArduinoOTA.setHostname("..."); et ArduinoOTA.begin(); à la fin de la fonction setup()
//  * ajouter ArduinoOTA.handle(); dans la fonction loop()
#include <ESP8266WiFi.h>

// OTA Includes
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Wifi config
#define WIFI_HOSTNAME "OTA_Demo"
const char* ssid = "*****"; // SSID
const char* password = "*****"; // mode de passe

void setup()
{
  Serial.begin(115200);

  // Wifi
  WifiConnection();

  // OTA
  ArduinoOTA.setHostname(WIFI_HOSTNAME); // on donne une petit nom a notre module
  ArduinoOTA.begin(); // initialisation de l'OTA
}

void loop()
{
  ArduinoOTA.handle(); // a chaque iteration, on verifie si une mise a jour nous est envoyee
  
  Serial.println("On attend tranquillement une nouvelle maj ...");
  delay(4000);
}

void WifiConnection()
{
  WiFi.hostname(WIFI_HOSTNAME);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.printf("Connected to %s, IP adress: %s \n", ssid, WiFi.localIP().toString().c_str()); 
}
  



