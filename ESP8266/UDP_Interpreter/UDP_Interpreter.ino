#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
const char* ssid = "****";
const char* password = "****";
 
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
 
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
  Serial.println(" connected");
 
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
 
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off
}
 
void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len> 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
 
    // Conversion d'un array en string pour faire des comparaisons
    String udp_packet_string((char*)incomingPacket);
 
    if (udp_packet_string == "ON" ) digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on
    else if (udp_packet_string == "OFF" ) digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off
    else if (udp_packet_string == "Hi" ) Serial.printf("Hi There? How are you?\n");
    else Serial.printf("Commande non reconnue ...\n");
 
  }
}
