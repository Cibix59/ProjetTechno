

// source : http://les-electroniciens.com/videos/arduino-ep14-comment-utiliser-le-module-rfid-rc522

#include <Arduino.h>

// Libraries
#include <SPI.h>     //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h> //https://github.com/miguelbalboa/rfid
#include "myFunctions.cpp"
using namespace std;
#include <Wifi.h>


// Variable pour la connection Wifi
const char *SSID = "Clement_";
const char *PASSWORD = "mdp_";
String ssIDRandom;

// les includes du main
#include <iostream>
#include <string>

// Pour la gestion du WIFI Manager
#include <HTTPClient.h>
#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

// Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

// Constants
#define SS_PIN 5
#define RST_PIN 0
// Methodes
void readRFID(void);
void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);

// Parameters
const int ipaddress[4] = {103, 97, 67, 25};
// Variables
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);

std::string CallBackMessageListener(string message)
{

  while (replaceAll(message, std::string("  "), std::string(" ")))
    ;
  // Décortiquer le message
  string actionToDo = getValue(message, ' ', 0);
  string arg1 = getValue(message, ' ', 1);
  string arg2 = getValue(message, ' ', 2);
  string arg3 = getValue(message, ' ', 3);
  string arg4 = getValue(message, ' ', 4);
  string arg5 = getValue(message, ' ', 5);
  string arg6 = getValue(message, ' ', 6);
  string arg7 = getValue(message, ' ', 7);
  string arg8 = getValue(message, ' ', 8);
  string arg9 = getValue(message, ' ', 9);
  string arg10 = getValue(message, ' ', 10);

  if (string(actionToDo.c_str()).compare(string("action")) == 0)
  {
    return (String("Ok").c_str());
  }

  std::string result = "";
  return result;
}

void setup()
{
  // Init Serial USB
  // Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("debut");

  // ----------------Connection au WifiManager----------------
  String ssIDRandom, PASSRandom;
  String stringRandom;
  stringRandom = get_random_string(4).c_str();
  ssIDRandom = SSID;
  ssIDRandom = ssIDRandom + stringRandom;
  stringRandom = get_random_string(4).c_str();
  PASSRandom = PASSWORD;
  PASSRandom = PASSRandom + stringRandom;

  // ----------------Initialisation du wifi----------------
  char strToPrint[128];
  sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom.c_str(), PASSRandom.c_str());
  Serial.println(strToPrint);
  Serial.println(WiFi.macAddress());
  if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str()))
  {
    Serial.println("Erreur de connexion.");
  }
  else
  {
    Serial.println("Connexion Établie:");
    Serial.println((WiFi.localIP()).toString().c_str());
    Serial.println(PASSRandom.c_str());
  }

  // ----------- Routes du serveur ----------------
  myServer = new MyServer(80);
  myServer->initAllRoutes();

  myServer->initCallback(&CallBackMessageListener);

  // init rfid D8,D5,D6,D7
  SPI.begin();
  rfid.PCD_Init();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
}
void loop()
{
  readRFID();
}
void readRFID(void)
{ /* function readRFID */
  ////Read RFID card
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println(rfid.uid.size);
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize)
{
  Serial.println("printhex");
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize)
{
  Serial.println("printdec");
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}