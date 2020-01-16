#ifndef CYCLEWALL_H
#define CYCLEWALL_H

#include "wall.h"
#include "ofMain.h"
#include "cycle.h"

class cycleWall : public wall
{
public:
    cycleWall();
    cycleWall(ofColor _color, cycle c);
    ~cycleWall();

    void placeWall(cycle c);
    void draw();

private:

};

#endif // CYCLEWALL_H
