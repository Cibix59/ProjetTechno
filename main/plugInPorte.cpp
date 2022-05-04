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

using namespace std;

class plugInPorte : public panelAddon
{
private:
    Stone *stone;

public:
    class myMqtt *mqtt;
    plugInPorte() : panelAddon(){};
    string xmlDescription;

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
        //todo : verifier si le mqtt est lancé
        mqtt = new myMqtt("raspberry", "zigbee2mqtt/0x00124b0023428a8a/set", "172.16.226.101", 1883);
        mqtt->send_msg( "{\"state\": \"ON\"}");
        delete mqtt;
    }
    void fermerPorte()
    {
        mqtt = new myMqtt("raspberry", "zigbee2mqtt/0x00124b0023428a8a/set", "172.16.226.101", 1883);
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
        std::cout << xmlDescription << std::endl;

        // stone
        this->stone = stone;
        stone->setText("lblhistoriquerfid", "test Historique");

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
