//https://esp8266-shop.com/blog/how-to-http-get-and-post-requests-with-esp8266/
//https://diyprojects.io/esp8266-web-client-tcp-ip-communication-examples-esp8266wifi-esp866httpclient/#.X4siVnUzaV4
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
unsigned long gts_timestamp = 1; // En secondes
const char* gts_gps_lat = "48.44484"; // A voir pour passer en  double 64 bits
const char* gts_gps_lon = "-4.46653"; // A voir pour passer en  double 64 bits
long gts_gps_elev = 150;  // En metres
const char* gts_labels = "lieu=maison";
int  gts_value = 10;

void setup()
{ 
  Serial.begin(115200);
  Serial.println();
 
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
  // Test de connectivite
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

  // Post HTTP
  HTTPClient http;                                                                                  // Creation objet http
  Serial.println("POST GTS to warp10 : ");
  Serial.println("    url = http://" WARP10_SERVER_IP ":" + String(WARP10_SERVER_PORT) + WARP10_UPDATE_URL);
  http.begin("http://" WARP10_SERVER_IP ":" + String(WARP10_SERVER_PORT) + WARP10_UPDATE_URL);
  http.addHeader("X-Warp10-Token", WARP10_WRITE_TOKEN);                                             // Add Warp10 Write Header Token
  Serial.println(gts_gps_lat);
  Serial.println("    data = " + String(gts_timestamp) + "000000/" + gts_gps_lat + ":" + gts_gps_lon + "/" + String(gts_gps_elev) + "000 " WARP10_CLASS_NAME "{" + gts_labels + "} " + String(gts_value));
  int httpCode = http.POST(String(gts_timestamp) + "000000/" + gts_gps_lat + ":" + gts_gps_lon + "/" + String(gts_gps_elev) + "000 " WARP10_CLASS_NAME "{" + gts_labels + "} " + String(gts_value));     // On poste les GTS et on recupere le code http
  Serial.printf("Http_return_code = %d \n", httpCode);                                              // Print HTTP Code
  if (httpCode > 0) {
    Serial.println("Warp10 server reached.");
    if (httpCode != 200) {
      Serial.println("Http reply : ");
      http.writeToStream(&Serial);                                                                  // On dump le message http si KO
    }
  }
  else
  {
    Serial.println("Failed to reach Warp10 server ... ");
  }
  http.end(); // Close connection

  // On glande (Deep Sleep si batterie)
  Serial.println("Waiting ...");
  delay(5000);
}
