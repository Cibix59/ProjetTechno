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
    /*     MySerial *serialPort; */
    char commPortName[80];
    void displayCharString(char *str, int len);

    int formatStrCommand(const char *cmd_code, const char *widget, char *buffer);
    ssize_t read_port(int fd, uint8_t *buffer, size_t size);
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
