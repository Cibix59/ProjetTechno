/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 *
    @file     main.cpp
    @author   Clément Bourdier
    @version  1.2 08/05/22
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe SerialPort

    platform = Raspberry Pi
    OS = Linux
    Langage : C++

    Inspiration
        https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

    Compilation (voir le fichier ReadMe.txt pour plus de détails)
      Utiliser le Makefile
        sudo make

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain       Première version du logiciel
        1.2        08/05/22  Clément     Rendu porjet technologique

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
        Optenir la version du Firmware de l'écran
    Fonctionnalités implantées (v1.2)
        Utilisation de la classe Stone.cpp
        Gestion de l'evenement switch sur l'ecran
        Detection et lecture de fichiers XML
        Chargement de plugins c++

 * */
#include "panelAddon.hpp"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h> //Dynamic library

#include <sstream>

#include <thread> // std::thread
#include <chrono> //Sleep

#include "stone.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <filesystem>
#include <stdlib.h>

#include "MySerial.h"
MySerial *mySerial;

void *plugIns[1024];
create_t *create_plugIns[1024];
destroy_t *destroy_plugIns[1024];
int NbrePlugIns = 0;
int NbreAddon = 0;
panelAddon *addon[1024];

static Stone *stone;

std::string stoneVersion;
std::string xmlVersion;
std::string xmlTeam;
std::string xmlThanks;

int positionPluginVoulu = -1;

namespace fs = std::filesystem;

// Structure local utilisés pour garder les informations lues de l'écran
/* struct datasRead
{
  int id; // Si 0 alors il n'y a pas de données, <0 Numero commande non traitée, >1 Numero de la commande traité
  char command[80];
  char name[80];
  int type;
  char line[2048];
}; */

// Menu utilisé pour tester les fonctionalités implantées
int selection = 0;
char data[255];
void menu()
{
  std::cout << std::endl;
  selection = 0;
  std::cout << " Please choose from the following options - \n";
  std::cout << " 7. rotation. \n";
  std::cout << " 6. xy. \n";
  std::cout << " 5. labelTest. \n";
  std::cout << " 4. SetHeure.\n";
  std::cout << " 3. Reboot.\n";
  std::cout << " 1. Lire la version du FirmWare.\n";
  std::cout << " 0. Exit.\n";
  std::cout << "\t";
  std::cout << "Commande : ";
  std::cin >> selection;

  if (selection == 1)
  { // Demande la version du FirmWare
    stone->firmware();
    menu();
  }
  if (selection == 3)
  { // Demande reboot
    stone->reboot();
    menu();
  }

  if (selection == 4)
  { // Demande set heure
    stone->setHeure();
    menu();
  }
  if (selection == 5)
  { // Change un label

    menu();
  }

  else if (selection == 0)
  {
    return;
  }
  menu();
}

