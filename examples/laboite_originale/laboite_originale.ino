/*
   Firmware for the connected display "laboite maker" by Redgick.
   Create an account on "https://lenuage.io" and get an api key.
*/

// Sure-electronics 32x16 RG Dual Color LED Dot Matrix Unit Board
#include <Redgick_MatrixHT1632C.h>
Redgick_MatrixHT1632C afficheur;

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
  Serial.begin(115200);

  // Display initialization
  afficheur.init();
  afficheur.intensity(10);

  // message d'accueil
  welcome.print(0, 2, "la");
  welcome.print(1, 9, "boite");
  afficheur.display(welcome.getBuffer());

  // we start by connecting to a WiFi network
  WiFiManager wifiManager;
  wifiManager.autoConnect("laboite");

  // We get a connection !
  welcome.print(27, 0, "c");
  afficheur.display(welcome.getBuffer());

  // First tiles buffer update
  lenuage.updateBuffer();
}

int current_id = lenuage.getNextTile(0);

void loop() {
  // Current tile displaying
  afficheur.intensity(lenuage.getTile(current_id).getBrightness());
  afficheur.display(lenuage.getTile(current_id).getScreen().getBuffer());

  // Top !
  unsigned long top = millis();

  // Tiles buffer update
  lenuage.updateBuffer();

  // Debugging :
  lenuage.printBuffer();

  // We wait before displaying the next tile
  int delai = lenuage.getTile(current_id).getDuration() * 1000;
  while (millis() < top + delai) {
    delay(50);
  }

  // Which is the next tile ?
  current_id = lenuage.getNextTile(current_id);

}
