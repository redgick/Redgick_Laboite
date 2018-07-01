/*
   Firmware for the connected display "laboite maker" by Redgick.
   Create an account on "https://lenuage.io" and get an api key.
*/

// Two modules MAX7219 4 in 1 connected in series to get 32×16 LED dotmatrix
#include <Redgick_MatrixMAX72XX.h>
Redgick_MatrixMAX72XX matrix;

// We need a WiFi connection
#include <ESP8266WiFi.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager

// laboite library !
#include <Redgick_Laboite.h>

// Welcome screen
Screen welcome(32, 16, MONOCHROME); // with, height, colors);

// Backend connection
Lenuage lenuage(
  "http://lenuage.io",                      // server url
  "00000000-0000-0000-0000-000000000000",   // api key
  // the server ssl fingerprint is needed for a HTTPS connection
  // get it using the following command in a terminal:
  // echo | openssl s_client -connect lenuage.io:443 |& openssl x509 -fingerprint -noout
  "AD:4D:92:32:D3:D2:5A:C4:5B:62:B6:B4:94:BF:10:94:4F:71:A5:5A"
);

void setup() {
  // Serial initialization for debugging
  Serial.begin(9600);

  // Display initialization
  matrix.init();
  matrix.intensity(10);

  // Welcome message
  welcome.print(0, 2, "la");
  welcome.print(1, 9, "boite");
  matrix.display(welcome.getBuffer());

  // Connecting to a wifi network using WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("laboite");

  // We get a connection !
  welcome.print(27, 0, "c");
  matrix.display(welcome.getBuffer());

  // First tiles buffer update
  lenuage.updateBuffer();
}

int currentId = lenuage.getNextTile(0);
void loop() {
  // Current tile displaying
  matrix.intensity(lenuage.getTile(currentId).getBrightness());
  matrix.display(lenuage.getTile(currentId).getScreen().getBuffer());

  // Top !
  unsigned long top = millis();

  if(currentId == lenuage.getNextTile(0)) {
    // Tiles buffer update
    lenuage.updateBuffer();
  
    // Debugging :
    lenuage.printBuffer();
  }
  
  // We wait before displaying the next tile
  unsigned int duration = lenuage.getTile(currentId).getDuration();
  while (millis() < top + duration)
    delay(50);

  // Which is the next tile ?
  currentId = lenuage.getNextTile(currentId);
}
