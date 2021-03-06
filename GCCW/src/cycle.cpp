#include "cycle.h"
#include "gameobject.h"
#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

#include <time.h>
#include <cstring>

//Constructors
cycle::cycle(){}
cycle::cycle(int _colour, bool _isAI){
    colour = _colour;
    isAI = _isAI;
    this->setColour(colour);
    this->setHeading(1);
}
cycle::cycle(int _colour, bool _isAI, float _x, float _y, float _z){
    colour = _colour;
    isAI = _isAI;
    x = _x;
    y = _y;
    z = _z;
    this->setColour(colour);
    this->heading = 1;
    this->turnCycle(1);
}

//Destructor
cycle::~cycle(){
    /*
    if(modelFP) delete [] modelFP;
    if(textureFP) delete [] textureFP;
    */
}

//Getters and setters
void cycle::setColour(int _colour){
    colour = _colour;
    switch(colour){
        case 0:
            this->setModelFP("Light_Cycle_B.dae");
            break;
        case 1:
            this->setModelFP("Light_Cycle_R.dae");
            break;
        case 2:
            this->setModelFP("Light_Cycle_Y.dae");
            break;
    }
}
int cycle::getColour(){ return colour; }
bool cycle::getActive(){ return active; }
void cycle::setActive(bool _active) {active = _active;}
bool cycle::getIsAI(){ return isAI; }
void cycle::setIsAI(bool _isAI){ isAI = _isAI; }
bool cycle::getLeftFlag(){ return leftFlag; }
bool cycle::getRightFLag(){ return rightFlag; }
void cycle::setLeftFlag(bool state){ leftFlag = state; }
void cycle::setRightFlag(bool state){ rightFlag = state; }
float cycle::getVelocity(){ return velocity; }
void cycle::setVelocity(float _velocity){ velocity = _velocity; }
float cycle::getLastX(){ return lastX; }
float cycle::getLastY(){ return lastY; }
void cycle::updateLastX(){ this->lastX = this->x; }
void cycle::updateLastY(){ this->lastY = this->y; }
float cycle::getTurnX(){ return this->turnX; }
float cycle::getTurnY(){ return this->turnY; }
void cycle::updateTurnCoords(){
    this->turnX = this->x;
    this->turnY = this->y;
}
void cycle::placeWallFromTurn(float* posDim){

    cycleWall placedWall = cycleWall();

    float offset = 1.5;

    float l = 0.0f;
    float w = 0.0f;
    float x = 0.0f;
    float y = 0.0f;

    switch((int) this->getHeading()){
        case 1:
            l = this->getTurnX() - this->getX();
            y = this->getY();
            x = (this->getX()+this->getTurnX())/2;
            x += offset;
            w = 0.0f;
            break;
        case 2:
            w = this->getTurnY() - this->getY();
            x = this->getX();
            y = (this->getY() + this->getTurnY())/2;
            y += offset;
            l = 0.0f;
            break;
        case 3:
            l = this->getTurnX() - this->getX();
            y = this->getY();
            y = this->getY();
            x = (this->getX()+this->getTurnX())/2;
            x -= offset;
            w = 0.0f;
            break;
        case 4:
            w = this->getTurnY() - this->getY();
            x = this->getX();
            y = (this->getY() + this->getTurnY())/2;
            y -= offset;
            l = 0.0f;
            break;
    }

    placedWall.placeWallFromCoords(x,y,l,w);

    posDim[0] = x;
    posDim[1] = y;
    posDim[2] = l;
    posDim[3] = w;

}
void cycle::setTarget(int _target){ this->target = _target; }
void cycle::setCurrent(int _current){ this->current = _current; }
int cycle::getTarget(){ return this->target; }
int cycle::getCurrent(){ return this->current; }

