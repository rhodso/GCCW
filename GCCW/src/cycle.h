#ifndef CYCLE_H
#define CYCLE_H

#include "gameobject.h"

class cycle : public gameObject
{
public:
    //Constructors
    cycle();
    cycle(int _colour, bool _isAI);
    cycle(int _colour, bool _isAI, float _x, float _y, float _z=0);

    //Deconstructor
    ~cycle();

    //Getters and setters
    void setColour(int _colour);
    int getColour();
    bool getActive();
    void setActive(bool _active);
    bool getIsAI();
    void setIsAI(bool _isAI);
    bool getLeftFlag();
    bool getRightFLag();
    void setLeftFlag(bool state);
    void setRightFlag(bool state);
    float getVelocity();
    void setVelocity(float _velocity);

    //Inherrited methods to override
    void doAI();
    void draw();

    //Other methods
    void moveCycle(bool accel);
    void updateODE();
    void turnCycle(int turnDir);
    void assignModel();
    void drawIndicator();

private:
    //Private vars
    int colour;
    bool active;
    bool isAI;
    bool leftFlag;
    bool rightFlag;
    float velocity;
    float accelForce = 0.1f;
};

/*
    Colour - Numer
    Blue - 0
    Red - 1
    Yellow - 2
*/

#endif // CYCLE_H
