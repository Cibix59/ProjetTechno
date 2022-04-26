#include "stone.h"

#include <thread>

Stone::Stone(){
    
};

int Stone::init(char *comPort,uint speed){
    MySerial *mySerial = new MySerial(comPort);
    /* int valRetour = mySerial->init(comPort); */
    /* return (valRetour); */
    return 0;
}