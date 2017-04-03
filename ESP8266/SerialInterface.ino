#include "ESP8266WiFi.h"

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
char endMarker = '\r'; // Il s'agit de marqueur de fin pour minicom (quand on press Enter)

boolean newData = false;

 ADC_MODE(ADC_VCC);

void setup() {
 Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

 Serial.println("<Arduino is ready>");
}

void loop() {
 recvWithEndMarker();
 showNewData();
}

void recvWithEndMarker() {
 static byte ndx = 0;

 char rc;

 while (Serial.available()> 0 && newData == false) {
 rc = Serial.read();

  if (rc != endMarker) {
  receivedChars[ndx] = rc;
  ndx++;
  if (ndx>= numChars) {
  ndx = numChars - 1;
  }
  }
  else {
  receivedChars[ndx] = '\0'; // terminate the string
   ndx = 0;
  newData = true;
  }
 }
}

void showNewData() {
 if (newData == true) {
   Serial.printf("Commande recue : %s\n", receivedChars);

   // Conversion d'un array en string pour faire des comparaisons
   String str((char*)receivedChars);
   if (str == "help") Serial.println("- help -> this help\r\n- tension -> print esp Vcc in volt\r\n- networks -> Discover and print Wifi SSID around esp");
   else if (str == "coucou") Serial.println("Coucou grand fou!");
   else if (str == "tension") Serial.println(ESP.getVcc());
   else if (str == "networks") listNetworks();
   else Serial.println("Commande non reconnue ...");
   Serial.println("----------------------------------");
   newData = false;
 }
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
    Serial.println(".");
  }
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.print("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.print("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.print("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.print("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.print("Auto");
      break;
  }
}
