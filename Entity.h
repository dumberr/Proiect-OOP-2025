#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <memory>

// 1) Clasa abstracta de baza pentru toate obiectele din joc
class Entity {

protected:
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;

public:
    virtual ~Entity() = default;                    // Tema 2: functie virtuala si destructor virtual

    // Tema 2: functie virtuala pura pentru actualizare si randare
    virtual void update() = 0;
    virtual void render() const = 0;

    // Pentru Tema 1: suprasarcina operator<< si operator>>
    friend std::ostream& operator<<(std::ostream& os, const Entity& obj) {
        obj.print(os);
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Entity& obj) {
        obj.read(is);
        return is;
    }


};

#endif
