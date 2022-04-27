#include "panelAddon.hpp"
#include <cmath>
#include "stone.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace std;

class plugInPorte : public panelAddon
{
private:
    Stone *stone;

public:
    plugInPorte() : panelAddon(){};
    string xmlDescription;

    int test()
    {
        return 42;
    }

    int init(std::string fileName,Stone* stone)
    {
        // mqtt
        // xml
        rapidxml::file<> xmlFile(fileName.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
        xmlDescription = (doc.first_node("description") ? doc.first_node("description")->value() : "");
        std::cout << xmlDescription << std::endl;
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
