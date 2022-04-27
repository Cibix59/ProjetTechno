#include "panelAddon.hpp"
#include <cmath>
#include "stone.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class plugInPorte : public panelAddon
{
private:
    Stone *stone;

public:
    plugInPorte() : panelAddon(){};

    int test(){
        return 42;
    }

    int init(std::string filename, Stone *stone)
    {
        // mqtt
        // xml
        // stone
        return 0;
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
