// REMOTE TELNET DEBUG + OTA Demo
// Remote Debug : https://github.com/JoaoLopesF/RemoteDebug
//
#include <ESP8266WiFi.h>

// OTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Uncomment to disable Remote Debug (port tcp telnet 23)
//#define DEBUG_DISABLED
// Uncomment to disable RemoteDebugApp WebSocket (port tcp http 8232)
//#define WEBSOCKET_DISABLED true
// Include Remote Debug
#include "RemoteDebug.h"

// Wifi config
#define WIFI_HOSTNAME "OTA_Debug_Demo"
const char* ssid = "*****"; // SSID
const char* password = "*****"; // mode de passe

// Instanciation Remote Debug
#ifndef DEBUG_DISABLED
RemoteDebug Debug;
#endif

// Time .... on arrête les delay ok?
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;

void setup()
{
  Serial.begin(115200);

  // Wifi
  WifiConnection();

  // OTA
  ArduinoOTA.setHostname(WIFI_HOSTNAME); // on donne une petit nom a notre module
  ArduinoOTA.begin(); // initialisation de l'OTA

  // Initialize RemoteDebug - or NOT
  #ifndef DEBUG_DISABLED
  
  Debug.begin(WIFI_HOSTNAME); // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
  Debug.showColors(true); // Colors

  // Project commands
  String specialCmd = "cmd1 - 1ere tache perso\n";
  specialCmd.concat("cmd2 - 2eme tache perso\n");
  specialCmd.concat("WifiInfo - Display Wifi Info");
  //specialCmd.concat("cmd3 - 3eme tache perso");
  // etc ..
  Debug.setHelpProjectsCmds(specialCmd);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
 
  #endif
}

void loop()
{
  // OTA
  ArduinoOTA.handle(); // a chaque iteration, on verifie si une mise a jour nous est envoyee

  // Toutes les secondes on lance les actions
  if ((millis() - mLastTime) >= 1000)
  {
    mLastTime = millis();
    mTimeSeconds++;

    // Action 1 : Each 5 seconds
    if (mTimeSeconds % 5 == 0) // Each 5 seconds
    {
      task_1();
    }

    // Action 2 : etc ..
    //if (mTimeSeconds % 5 == 0) // Each 5 seconds
    //{
    //  task_2();
    //}
  }

  #ifndef DEBUG_DISABLED
  // RemoteDebug handle
  Debug.handle();
  #endif
  
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
  
void task_1()
{

// Example of use:
//
//#ifndef DEBUG_DISABLED
//        if (Debug.isActive(Debug.<level>)) { // <--- This is very important to reduce overheads and work of debug levels
//            Debug.printf("bla bla bla: %d %s\n", number, str);
//            Debug.println("bla bla bla");
//        }
//#endif
//
// Or short way (prefered if only one debug at time)
//    debugA("This is a any (always showed) - var %d", var);
//    debugV("This is a verbose - var %d", var);
//    debugD("This is a debug - var %d", var);
//    debugI("This is a information - var %d", var);
//    debugW("This is a warning - var %d", var);

  debugA("This is a any (always showed) - Var=%d", 3);
  debugV("This is a message of debug level VERBOSE");
  debugD("This is a message of debug level DEBUG");
  debugI("This is a message of debug level INFO");
  debugW("This is a message of debug level WARNING");
  debugE("This is a message of debug level ERROR");
  
  #ifndef DEBUG_DISABLED
  if (Debug.isActive(Debug.VERBOSE)) {
    debugV("Calling a special debug task !!!");
    debug_task1();
  }
  #endif
}

#ifndef DEBUG_DISABLED
// Special tasks
void debug_task1()
{
  debugV("More debugging task for VERBOSE mode ....");
}

// Special Process commands
void processCmdRemoteDebug() {
  String lastCmd = Debug.getLastCommand();

  // Special Command 1
  if (lastCmd == "cmd1") {
    debugA("Commande numero 1");
    // des choses ...
  }
  // Special Command 2
  else if (lastCmd == "cmd2")
  {
    debugA("Commande numero 2");
    // des choses ...
  }
  // Special Command 3
  else if (lastCmd == "WifiInfo")
  {
    WifiInfo();
  }
}

void WifiInfo()
{
  if (Debug.isActive(Debug.ANY)) {
  Debug.println("***** WifiInfo - Configurations Wifi *****");
  Debug.printf("Mode: %s\n", WifiMode(WiFi.getMode()));
  Debug.printf("SSID: %s\n", WiFi.SSID().c_str());
  Debug.printf("Adresse IP: %s\n", WiFi.localIP().toString().c_str());
  Debug.printf("Adresse MAC: %s\n", WiFi.macAddress().c_str());
  Debug.printf("Masque : %s\n", WiFi.subnetMask().toString().c_str());
  Debug.printf("Passerelle : %s\n", WiFi.gatewayIP().toString().c_str());
  Debug.printf("RSSI (force du signal): %i\n", WiFi.RSSI());
  Debug.printf("Canal: %i\n", WiFi.channel());
  Debug.printf("Hostname: %s\n", WiFi.hostname().c_str());
  Debug.printf("DNS 1: %s\n", WiFi.dnsIP().toString().c_str());
  Debug.printf("DNS 2: %s\n", WiFi.dnsIP(1).toString().c_str());
  }
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
#endif
