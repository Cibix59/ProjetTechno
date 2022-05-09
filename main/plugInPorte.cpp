/* Copyright (C) 2022 Clément Bourdier
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 *
    @file     pluginPorte.cpp
    @author   Clément Bourdier
    @version  1.1 09/05/22
    @description
      Plugin destiné à être chargé dans main.cpp

    platform = Raspberry Pi
    OS = Linux
    Langage : C++

    Fonctionnalités implantées
        communications avec le broker mqtt
        chargement du xml
        implementation de Stone

 * */
#include "panelAddon.hpp"
#include <cmath>
#include "stone.h"
#include <dlfcn.h>

#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

#include "myMqtt.hpp"
#include <curl/curl.h>

using namespace std;

class plugInPorte : public panelAddon
{
private:
    Stone *stone;

public:
    class myMqtt *mqtt;
    plugInPorte() : panelAddon(){};
    string xmlDescription;
    string xmlTopic = "zigbee2mqtt/0x00124b0023428a8a/set";
    string xmlIp = "172.16.226.101";
    int xmlPort = 1883;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

    void startMqtt()
    {
        mqtt = new myMqtt("raspberry", "rasp/test", "172.16.226.101", 1883);
        /*         mqtt->send_msg("msg");
                delete mqtt; */
    }

    void stopMqtt()
    {
        delete mqtt;
    }

    void ouvrirPorte()
    {
        // todo : verifier si le mqtt est lancé
        mqtt = new myMqtt("raspberry", xmlTopic.c_str(), xmlIp.c_str(), 1883);
        mqtt->send_msg("{\"state\": \"ON\"}");
        delete mqtt;
    }
    void fermerPorte()
    {
        mqtt = new myMqtt("raspberry", xmlTopic.c_str(), xmlIp.c_str(), 1883);
        mqtt->send_msg("{\"state\": \"OFF\"}");
        delete mqtt;
    }

    void sendInfos(const char *msg)
    {
        try
        {
            mqtt->send_msg(msg);
        }
        catch (const exception &e)
        {
            printf("Erreur dans l'envoie du message");
        }
    }

    bool getInfos()
    {
        try
        {
            bool set = mqtt->receive_msg();
            return set;
        }
        catch (const exception &e)
        {
            printf("Erreur dans la réception des messages");
            return 0;
        }
    }

    int test()
    {
        stone->reboot();
        return 42;
    }

    int init(std::string fileName, Stone *stone)
    {
        // mqtt
        /*         startMqtt();
                sendInfos("testOKK"); */
        // getInfos();

        /*         while (true)
                {
                    if(getInfos()){
                        std::cout << "\nrecu"<< std::endl;
                    }else{
                        std::cout << "\nNON"<< std::endl;
                    }

                    sleep(100);
                }
                 */

        // xml
        rapidxml::file<> xmlFile(fileName.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
        xmlDescription = (doc.first_node("description") ? doc.first_node("description")->value() : "");
        xmlTopic = (doc.first_node("topic") ? doc.first_node("topic")->value() : "");
        xmlIp = (doc.first_node("ip") ? doc.first_node("ip")->value() : "");
        // todo : resoudre probleme avec le port
        xmlPort = (int)(doc.first_node("port") ? doc.first_node("port")->value() : "");
        std::cout << "xmlDescription" << std::endl;
        std::cout << xmlDescription << std::endl;

        // stone
        this->stone = stone;

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "http://172.16.199.85:3000/api/historique/rfid");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            std::cout << "readBuffer" << std::endl;
            std::cout << readBuffer << std::endl;
        }

        stone->setText("lblhistoriquerfid", "Historique");
        /*         stone->setText("lbldescriptionrfid", "test avec espaces"); */

        stone->setText("lbldescriptionrfid", xmlDescription.c_str());

        // todo : set la description
        /* lbldescriptionrfid */

        return 42;
    };
    virtual double area() const
    {
        return side_length_ * side_length_ * sqrt(3) / 2;
    };
    /*     int test()
        {
            return 42;
        }; */
};

// the class factories
extern "C" panelAddon *create()
{
    return new plugInPorte;
}

extern "C" void destroy(panelAddon *p)
{
    delete p;
}
