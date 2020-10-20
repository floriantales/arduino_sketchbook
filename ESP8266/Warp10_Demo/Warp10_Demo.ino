//https://github.com/esp8266/Arduino/issues/1390
//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/examples/PostHttpClient/PostHttpClient.ino

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Wifi config
#define WIFI_HOSTNAME "Warp10_Demo"
const char* ssid = "*****";
const char* password = "*****";

// Warp10 config
#define WARP10_SERVER_IP "192.168.0.11"
#define WARP10_SERVER_PORT 80
#define WARP10_UPDATE_URL "/api/v0/update"
#define WARP10_WRITE_TOKEN "AP_8QdbvhyjFJuuOoohNyHJClJd7ODr.vP5GMt.Y6irthsyFdeaZt_vx2CeCrQfpF465ADT1RKD5e488pteN2MhfVomQbEHAPX8Ra3foeYo"
#define WARP10_CLASS_NAME "GTS_Titi"

void setup()
{ 
  Serial.begin(115200);
  delay(10);
  Serial.println();

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

void loop()
{
  // ALIMENTATION GTS
  //unsigned long gts_timestamp_s = 0;  // epoch in seconds -> zero will let warp10 server fixe the timestamp for you
  //String gts_gps_lat = "48.44484";    // A voir pour passer en  double 64 bits // xx let warp10 server fixe no LAT/LON values
  //String gts_gps_lon = "-4.46653";    // A voir pour passer en  double 64 bits
  //int gts_gps_elev_m = -1;            // meters -> -1 will let warp10 server fixe no elevation value
  //String gts_labels = "id=wemos1";
  //float gts_value = 0;

  // POST to Warp10
  Warp10_update(0, "xx", "-xx", -1, "id=wemos1", 0);
  
  // WAIT BEFORE RESTART LOOP (Deep Sleep si batterie)
  Serial.println("Waiting ...");
  delay(5000);
}

void Warp10_update(unsigned long gts_timestamp_s, String gts_gps_lat, String gts_gps_lon, int gts_gps_elev_m, String gts_labels, float gts_value)
{
  // FORMATTAGE STRING POST
  String gts_post_string = "";
  // Timestamp
  String gts_timestamp_us = "";    // default : nothing to send to warp10
  if (gts_timestamp_s != 0) // GTS Timestamp
  {
    gts_timestamp_us = String(gts_timestamp_s) + "000000";
  }
  // GPS Elevation
  String gts_gps_elev_mm = "";    // default : nothing to send to warp10
  if (gts_gps_elev_m != -1) // GTS Timestamp
  {
    gts_gps_elev_mm = String(gts_gps_elev_m) + "000";
  }
  // GPS lat/lon
  String gts_gps_coordinates = "";    // default : nothing to send to warp10
  if (gts_gps_lat != "xx") // GTS Timestamp
  {
    gts_gps_coordinates = gts_gps_lat + ":" + gts_gps_lon;
  }
  // Write POST string
  gts_post_string = gts_timestamp_us + "/" + gts_gps_coordinates + "/" + gts_gps_elev_mm + " " + WARP10_CLASS_NAME "{" + gts_labels + "} " + String(gts_value); //  GTS Post String
  Serial.println("    data    = " + gts_post_string);

  
  // WARP10 SERVER CONNECTIVITY TEST
  Serial.print("Connectivity test : ");
  WiFiClient client; // Creation object client
  if (!client.connect(WARP10_SERVER_IP, WARP10_SERVER_PORT))  // On test
  {
      Serial.println("connection failed !");
      delay(1000);                                                                                  // Sleep ou un DeepSleep pour batterie
      return;
  }
  Serial.println("Pass");
  client.stop();


  // HTTP POST to WARP10 SERVER
  HTTPClient http;                                                                                  // Create http object
  Serial.println("POST GTS to warp10 : ");
  Serial.println("    url     = http://" WARP10_SERVER_IP ":" + String(WARP10_SERVER_PORT) + WARP10_UPDATE_URL);
  http.begin("http://" WARP10_SERVER_IP ":" + String(WARP10_SERVER_PORT) + WARP10_UPDATE_URL);
  Serial.println("    header  = X-Warp10-Token: " + String(WARP10_WRITE_TOKEN));
  http.addHeader("X-Warp10-Token", WARP10_WRITE_TOKEN);                                             // Add Warp10 Write Header Token
  Serial.println("    data    = " + gts_post_string);
  int httpCode = http.POST(gts_post_string);                                                        // Post GTS and catch http code
  Serial.printf("Http_return_code = %d \n", httpCode);                                              // Print HTTP Code
  if (httpCode > 0) {                                                                               // if > 0 tcp layer ok
    Serial.println("Data posted to Warp10.");
    if (httpCode != 200) {
      Serial.println("Http reply : ");
      http.writeToStream(&Serial);                                                                  // http dump if http code != 200
    }
  }
  else
  {
    Serial.println("Failed to reach Warp10 server.");
  }
  http.end(); // Close connection
}
