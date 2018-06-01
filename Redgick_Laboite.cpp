// include Arduino core library
#include "Arduino.h"

// include this library's description file
#include "Redgick_Laboite.h"

// include description files for other libraries used


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Tile::Tile() {
  _screen.init(32, 16, BICOLOR);
  clear();
};


Lenuage::Lenuage(char* server, char* apikey, char* fingerprint = "") {
  strcpy(this->server, server);
  strcpy(this->apikey, apikey);
  strcpy(this->fingerprint, fingerprint);
  this->url = String(server) + "/boites/" + String(apikey) + "/";
};


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Redgick_GFX sketches, this library, and other libraries

void Tile::clear() {
  setId(0);
  setLastActivity(0);
  setPreviousId(0);
  setDuration(0);
  setBrightness(0);
  exist(false);
  needUpdate(false);
};


String Lenuage::getBoite() {
  String json; // buffer

  HTTPClient http;
  if (server[4] == 's') {
    http.begin(url, fingerprint);
  }
  else {
    http.begin(url);
  }
  http.addHeader("Accept", "application/json");

  int httpCode = http.GET();
  if (httpCode > 0 ){
    // HTTP header has been send and Server response header has been handled

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      json = http.getString();
    }
  }
  http.end();

  return json;
};


String Lenuage::getTileRaw(int id) {
  String json; // buffer
  String urlTile = url + "tiles/" + String(id) + "/";

#ifdef DEBUG
  Serial.println(urlTile);
#endif

  HTTPClient http;
  if (server[4] == 's') {
    http.begin(urlTile, fingerprint);
  }
  else {
    http.begin(urlTile);
  }
  http.addHeader("Accept", "application/json");

  int httpCode = http.GET();
  if (httpCode > 0 ){
    // HTTP header has been send and Server response header has been handled

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      json = http.getString();
    }
  }
  http.end();

  return json;
};


void Lenuage::updateBuffer() {
  // we create a buffer
  String json;

  // then we get a records collection
  json = getBoite();

  // Don’t reuse the same JsonBuffer
  // During is lifetime a JsonBuffer growth until it’s discarded.
  // If you try to reuse the same instance several time, it will rapidly get full.
  StaticJsonBuffer<1024> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(json); // JsonObject alloué en mémoire dynamiquement ?
  if (!root.success()) {
#ifdef DEBUG
    Serial.println("parseObject() failed");
#endif
    return;
  }

  // on met le flag "exist" à "false"
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    tiles[i].exist(false);
  }

  // pour chaque tuile on renseigne l'id de la tuile précédente
  uint8_t previous_id = 0;

  // on explore l'arbre json
  JsonArray& data = root["tiles"];
  for (auto tile : data) {
    JsonObject& tile_data = tile;

    uint8_t id = 0;
    uint32_t last_activity = 0;

    if (tile_data.containsKey("id")) {
      id = int(tile_data["id"]);
    }
    else {
      continue; // problème de lecture, on passe à la prochaine valeur de la boucle for
    }

    if (tile_data.containsKey("last_activity")) {
      last_activity = long(tile_data["last_activity"]);
    }
    else {
      continue; // problème de lecture, on passe à la prochaine valeur de la boucle for
    }

    // la tuile est-elle présente dans le buffer ? et si oui, à quel indice ?
    bool exist = false;
    uint8 index = 0;
    for (int i = 0; i < TILES_MAX_NUMBER; i++) {
      if (tiles[i].getId() == id) {
        exist = true;
        index = i;
        break;
      }
    }

    if (exist) {
      // la tuile existe
      // est-elle à jour ?
      if (tiles[index].getLastActivity() == last_activity) {
        // la tuile est à jour
        // ... rien à faire
      }
      else {
        // la tuile n'est pas à jour
        // on active le flag update
        tiles[index].needUpdate(true);

        // ATTENTION
        // LE JSON DE LA TUILE NE CONTIENT PAS L'INFORMATION last_activity !!!
        tiles[index].setLastActivity(last_activity);
        // ATTENTION
      }
    }
    else {
      // la tuile n'existe pas encore

      // on recherche la premiere place libre dans le buffer (s'il en reste)
      bool buffer_overflow = true;
      for (int i = 0; i < TILES_MAX_NUMBER; i++) {
        if (tiles[i].getId() == 0) {
          // une case est libre !
          buffer_overflow = false;
          index = i;
          break;
        }
      }

      // si une place est disponible
      if (not buffer_overflow) {
        // on crée la tuile
        tiles[index].setId(id);
        tiles[index].setLastActivity(last_activity);

        // on la met à jour
        tiles[index].needUpdate(true);
      }
      else {
        // le buffer est plein
        // que fait-on ?
      }
    }

    // on active le flag "exist"
    tiles[index].exist(true);
    tiles[index].setPreviousId(previous_id);
    previous_id = id;
    // remarque : la première tuile est la seule à avoir un previous_id==0 et id!=0
  }

  // on libère les tuiles pour lesquelles le flag "exist" est à "false"
  // cela correspond aux tuiles supprimées depuis la précédente mise à jour du buffer
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    if (tiles[i].getId() != 0 and (not tiles[i].exist())) {
#ifdef DEBUG
      Serial.print("suppression de la tuile ");
      Serial.println(tiles[i].getId());
#endif
      tiles[i].clear();
    }
  }

  // mise à jour des tuiles dont le flag "update" est true
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    if (tiles[i].needUpdate()) {
      updateTile(i);
    }
  }
};


