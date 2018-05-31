/*
  Redgick_Laboite.h
*/

// ensure this library description is only included once
#ifndef Redgick_Laboite_h
#define Redgick_Laboite_h

// uncomment to activate debugging mode
//#define DEBUG 1

// include description files for other libraries used
#include "ESP8266HTTPClient.h"
#include <ArduinoJson.h>
#include <Redgick_GFX.h>

// library interface description

#define TILES_MAX_NUMBER 50

class Tile {
  // user-accessible "public" interface
  public:
    Tile();
    void clear();

    void clearScreen() { _screen.clear(); };
    void print(uint8_t x, uint8_t y, String s) { _screen.print(x, y, s); };
    void print(uint8_t x, uint8_t y, String s, uint8_t font_name, uint8_t color) { _screen.print(x, y, s, font_name, color); };
    void drawBitmap(uint8_t x, uint8_t y, Bitmap bitmap, uint8_t color) { _screen.drawBitmap(x, y, bitmap, color); };
    Screen getScreen() { return _screen; };

    uint8_t getId() {return _id;};
    void setId(uint8_t value) {_id = value;};

    uint64_t getLastActivity() {return _last_activity;};
    void setLastActivity(uint64_t value) {_last_activity = value;};

    uint8_t getPreviousId() {return _previous_id;};
    void setPreviousId(uint8_t value) {_previous_id = value;};

    uint16_t getDuration() {return _duration;};
    void setDuration(uint16_t value) {_duration = value;};

    uint8_t getBrightness() {return _brightness;};
    void setBrightness(uint8_t value) {_brightness = value;};

    bool exist() {return _exist;};
    void exist(bool value) {_exist = value;};

    bool needUpdate() {return _update;};
    void needUpdate(bool value) {_update = value;};


  // library-accessible "private" interface
  private:
    uint8_t  _id;
    uint64_t _last_activity;
    uint8_t  _previous_id;
    Screen   _screen;
    uint16_t  _duration;
    uint8_t  _brightness;
    bool     _exist;
    bool     _update;
};

class Lenuage {
  // user-accessible "public" interface
  public:
    Lenuage(char* server, char* apikey, char* fingerprint);
    String getBoite();
    String getTileRaw(int id);
    void updateBuffer();
    void updateTile(int index);
    void printBuffer();
    Screen getTileScreen(int id);
    int getNextTile(int id);
    Tile getTile(int id);

  // library-accessible "private" interface
  private:
    char server[100];
    char apikey[36+1];
    char fingerprint[59+1];
    String url;
    Tile tiles[TILES_MAX_NUMBER];
};

#endif
