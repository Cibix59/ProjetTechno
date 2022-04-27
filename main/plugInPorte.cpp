#include "panelAddon.hpp"
#include <cmath>

class plugInPorte : public panelAddon
{
public:
    plugInPorte() : panelAddon(){};
    int test()
    {
        return 42;
    }

};
/*     // the class factories
    extern "C" panelAddon *create()
    {
        return new plugInPorte;
    }

    extern "C" void destroy(panelAddon *p)
    {
        delete p;
    } */