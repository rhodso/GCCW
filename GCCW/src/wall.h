#ifndef WALL_H
#define WALL_H

#include "gameobject.h"
#include "ofMain.h"

class wall : public gameObject
{
public:
    //Constructors
    wall();
    wall(ofColor _color);

    //Deconstructors
    virtual ~wall();

    //Getters and setters
    void setColor( ofColor _color);
    ofColor getColor();

    virtual void draw();
    virtual void doAI();

protected:
    ofColor color;
};

#endif // WALL_H
