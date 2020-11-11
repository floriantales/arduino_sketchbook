#include "WebSocketClient.h" // Include from https://github.com/floriantales/esp8266-websocketclient
#include "ESP8266WiFi.h"

// Wifi config
#define WIFI_HOSTNAME "Warp10_Demo"
const char* ssid = "*****";
const char* password = "*****";

// Websocket class
WebSocketClient warp10(false);	// Set true for wss

void setup() {
	Serial.begin(115200);
	Serial.println();
	WiFi.begin(ssid, password);

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

void loop() {
  // SENDING NOOP TO WARP10
	if (!warp10.isConnected()) {
		Serial.println("Connecting to Warp10 websocket ..");
		warp10.connect("192.168.0.11", "/api/v0/streamupdate", 80);
	} else {
		Serial.println("Sending NOOP to websocket ..");
		warp10.send("NOOP");

		String msg;
		if (warp10.getMessage(msg)) {
			Serial.printf("WS Response : %s \n", msg.c_str());
		}
	}

  // WAIT BEFORE RESTART LOOP (Deep Sleep si batterie)
  Serial.println("Waiting ...");
  delay(5000);
}
