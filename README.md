# Redgick_Laboite
*laboite* is a connected clock that displays a lot of information from the Internetz.

## Requirement

 * [Redgick_GFX](https://github.com/redgick/Redgick_GFX), the core graphics library;
 * [WiFiManager](https://github.com/tzapu/WiFiManager);
 * **ESP8266HTTPClient** by Markus Sattler
 * **ArduinoJson** by Benoit Blanchon


## Installation

[Download this repository](https://github.com/redgick/Redgick_Laboite/archive/master.zip) and unzip it into the Arduino libraries folder on your computer. You should rename the folder `Redgick_Laboite-master` in `Redgick_Laboite`.

In the Arduino IDE, look at `File/Examples/Redgick_Laboite` folder to load an example sketch.

## Building _laboîte maker_

### Material

 * 1 x NodeMcu _V2_ or _V3_;
 * 2 x MAX7219 Dot Matrix Modules;
 * 1 x frame 3D printed ([download 3D model](https://github.com/redgick/Redgick_Laboite/blob/master/CAD/laboite_maker_frame_v1.9.stl?raw=true));
 * 1 x PMMA screen laser cutted ([download 2D model](https://github.com/redgick/Redgick_Laboite/blob/master/CAD/laboite_maker_screen_v1.9.svg?raw=true));
 * 1 x colson collar.

The CAD models of the frame and the screen can be edited on [Tinkercad](https://www.tinkercad.com/things/hslmXMSMxwP)

### Schematic

_laboîte maker_ as been tested with two types of NodeMCU : _V2_ and _V3_.
Both come with the chip _ESP-12E_. If you compare the pin layout of the _V3_, there’s only a tiny difference to the _V2_ layout. _V3_ use one of the two reserve pins for USB power out and the other for an additional GND.

![schematic](https://github.com/redgick/Redgick_Laboite/blob/master/images/nodemcu_v2_v3.jpg?raw=true)
_NodeMCU V2 / NodeMCU V3_


<table>
  <tr>
    <th colspan=2>NodeMCU V2</th><th colspan=2>NodeMCU V3</th><th>LED Matrix</th>
  </tr>
  <tr>
    <th>Pin</th><th>GPIO</th><th>Pin</th><th>GPIO</th><th>Pin</th>
  </tr>
  <tr>
  <td>Vin (+5V)</td><td>-</td><td>VV (+5V)</td><td>-</td><td>+12V</td>
  </tr>
  <tr>
    <td>GND</td><td>-</td><td>GND</td><td>-</td><td>GND</td>
  </tr>
  <tr>
    <td>D7</td><td>GPIO13</td><td>D7</td><td>GPIO13</td><td>DIN</td>
  </tr>
  <tr>
    <td>D8</td><td>GPIO15</td><td>D8</td><td>GPIO15</td><td>CS</td>
  </tr>
  <tr>
    <td>D5</td><td>GPIO14</td><td>D5</td><td>GPIO14</td><td>CLK</td>
  </tr>
</table>

 * MAX7219 Dot Matrix Modules should be connected together in serial. Only input pins are welded on the modules, so start by welding output pins on the upper MAX7219 Dot Matrix Module.
 * Then insert the Colson collar in the frame to fasten the NodeMCU on the rear side of the frame.
 * Insert MAX7219 Dot Matrix Modules in the frame and plug the PMMA screen on it.
 * Connect the MAX7219 Dot Matrix Modules and the NodeMCU following the schematic bellow :

![schematic](https://github.com/redgick/Redgick_Laboite/blob/master/images/laboite_bb.png?raw=true)

### lenuage.io
 * Create an account in [lenuage.io](https://lenuage.io/) and add a new _boîte_. An API Key is automatically generated.
 
### Firmware
 * Start [Arduino IDE](https://www.arduino.cc/en/Main/Software) and open `File > Preferences` window.
 * Enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into `Additional Board Manager URLs` field.
 * Open `Tools > Board: > Boards Manager...` menu and find __ESP8266 by ESP8266 Community__ and install it.
 * Select `Tools > Board > NodeMCU 1.0 (ESP-12E Module)`.
 * Open library manager from `Sketch > Include library > Manage Libraries...` and find __ArduinoJson by Benoit Blanchon__ and install the latest __version 5__ (do not install a beta version 6).
 * Then find __WifiManager by tzapu__ and install the latest version.
 * Download [Redgick_GFX libraries](https://github.com/redgick/Redgick_GFX), copy and past __Redgick-GFX__ and __Redgick_MatrixMax72XX__ into your Arduino library folder.
 * Download [Redgick-Laboite](https://github.com/redgick/Redgick_Laboite) into your Arduino lbrary folder. Remove the trailling  extension (_-master_) of the library folder name.
 * Open `File > Exemples > Redgick_laboite > laboite_maker_edition`
 * First click on `Verify/Compile`, if an error message appears, get sure that you have correctly placed the files with the right name.
 * Replace `00000000-0000-0000-0000-000000000000` with your API key
 * Connect the NodeMCU, upload and enjoy !

### WiFi
 * At the first boot, _laboîte_ create an WiFi access point named "laboite". Connect to it with your smartphone, open a navigator à http://192.168.4.1 and set the SSID and Password of you WiFi network.
 * _laboîte_ will reboot and automatically connect to the given WiFi network. That's it !

### Going further ...
Explore _lenuage.io_ interface to configure apps and have fun !
