int main(int argc, char** argv){
    using std::cout;
    using std::cerr;



    char ComPortName[] = {"/dev/ttyUSB0" };
    int valRet = stone.init(ComPortName, 115200);
    if(valRet==-1){
        return(0);
            }

 //return(0);
    //Lecture des configurations Xml du programme
    //Vide et initialise tous les éléments de l'écran
    rapidxml::file<> xmlFile("stone.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    //rapidxml::xml_node<> *node = doc.first_node();
    xmlVersion = (doc.first_node("version")? doc.first_node("version")->value():"");
    xmlTeam = (doc.first_node("team")? doc.first_node("team")->value():"");
    xmlThanks = (doc.first_node("thanks")? doc.first_node("thanks")->value():"");
    //handlenode(node);



    //Parcourir le répertoire pour loader les libraries
    int iPlugIns = 0;
    std::string path = "./";
    std::string ext(".so");
    for (const auto & entry : fs::directory_iterator(path))
        if (entry.path().extension() == ext) {
            std::cout << "\nLibrarie trouvee: " << entry.path() << std::endl;

            //Essai de loader la librarie
            // load the plugInFumee library
            std::string  libraryFileName = entry.path();
            plugIns[NbrePlugIns] = dlopen(libraryFileName.c_str(), RTLD_LAZY);
            if (!plugIns[NbrePlugIns] ) {
                cerr << "Cannot load " << libraryFileName.c_str() << ": " << dlerror() << '\n';
                return 1;
                }
            std::cout << "\nLibrarie overte " << std::endl;

            // reset errors
            dlerror();

            // load the symbols
            create_plugIns[NbrePlugIns] = (create_t*) dlsym(plugIns[NbrePlugIns] , "create");
            const char* dlsym_error = dlerror();
            if (dlsym_error) {
                cerr << "Cannot load symbol create: " << dlsym_error << '\n';
                return 1;
                }

            destroy_plugIns[NbrePlugIns] = (destroy_t*) dlsym(plugIns[NbrePlugIns], "destroy");
            dlsym_error = dlerror();
            if (dlsym_error) {
                cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
                return 1;
                }

            //Trouver tous les fichiers Xml correspendants (maximum de 10 pour l'instant)
            int vRet = 0;
            for(int jj=0; jj<10; jj++){
                std::string fileName = entry.path().parent_path();
                fileName += std::string("/");
                fileName += entry.path().filename().replace_extension("xml.");
                fileName += std::to_string(jj);

                if(std::filesystem::exists(fileName)){
                    cout << "File exist: " << fileName << "\n";

                    addon[NbreAddon] = create_plugIns[NbrePlugIns]();

                    vRet = addon[NbreAddon]->init(fileName, &stone);
                    if (vRet < 0 ){
                        cerr << "Initialisation addon failed: " << vRet << '\n';
                        continue;
                        }

                    NbreAddon++;
                    }
                }
                NbrePlugIns++;
            }

    //Parcourir les plugsins et les initialiser
    for(int i=0; i<NbreAddon; i++) {
        addon[i]->set_side_length(7);
        cout << "The area is: " << addon[i]->area() << '\n';
        }

  setUpStonePanel();

  //Lancer un tread pour lire les données de la tablette Stone
  std::thread first (f);

    menu();


    //Détruire les addOns
    for(int i=0; i<NbreAddon; i++) {
        delete addon[i];
        std::cout << "\nAddOn détruit : " << i << std::endl;
        }

    //Détruire les plugsins
    for(int i=0; i<NbrePlugIns; i++) {
        // unload the library
        dlclose(plugIns[i]);
        std::cout << "\nLibrarie détruite : "<< i << std::endl;
        }

    return(0);





}
