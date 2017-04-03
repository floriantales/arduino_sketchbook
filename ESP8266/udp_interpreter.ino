#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
const char* ssid = "freebox";
const char* password = "maison de clairon et tata";
 
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
 
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
 
  pinMode(D1, OUTPUT );
  digitalWrite(D1, LOW);
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
 
    if (udp_packet_string == "ON" ) digitalWrite(D1, HIGH);
    else if (udp_packet_string == "OFF" ) digitalWrite(D1, LOW);
    else Serial.printf("Commande non reconnue ...\n");
 
  }
}
