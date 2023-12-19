#include "ENCRYPTION.h"

// Convertir une chaîne de caractères en représentation hexadécimale
String StringToHex(const String& str) {
  String hexString = "";
  for (size_t i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    hexString += String(c, HEX);
  }
  return hexString;
}

// Convertir une représentation hexadécimale en chaîne de caractères
String HexToString(const String& hex) {
  String str = "";
  for (size_t i = 0; i < hex.length(); i += 2) {
    // Extraire deux caractères hexadécimaux à la fois
    String hexChar = hex.substring(i, i + 2);

    // Convertir les caractères hexadécimaux en caractère ASCII
    char c = strtol(hexChar.c_str(), NULL, 16);

    // Ajouter le caractère à la chaîne résultante
    str += c;
  }
  return str;
}