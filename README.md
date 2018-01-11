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

## Building laboîte maker

### Material

 *  1 x [NodeMcu V3](https://www.aliexpress.com/item/NodeMcu-Lua-WIFI-development-board-based-on-the-ESP8266-Internet-of-things/32338121965.html)

![NodeMcu V3](https://github.com/redgick/Redgick_Laboite/blob/master/images/nodemcu.jpg?raw=true)

 * 2 x [MAX7219 Dot Matrix Module](https://www.aliexpress.com/item/MAX7219-Dot-Matrix-Module-For-Arduino-Microcontroller-4-In-One-Display-with-5P-Line/32618155357.html)

![MAX7219 Dot Matrix Module](https://github.com/redgick/Redgick_Laboite/blob/master/images/max7219.jpg?raw=true)


### Schematic

<table>
  <tr>
    <th colspan=2>NodeMCU</th><th>LED Matrix</th>
  </tr>
  <tr>
    <th>Pin</th><th>GPIO</th><th>Pin</th>
  </tr>
  <tr>
  <td>Vin (+5V) <strong>(*)</strong></td><td>-</td><td>+12V</td>
  </tr>
  <tr>
    <td>GND</td><td>-</td><td>GND</td>
  </tr>
  <tr>
    <td>D7</td><td>GPIO13</td><td>DIN</td>
  </tr>
  <tr>
    <td>D8</td><td>GPIO15</td><td>CS</td>
  </tr>
  <tr>
    <td>D5</td><td>GPIO14</td><td>CLK</td>
  </tr>
</table>

**(*)** or *3V3* for older NodeMCU boards.

![schematic](https://github.com/redgick/Redgick_Laboite/blob/master/images/laboite_bb.png?raw=true)


### lenuage
Account creation, api key, configuration, ...

### Firmware

### WiFi

### Going further ...
