#ifndef BOUNDARYWALL_H
#define BOUNDARYWALL_H

#include "ofMain.h"
#include "wall.h"

class boundaryWall : public wall
{
public:
    //Constructors
    boundaryWall();
    boundaryWall(ofColor _color);

    //Desructor
    ~boundaryWall();

    //Getters and setters
    int getBoundary();
    void setBoundary(int _boundary);
    void setColor( ofColor _color);
    ofColor getColor();

    //Inherited methods
    void draw();
    void doAI();

private:
    int boundary;
    ofPlanePrimitive wallPlane;
};

#endif // BOUNDARYWALL_H
