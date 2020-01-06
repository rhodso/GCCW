#include "cycle.h"
#include "gameobject.h".h"
#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

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
    if(modelFP) delete [] modelFP;
    if(textureFP) delete [] textureFP;
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

//Inherited methods
void cycle::doAI(){
    //Only do stuff if the player is bot.
    if(this->isAI){
        //Do the AI processing here

    }
}
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

void cycle::turnCycle(int turnDir){

    /*  Heading:
        1 = North
        2 = East
        3 = South
        4 = West
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

    std::cout << "Heading: " << this->heading << std::endl;

}

void cycle::moveCycle(bool accel){
    int tempHead = (int) this->heading;

    //Apply acceleration to the speed
    if(accel){
        velocity += accelForce;
        if(velocity > 5){ //Cap velocity at 5u/frame
            velocity = 5;
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

    //this->x += velocity;

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

