#include "stone.h"
#include <sstream>

#include <thread>

Stone::Stone(std::string portName)
    : MySerial{portName} {}

      ;

int Stone::init(char *comPort, uint speed)
{
  /* MySerial *mySerial = new MySerial(comPort); */

  /* int valRetour = init(comPort); */
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
void Stone::setText(char *label, const char *text)
{
  char cmdFormat2[99];
  // std::string tmp =  "ST<{\"cmd_code\":\"set_text\",\"type\":"+label.c_str()+",\"widget\":\"label1\","+text.c_str()+":\"test\"}>ET";

  std::stringstream ss;
  ss << "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"" << label << "\",\"text\":\"" << text << "\"}>ET";
  std::string commande = ss.str();

  strcpy(cmdFormat2, commande.c_str());

  std::cout << cmdFormat2 << "\n";
  writeIt((char *)cmdFormat2);
}
void Stone::OuvrirFenetre(char *fenetre)
{
  char cmdFormat2[99];
  std::stringstream ss;
  ss << "ST<{\"cmd_code\":\"open_win\",\"widget\":\"" << fenetre << "\"}>ET";
  std::string commande = ss.str();

  strcpy(cmdFormat2, commande.c_str());

  std::cout << cmdFormat2 << "\n";
  writeIt((char *)cmdFormat2);
}

// Lire le contenu disponible sur le port série
// Retourne les données dans la structure local datasRead
// Si dataRead.id == 0 alors il n'y a pas de données lues sur le port
// Exemple de données qui peuvent être lues sur le port:
//   Basic format(Voir la documentation de l'écran pour plus de détails):
//     Frame head  : ST<
//     Command     : 0x1068 (exemple)
//     Longeur     : 0x0004 (exemple)
//     Data        : ctew   (exemple)
//     Key Value   : 0x01   (exemple)
//     Frame Tail  : >ET
//     CRC         : AB 24  (exemple)
datasRead Stone::getValidsDatasIfExists()
{
  int n, ii;
  datasRead rd;

  int commande, longeur;
  char data[2048];
  struct
  {
    union
    { // Commande
      unsigned short hexaShort;
      struct
      {
        char c1;
        char c2;
      } c;
    } shortDataCommand;
    union
    { // Longeur
      unsigned short hexaShort;
      struct
      {
        char c1;
        char c2;
      } c;
    } shortDataLongeur;

  } shortData;

  // Initialisation
  rd.id = 0; // 0 : Pas de données
  rd.line[0] = 0x00;
  rd.command[0] = 0x00;
  rd.name[0] = 0x00;
  rd.type = 0;

  // Essai de trouver un S
  n = readIt((char *)data, 1);
  while ((n == 1) && (data[0] != 'S'))
  {
    n = readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "S FOUND\n";

  // Essai de trouver un T
  n = readIt((char *)data, 1);
  while ((n == 1) && (data[0] != 'T'))
  {
    n = readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "T FOUND\n";

  // Essai de trouver un <T>
  n = readIt((char *)data, 1);
  while ((n == 1) && (data[0] != '<'))
  {
    n = readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "< FOUND\n";

  // Lecture de la commande et de la longeur de la donnee
  n = readIt(&shortData.shortDataCommand.c.c1, 4);

  // Inverser certaines données
  std::swap(shortData.shortDataCommand.c.c2, shortData.shortDataCommand.c.c1);
  std::swap(shortData.shortDataLongeur.c.c2, shortData.shortDataLongeur.c.c1);

  commande = shortData.shortDataCommand.hexaShort;
  longeur = shortData.shortDataLongeur.hexaShort;

  // Lecture Data
  n = readIt((char *)data, longeur);

  switch (commande)
  {

  case 0x0002:
  { // Version
    int keyValue = (int)data[longeur - 1];
    data[longeur - 1] = 0x00;

    // Lire les données suivantes : TAIL (3 char ">ET") et CRC (Hexa16)
    char TailDatas[5];
    n = readIt(TailDatas, 5);
    // Check if TAIL (>ET) is OK
    if ((n != 5) || (TailDatas[0] != '>') || (TailDatas[1] != 'E') || (TailDatas[2] != 'T'))
      return (rd);
    // Nous ne vérifions pas le CRC pour plus de rapidité mais ce serait mieux...
    // Traitement du CRC
    // int crc = TailDatas[4]; crc <<= 8; crc |= TailDatas[3];
    // std::cout << "Crc: " << intToString(crc, "%4X") << "\n";

    rd.id = commande;
    strcpy(rd.command, "Version");
    strcpy(rd.name, data);
    rd.type = keyValue;

    return (rd);
    break;
  };

  default:
  {

    int keyValue = (int)data[longeur - 1];
    data[longeur - 1] = 0x00;
    // std::cout << "Key Value: " << keyValue << "\n";
    // std::cout << "Data: " << data << "\n";

    // Lire les données suivantes : TAIL (3 char ">ET") et CRC (Hexa16)
    char TailDatas[5];
    n = readIt(TailDatas, 5);

    // Check if TAIL (>ET) is OK
    if ((n != 5) || (TailDatas[0] != '>') || (TailDatas[1] != 'E') || (TailDatas[2] != 'T'))
      return (rd);
    // Nous ne vérifions pas le CRC pour plus de rapidité mais ce serait mieux...
    // Traitement du CRC
    // int crc = TailDatas[4]; crc <<= 8; crc |= TailDatas[3];
    // std::cout << "Crc: " << intToString(crc, "%4X") << "\n";

    /* rd.id = -commande; */
    rd.id = commande;
    strcpy(rd.command, "???????");
    strcpy(rd.name, data);
    rd.type = keyValue;
    std::cout << "GData TEST : " << rd.id << " " << rd.command << " " << rd.name << " " << rd.type << "\n";

    return (rd);
    break;
  };
  };

  return (rd);
};

/*   if (selection == 6)
  { // Demande set heure
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_xy\",\"type\":\"label\",\"widget\":\"label1\",\"x\":150,\"y\":150}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);
    menu();
  } */

/*     if (selection == 7)
  { // Demande rotation
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_rotation\",\"type\":\"image\",\"widget\":\"logocegep\",\"rotation\":90}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);

    sleep(200);

    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_rotation\",\"type\":\"image\",\"widget\":\"logocegep\",\"rotation\":150}>ET");
    std::cout << cmdFormat2 << "\n";
    writeIt((char *)cmdFormat2);

    menu();
  } */