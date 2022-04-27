#ifndef PANNEL_ADDON_HPP
#define PANNEL_ADDON_HPP

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "stone.h"

class panelAddon {
protected:
    double side_length_;

public:
    panelAddon()
        : side_length_(0) {}

    virtual ~panelAddon() {}

    virtual int init(std::string fileName,Stone* stone) {}

    void set_side_length(double side_length) {
        side_length_ = side_length;
    }

    virtual double area() const = 0;
    virtual int test(){};
};

// the types of the class factories
typedef panelAddon* create_t();
typedef void destroy_t(panelAddon*);

#endif
