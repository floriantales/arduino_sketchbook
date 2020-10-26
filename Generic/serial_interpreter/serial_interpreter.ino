const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
char endMarker = '\r'; // Il s'agit de marqueur de fin pour minicom (quand on press Enter)
 
boolean newData = false;
 
void setup() {
 Serial.begin(9600);
 Serial.println("<Arduino is ready>");
 pinMode(LED_BUILTIN, OUTPUT);
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
   Serial.printf("Commande reçue %s\n", receivedChars);
 
   // Conversion d'un array en string pour faire des comparaisons
   String str((char*)receivedChars);
   if (str == "test") Serial.println("It works!");
   if (str == "on") digitalWrite(LED_BUILTIN, LOW);
   if (str == "off") digitalWrite(LED_BUILTIN, HIGH);
 
   newData = false;
 }
}
