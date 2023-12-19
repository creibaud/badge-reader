#include "ENCRYPTION.h"

String StringToHex(const String& str) {
  String hexString = "";
  for (size_t i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    hexString += String(c, HEX);
  }
  return hexString;
}

String HexToString(const String& hex) {
  String str = "";
  for (size_t i = 0; i < hex.length(); i += 2) {
    String hexChar = hex.substring(i, i + 2);
    char c = strtol(hexChar.c_str(), NULL, 16);
    str += c;
  }
  return str;
}