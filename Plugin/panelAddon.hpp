#ifndef PANNEL_ADDON_HPP
#define PANNEL_ADDON_HPP

class panelAddon {
protected:
    double side_length_;

public:
    panelAddon()
        : side_length_(0) {}

    virtual ~panelAddon() {}

    void set_side_length(double side_length) {
        side_length_ = side_length;
    }

    virtual double area() const = 0;
};

// the types of the class factories
typedef panelAddon* create_t();
typedef void destroy_t(panelAddon*);

#endif
