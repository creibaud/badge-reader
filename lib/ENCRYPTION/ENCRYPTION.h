#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <Arduino.h>

// Fonction pour convertir une chaîne de caractères en représentation hexadécimale
String StringToHex(const String& str);
// Fonction pour convertir une représentation hexadécimale en chaîne de caractères
String HexToString(const String& hex);

#endif // ENCRYPTION_H