std::string intToHexa(int value)
{
  char buffer[10];
  sprintf(buffer, "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr)
{
  char buffer[10];
  sprintf(buffer, formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr)
{
  char buffer[10];
  sprintf(buffer, formatStr.c_str(), value);
  return (buffer);
};

// Thread qui permet de LOOPER et lire le contenu du port serie
// avec l'aide du la fonction getValidsDatasIfExists
void fonctionLoop()
{
  while (true)
  {

    datasRead rd = stone->getValidsDatasIfExists();
    /* std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n"; */
    switch (rd.id)
    {

    case 0x0002:
    { // Version
      std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
      std::string stoneVersion = rd.name;
      std::cout << "Version : " << stoneVersion.c_str() << "\n";

      // std::this_thread::sleep_for(std::chrono::milliseconds(10));

      break;
    }
    case 4112:
    {
      std::cout << " ok";
      // Bouton
      std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
      std::string stoneVersion = rd.name;
      if(rd.type == 0){
        addon[positionPluginVoulu]->fermerPorte();
      }else if (rd.type ==1)
      {
        addon[positionPluginVoulu]->ouvrirPorte();
      }
      
      
    //todo : declenche porte
        //meilleur todo : avoir une liste vide d'event à surveiller, remplie par le plugin, associée à une methode du plugin à appeler -> creer un listener en gros
      // std::this_thread::sleep_for(std::chrono::milliseconds(10));

      break;
    }
    }

    if (rd.id < 0)
    {

      std::cout << abs(rd.id);
      std::cout << rd.name;
      std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type << ")\n";
    }

    if (rd.id == 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}

int main(int argc, char **argv)
{
  using std::cerr;
  using std::cout;

  char serialPort[255];
  strcpy(serialPort, "/dev/");
  strcat(serialPort, argv[1]);

  cout << std::string("Serial port used: ") << serialPort << "\n";

  mySerial = new MySerial(serialPort);
  stone = new Stone(serialPort);
  /* char ComPortName[] = {"/dev/ttyUSB0"}; */
  char ComPortName[] = {"/dev/"};
  // int valRet = stone.init(ComPortName /* serialPort */, 115200);
  // todo : en test

  /*   if (valRet == -1)
    {
      return (0);
    } */

  // return(0);
  // Lecture des configurations Xml du programme
  // Vide et initialise tous les éléments de l'écran
  rapidxml::file<> xmlFile("stone.xml");
  rapidxml::xml_document<> doc;
  doc.parse<0>(xmlFile.data());
  // rapidxml::xml_node<> *node = doc.first_node();
  xmlVersion = (doc.first_node("nom") ? doc.first_node("nom")->value() : "");
  xmlTeam = (doc.first_node("broker") ? doc.first_node("broker")->value() : "");
  xmlThanks = (doc.first_node("description") ? doc.first_node("description")->value() : "");
  // handlenode(node);

  // Parcourir le répertoire pour loader les libraries
  int iPlugIns = 0;
  std::string path = "./";
  std::string ext(".so");
  // passe en revue tout les fichiers du dossier
  for (const auto &entry : fs::directory_iterator(path))
    // si le fichier est un .so, alors
    if (entry.path().extension() == ext)
    {
      std::cout << "\nLibrarie trouvee: " << entry.path() << std::endl;

      // Essai de loader la librarie
      std::string libraryFileName = entry.path();
      // ajoute la librairie au tableau des librairies
      plugIns[NbrePlugIns] = dlopen(libraryFileName.c_str(), RTLD_LAZY);
      if (!plugIns[NbrePlugIns])
      {
        cerr << "Cannot load " << libraryFileName.c_str() << ": " << dlerror() << '\n';
        return 1;
      }
      std::cout << "\nLibrarie overte " << std::endl;

      // reset errors
      dlerror();

      // load the symbols
      create_plugIns[NbrePlugIns] = (create_t *)dlsym(plugIns[NbrePlugIns], "create");
      const char *dlsym_error = dlerror();
      if (dlsym_error)
      {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
      }

      destroy_plugIns[NbrePlugIns] = (destroy_t *)dlsym(plugIns[NbrePlugIns], "destroy");
      dlsym_error = dlerror();
      if (dlsym_error)
      {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
      }

      // Trouver tous les fichiers Xml correspendants (maximum de 10 pour l'instant)
      int vRet = 0;
      for (int jj = 0; jj < 10; jj++)
      {
        std::string fileName = entry.path().parent_path();
        fileName += std::string("/");
        fileName += entry.path().filename().replace_extension("xml");
        /* fileName += std::to_string(jj); */

        std::cout << fileName << std::endl;

        if (fs::exists(fileName))
        {
          cout << "File exist: " << fileName << "\n";

          addon[NbreAddon] = create_plugIns[NbrePlugIns]();

          vRet = addon[NbreAddon]->init(fileName, stone);
          //todo : retirer ca, c'est pas tres beau
          if(vRet == 42){
            positionPluginVoulu = NbreAddon;
          }
          if (vRet < 0)
          {
            cerr << "Initialisation addon failed: " << vRet << '\n';
            continue;
          }
          // test ici
          std::cout << "\npendant init part -1" << std::endl;
          // addon[NbreAddon]->startMqtt();
          std::cout << "\npendant init part 0" << std::endl;
          /*           addon[NbreAddon]->sendInfos("testtt");
                    addon[NbreAddon]->stopMqtt();
                    cout << "The test is ..... : " << std::to_string(addon[NbreAddon]->test()) << '\n'; */
          /* cout << "The test is ..... : " << std::to_string(addon[NbreAddon]->test()) << '\n'; */
          NbreAddon++;
        }
      }

      NbrePlugIns++;
    }

  std::cout << "\navant init" << std::endl;
  // Parcourir les plugins et les initialiser
  for (int i = 0; i < NbreAddon; i++)
  {
    /*     addon[i]->init(); */
  }

  std::cout << "\napres init" << std::endl;

  /*   std::cout << "\nLibrarie trouvee: " << std::to_string(plugIns[1]->test()) << std::endl; */

  /*     setUpStonePanel(); */

  // Lancer un tread pour lire les données de la tablette Stone
  std::thread first(fonctionLoop);

  menu();

  // Détruire les addOns
  for (int i = 0; i < NbreAddon; i++)
  {
    // todo terminer correctement tout les addons
    addon[i]->stopMqtt();
    delete addon[i];
    std::cout << "\nAddOn détruit : " << i << std::endl;
  }

  // Détruire les plugsins
  for (int i = 0; i < NbrePlugIns; i++)
  {
    // unload the library
    dlclose(plugIns[i]);
    std::cout << "\nLibrarie détruite : " << i << std::endl;
  }

  return (0);
}
