#include "stone.h"

#include <thread>

Stone::Stone(std::string portName)
    : MySerial{portName} {}

      ;

int Stone::init(char *comPort, uint speed)
{
    /* MySerial *mySerial = new MySerial(comPort); */

    /* int valRetour = mySerial->init(comPort); */
    /* return (valRetour); */
    return 0;
}

void Stone::reboot()
{
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_reboot\",\"type\":\"system\"}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);
}
void Stone::firmware()
{
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);
}
void Stone::setHeure()
{
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_date\",\"type\":\"digit_clock\",\"widget\":\"digit_clock\",\"date\":\"2022-02-09 13:58:30\"}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);
}
void Stone::setText(std::string label,std::string text)
{
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"label1\",\"text\":\"test\"}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);
}



/*   if (selection == 6)
  { // Demande set heure
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_xy\",\"type\":\"label\",\"widget\":\"label1\",\"x\":150,\"y\":150}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  } */


/*     if (selection == 7)
  { // Demande rotation
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_rotation\",\"type\":\"image\",\"widget\":\"logocegep\",\"rotation\":90}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);

    sleep(200);

    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_rotation\",\"type\":\"image\",\"widget\":\"logocegep\",\"rotation\":150}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);

    menu();
  } */