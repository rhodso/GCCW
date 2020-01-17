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
    virtual ~cycle();

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
    float getLastX();
    void updateLastX();
    float getLastY();
    void updateLastY();

    //Inherrited methods to override
    void doAI();
    void draw();

    //Other methods
    void moveCycle(bool accel);
    void updateODE();
    void turnCycle(int turnDir);
    void assignModel();
    void drawIndicator();
    void updateIndicatorPosition(float camX, float camY, float camZ);

private:
    //Private vars
    int colour;
    bool active;
    bool isAI;
    bool leftFlag;
    bool rightFlag;
    float velocity;
    float accelForce = 0.1f;

    float lastX;
    float lastY;

    float indX;
    float indY;
    float indZ;

    int target = 20;
    int current = 0;
};

/*
    Colour - Numer
    Blue - 0
    Red - 1
    Yellow - 2
*/

#endif // CYCLE_H