void Lenuage::updateTile(int index) {
#ifdef DEBUG
  Serial.print("mise a jour de la tuile ");
  Serial.println(tiles[index].getId());
#endif

  // on efface le screen de la tuile
  tiles[index].clearScreen();

  // we create a buffer
  String json;

  // then we get a records collection
  json = getTileRaw(tiles[index].getId());

  // Don’t reuse the same JsonBuffer
  // During is lifetime a JsonBuffer growth until it’s discarded.
  // If you try to reuse the same instance several time, it will rapidly get full.
  StaticJsonBuffer<1024> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(json); // JsonObject alloué en mémoire dynamiquement ?
  if (!root.success()) {
#ifdef DEBUG
    Serial.println("parseObject() failed");
#endif
    return;
  }

  if (root.containsKey("duration")) {
    tiles[index].setDuration(uint16_t(root["duration"]));
  }

  if (root.containsKey("brightness")) {
    tiles[index].setBrightness(int(root["brightness"]));
  }

  // on explore l'arbre json
  JsonArray& data = root["items"];
  for (auto item : data) {
    JsonObject& item_data = item;

    int scrolling = 0;
    if (item_data.containsKey("scrolling")) {
      scrolling = int(item_data["scrolling"]);
    }

    int x = 0;
    if (item_data.containsKey("x")) {
      x = int(item_data["x"]);
    }

    int y = 0;
    if (item_data.containsKey("y")) {
      y = int(item_data["y"]);
    }

    int width = 8;
    if (item_data.containsKey("width")) {
      width = int(item_data["width"]);
    }

    int height = 8;
    if (item_data.containsKey("height")) {
      height = int(item_data["height"]);
    }

    int color = RED;
    if (item_data.containsKey("color")) {
      color = int(item_data["color"]);
    }

    int font = FONT_DEFAULT;
    if (item_data.containsKey("font")) {
      font = int(item_data["font"]);
    }

    String content = "";
    if (item_data.containsKey("content")) {
      content = item_data["content"].asString();
    }

    if (item_data.containsKey("type")) {
      String type = item_data["type"].asString();
      if (type.equals("text")) {
        tiles[index].print(x, y, content, font, color);
      }
      else {
        if (type.equals("bitmap")) {
          tiles[index].drawBitmap(x, y, Bitmap(width, height, content), color);
        }
        else {
#ifdef DEBUG
          Serial.println("type inconnu ...");
#endif
          return;
        }
      }
    }
  }

  // mise à jour effectuée !
  tiles[index].needUpdate(false);
};


void Lenuage::printBuffer() {
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    Serial.print("index ");
    Serial.print(i);
    Serial.print(" | id ");
    Serial.print(tiles[i].getId());
    Serial.print(" | last_activity ");
    Serial.print(long(tiles[i].getLastActivity()));
    Serial.print(" | duration ");
    Serial.print(long(tiles[i].getDuration()));
    Serial.print(" | previous_id ");
    Serial.print(tiles[i].getPreviousId());
    Serial.print(" | update ");
    Serial.print(tiles[i].needUpdate());
    Serial.print(" | exist ");
    Serial.println(tiles[i].exist());
    Serial.print(" | brightness ");
    Serial.println(tiles[i].getBrightness());
  }
};


Screen Lenuage::getTileScreen(int id) {
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    if (tiles[i].getId() == id) {
      // on a trouvé la tuile !
      return(tiles[i].getScreen());
    }
  }
  // par defaut on renvoie le premier buffer de la file
  return tiles[0].getScreen();
};


int Lenuage::getNextTile(int id) {
  // exemple id=23
  // quelle est la tuile dont le previous_id == 23 ?
    // c'est la tuile id == 24
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    if (tiles[i].getPreviousId() == id and tiles[i].exist()) {
      // on a trouvé la tuile !
      return(tiles[i].getId());
    }
  }
  // si on n'a pas trouvé, on affiche la première tuile du buffer
  // celle dont _previous_id == 0 et _exist == 1
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    if (tiles[i].getPreviousId() == 0 and tiles[i].exist()) {
      // on a trouvé la tuile !
      return(tiles[i].getId());
    }
  }
  // par défaut on renvoie la première tuile du buffer
  return(tiles[0].getId());
};


Tile Lenuage::getTile(int id) {
  for (int i = 0; i < TILES_MAX_NUMBER; i++) {
    if (tiles[i].getId() == id) {
      // on a trouvé la tuile !
      return(tiles[i]);
    }
  }
  // par defaut on renvoie la premiere
  return tiles[0];
};

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
