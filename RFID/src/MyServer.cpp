/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Clément Bourdier
    @version 1.1 20/11/20
    @version 1.2 30/11/21
    @version 1.3 23/02/22
    @version 1.4 21/03/22
*/
#include <Arduino.h>
#include "MyServer.h"
using namespace std;
typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

// Exemple pour appeler une fonction CallBack
// if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend);
void MyServer::initCallback(CallbackType callback)
{
    ptrToCallBackFunction = callback;
}

void MyServer::initAllRoutes()
{
    currentTemperature = 3.3f;

    // Initialisation du SPIFF.
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/login.html", "text/html"); });

    this->on("/home", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/index_fr.html", "text/html"); });

    // Route du script JavaScript
    this->on("/js/global.js", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/js/global.js", "text/javascript"); });

    this->on("/css/global.css", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/css/global.css", "text/css"); });

    this->on("/image/logo.png", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/image/logo.png", "image/png"); });

    this->onNotFound([](AsyncWebServerRequest *request)
                     { request->send(404, "text/plain", "Page Not Found"); });

    this->on("/connexion", HTTP_POST, [](AsyncWebServerRequest *request)
             {
                 String txt = "pasOK";
                 if (request->hasParam("login", true) && request->hasParam("password", true))
                 {
                     String login = request->getParam("login", true)->value();
                     String password = request->getParam("password", true)->value();
                     if(login == "testlogin"){
                        txt = "ok";
                     }  
                    Serial.println(login);
                    Serial.println(password);
                    Serial.println("ici");
                 }
               request->send(200, "text/plain", txt); });

    // récupere le status de l'esp (off/cold/heat)
    this->on("/getInfosFromESP", HTTP_GET, [](AsyncWebServerRequest *request)
             {
                     std::string repString = "";

                     if (ptrToCallBackFunction)
                         repString = (*ptrToCallBackFunction)("getInfosFromESP");

                     String infos = String(repString.c_str());

                     request->send(200, "text/plain", infos); });

    // met à jour les variables de l'esp
    this->on("/envoisInfosToEsp", HTTP_POST, [](AsyncWebServerRequest *request)
             {
                std::string repString = "";
                String infos="";

                if (request->hasParam("ipInput", true) )
                 {
                     infos = "ipInput "+request->getParam("ipInput", true)->value();
                 }
                if (request->hasParam("portInput", true) )
                 {
                    infos = "portInput "+request->getParam("portInput", true)->value();
                 }
                if (request->hasParam("intervalleInput", true) )
                 {
                    infos = "intervalleInput "+request->getParam("intervalleInput", true)->value();
                 }
                if (ptrToCallBackFunction)
                    repString = (*ptrToCallBackFunction)(("envoisInfosToEsp "+ infos).c_str());

               request->send(200, "text/plain", repString.c_str()); });

    this->begin();
};
