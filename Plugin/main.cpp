#include "panelAddon.hpp"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>          //Dynamic library


 #include <thread>         // std::thread
 #include <chrono>          //Sleep

void* plugIns[1024];
create_t* create_plugIns[1024];
destroy_t* destroy_plugIns[1024];


int main(int argc, char** argv){
    using std::cout;
    using std::cerr;

    // load the plugInFumee library
    plugIns[0] = dlopen("./plugInFumee.so", RTLD_LAZY);
    if (!plugIns[0] ) {
        cerr << "Cannot load plugInFumee: " << dlerror() << '\n';
        return 1;
    }

    // reset errors
    dlerror();

    // load the symbols
    create_plugIns[0] = (create_t*) dlsym(plugIns[0] , "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }

    
    destroy_plugIns[0] = (destroy_t*) dlsym(plugIns[0], "destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }

    // create an instance of the class
    panelAddon* addon = create_plugIns[0]();

    // use the class
    addon->set_side_length(7);
        cout << "The area is: " << addon->area() << '\n';

    // destroy the class
    destroy_plugIns[0](addon);

    // unload the triangle library
    dlclose(plugIns[0]);

}
