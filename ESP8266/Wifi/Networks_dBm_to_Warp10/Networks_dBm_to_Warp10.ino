//https://github.com/esp8266/Arduino/issues/1390
//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/examples/PostHttpClient/PostHttpClient.ino

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "*****";
const char* password = "*****";

#define WARP10_SERVER_IP "192.168.0.11"
#define WARP10_SERVER_PORT 80
#define WARP10_UPDATE_URL "/api/v0/update"
#define WARP10_WRITE_TOKEN "AP_8QdbvhyjFJuuOoohNyHJClJd7ODr.vP5GMt.Y6irthsyFdeaZt_vx2CeCrQfpF465ADT1RKD5e488pteN2MhfVomQbEHAPX8Ra3foeYo"
#define WARP10_CLASS_NAME "GTS_demo"

// Pour la demo
unsigned long gts_timestamp_s = 0; // seconds -> zero will let warp10 server fixe the timestamp for you
String gts_timestamp_us = "";    // nothing to send to warp10
String gts_gps_lat = "48.44484"; // A voir pour passer en  double 64 bits
String gts_gps_lon = "-4.46653"; // A voir pour passer en  double 64 bits
long gts_gps_elev_m = 150;  // meters
String gts_labels = "lieu=maison";
int  gts_value = 0;
String gts_post_string = "";

void setup()
{ 
  //Serial.begin(115200);
  //Serial.println();
 
  //Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
  }

  //Serial.println("");
  //Serial.printf("Connected to %s, IP adress: %s \n", ssid, WiFi.localIP().toString().c_str());
}

void loop()
{

  //Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    //Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  //Serial.print("number of available networks:");
  //Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
  //Serial.println(WiFi.SSID(thisNet));
  //Serial.println(EncryptionType(WiFi.encryptionType(thisNet)));
    gts_labels = "id=wemos3,ssid=" + String(WiFi.SSID(thisNet)) + ",encryption=" + EncryptionType(WiFi.encryptionType(thisNet));
    gts_value = WiFi.RSSI(thisNet);  //  GTS Value
  
  // Ecriture des donnees
  if (gts_timestamp_s != 0) // GTS Timestamp
  {
    String gts_timestamp_us = String(gts_timestamp_s) + "000000";
  }
  gts_post_string = gts_timestamp_us + "/" + gts_gps_lat + ":" + gts_gps_lon + "/" + String(gts_gps_elev_m) + "000 " WARP10_CLASS_NAME "{" + gts_labels + "} " + String(gts_value); //  GTS Post String
  
  // Test de connectivite
  //Serial.print("Connectivity test : ");
  WiFiClient client; // Creation object client
  if (!client.connect(WARP10_SERVER_IP, WARP10_SERVER_PORT))  // On test
  {
      //Serial.println("connection failed !");
      delay(1000);                                                                                  // Sleep ou un DeepSleep pour batterie
      return;
  }
  //Serial.println("Pass");
  client.stop();

  // Post HTTP
  HTTPClient http;                                                                                  // Create http object
  //Serial.println("POST GTS to warp10 : ");
  //Serial.println("    url     = http://" WARP10_SERVER_IP ":" + String(WARP10_SERVER_PORT) + WARP10_UPDATE_URL);
  http.begin("http://" WARP10_SERVER_IP ":" + String(WARP10_SERVER_PORT) + WARP10_UPDATE_URL);
  //Serial.println("    header  = X-Warp10-Token: " + String(WARP10_WRITE_TOKEN));
  http.addHeader("X-Warp10-Token", WARP10_WRITE_TOKEN);                                             // Add Warp10 Write Header Token
  //Serial.println("    data    = " + gts_post_string);
  int httpCode = http.POST(gts_post_string);                                                        // Post GTS and catch http code
  //Serial.printf("Http_return_code = %d \n", httpCode);                                              // Print HTTP Code
  if (httpCode > 0) {                                                                               // if > 0 tcp layer ok
    //Serial.println("Data posted to Warp10.");
    if (httpCode != 200) {
      //Serial.println("Http reply : ");
      http.writeToStream(&Serial);                                                                  // http dump if http code != 200
    }
  }
  else
  {
    //Serial.println("Failed to reach Warp10 server.");
  }
  http.end(); // Close connection

  }
  
  // On glande (Deep Sleep si batterie)
  //Serial.println("Waiting ...");
  delay(30000);
}

String EncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      return "WEP";
    case ENC_TYPE_TKIP:
      return "WPA";
    case ENC_TYPE_CCMP:
      return "WPA2";
    case ENC_TYPE_NONE:
      return "None";
    case ENC_TYPE_AUTO:
      return "Auto";
    default:
      return "Unknow";
  }
}