//Inherited methods
void cycle::doAI(){}
void cycle::draw(){
    //Update OF position
    float rotationAmount;
    ofVec3f rotationAngle;

    //rot->getRotate(rotationAmount, rotationAngle);
    rot.getRotate(rotationAmount, rotationAngle);

    //Do rotation of the model
    float tempRot = (90 * (this->heading - 1));
    rotationAmount = tempRot;

    if(this->debugDraw){ //Draws a translucent blox that represents the hitbox of the object
        //Setup
        ofSetColor(ofColor::white, 128);
        ofPushMatrix();

        //Draw the box
        ofTranslate(this->x, this->y, this->z);
        ofRotateDeg(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
        ofDrawBox(this->l, this->w, this->h);

        //Cleanup
        ofSetColor(ofColor::white, 255);
        ofPopMatrix();
    }

    //Draw model
    ofPushMatrix();
    ofTranslate(this->x, this->y, (this->z)-2.45);

    //Offset the model based on heading
    switch((int) this->getHeading()){
        case 1:
            ofTranslate(-6.275, 0, 0);
            break;
        case 2:
            ofTranslate(0, -6.275, 0);
            break;
        case 3:
            ofTranslate(6.275, 0, 0);
            break;
        case 4:
            ofTranslate(0, 6.275, 0);
            break;
    }

    //Rotate model to correct orientation (from heading), then draw, then pop matrix
    ofRotateDeg(rotationAmount, (rotationAngle.x), (rotationAngle.y), (rotationAngle.z+180));
    this->model.drawFaces();
    ofPopMatrix();
}
void cycle::drawIndicator(){
    //Update OF position
    float rotationAmount;
    ofVec3f rotationAngle;

    //rot->getRotate(rotationAmount, rotationAngle);
    rot.getRotate(rotationAmount, rotationAngle);

    //Do rotation of the model
    float tempRot = (90 * (this->heading - 1));
    rotationAmount = tempRot;

    //Draws a translucent blox that represents the hitbox of the object
    //Setup
    ofSetColor(ofColor::orange);
    ofPushMatrix();

    //Draw the indicator
    //ofTranslate(this->x, this->y, this->z);
    ofDrawSphere(this->indX,this->indY,this->indZ,this->longestDim+2);

    //Cleanup
    ofSetColor(ofColor::white, 255);
    ofPopMatrix();

}
void cycle::updateIndicatorPosition(float camX, float camY, float camZ){
    //Works for CameraZ = 200
        //this->indX = (this->x*0.95285);
        //this->indY = (this->y*0.95285);

        float indicatorZ = 10;
        ofVec3f cycleCenter = this->getCenter();

        /*
         * General formula to work out the position of the player indicator
         * Given the position of the player and the camera. This needs to be
         * done because of a sort of paralax effect when the indicator was
         * positioned exactly over the camera. This means that we have to do
         * some maths to work this out. Hence, updating the position and drawing
         * is split into seperate functions, so that this can be updated in
         * ofApp's update() function (Where it is also assigned to a different
         * thread for performance) and then it is later drawn in the draw()
         * function. Credit to Maria for helping with maths because I got
         * an A in GCSE maths and then a U at A-level.
        */

        this->indX = (((indicatorZ-cycleCenter.z)*(camX-cycleCenter.x))/(camZ-cycleCenter.z)) + cycleCenter.x;
        this->indY = (((indicatorZ-cycleCenter.z)*(camY-cycleCenter.y))/(camZ-cycleCenter.z)) + cycleCenter.y;
        this->indZ = indicatorZ;
}
void cycle::turnCycle(int turnDir){

    /*  Heading:
        1 = West
        2 = North
        3 = East
        4 = South
    */

    if(turnDir == 1) { //Left
        this->heading += 1;
    }
    else if(turnDir == 2){ //Right
        this->heading -= 1;
    }

    //Wraparound heading
    if(this->heading < 1){
        this->heading = 4;
    } else if(this->heading > 4){
        this->heading = 1;
    }
}
void cycle::moveCycle(bool accel){
    int tempHead = (int) this->heading;

    //Apply acceleration to the speed
    if(accel){
        velocity += accelForce;
        if(velocity > 0.5){ //Cap velocity at 0.5u/frame
            velocity = 0.5;
        }
    }
    else{ //If no btton is being pressed, declerate
        velocity *= (1-accelForce);
    }

    //Setting x/y based on heading
    switch(tempHead){
            case 1:
                this->x -= velocity;
                break;
            case 2:
                this->y -= velocity;
                break;
            case 3:
                this->x += velocity;
                break;
            case 4:
                this->y += velocity;
                break;
        }

}
void cycle::assignModel(){

    debugDraw = true;

    //Setup model
    this->model.loadModel(this->modelFP, true);
    float scale = 1.0f/ this->model.getNormalizedScale();
    //scale = scale*2;
    this->model.setScale(scale, scale, scale);
    this->model.setRotation(0, rot[1], rot[2], rot[3], 0);

    //Setup texture


    //Setup HitBox
    this->l = 2.883;
    this->w = 0.83;
    this->h = 1.25;
}

