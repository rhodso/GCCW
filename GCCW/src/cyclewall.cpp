#include "cyclewall.h"
#include "wall.h"
#include "cycle.h"

//Constructor
cycleWall::cycleWall(){
    setup();
}
cycleWall::cycleWall(ofColor _color){
    this->color = _color;
    setup();
}

//Destructor
cycleWall::~cycleWall(){}

//Setup
void cycleWall::setup(){
    this->l = 0.1;
    this->w = 0.3;
    this->h = 1.25;
}

//Inherited methods
void cycleWall::draw(){
    ofSetColor(this->color, 128);
    ofDrawBox(x,y,z,l,h,w);
}

//Other methods
void cycleWall::placeWall(cycle* theCycle){
    ofVec3f center = theCycle->getCenter();

}
