#ifndef CYCLEWALL_H
#define CYCLEWALL_H

#include "wall.h"
#include "ofMain.h"

class cycleWall : public wall
{
public:
    //Constructor
    cycleWall();
    cycleWall(ofColor _color);

    //Destructor
    ~cycleWall();

    //Iherited methods
    void draw();

    //Other methods
    void placeWall();


private:
    void setup(); //Setup wall dims
};

#endif // CYCLEWALL_H
