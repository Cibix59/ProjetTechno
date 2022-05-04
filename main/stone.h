#ifndef STONE_H
#define STONE_H

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "MySerial.h"

struct datasRead
{
    int id; // 0 = pas de données
    char command[80];
    char name[80];
    int type;
    char line[2048];
};

class Stone : public MySerial
{
    using MySerial::MySerial;
private:
    //todo : transferer ces methodes
    std::string charToString(char str, std::string formatStr);
    std::string intToString(int value, std::string formatStr);
public:
    Stone(std::string portName);
    ~Stone(){};
    int init(char *comPort, uint speed = 115200);
    void reboot();
    void firmware();
    void setHeure();
    void setText(char* label,char* text);
    datasRead getValidsDatasIfExists();

};

#endif
