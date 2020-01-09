#include "boundarywall.h"
#include "ofMain.h"

//Constructors
boundaryWall::boundaryWall(){
    //WallPlane
    this->wallPlane.set(100,2);
    this->wallPlane.mapTexCoords(0,0,5,5);
    this->wallPlane.setResolution(128,128);

    //Set constant things
    this->l = 200.0f;
    this->w = 0.01;
    this->h = 2.0f;
}

//Destructors
boundaryWall::~boundaryWall(){}

//Getters and setters
int boundaryWall::getBoundary(){ return boundary; }
void boundaryWall::setBoundary(int _boundary){ boundary = _boundary; }

//Inherited methods
void boundaryWall::draw(){

    //Setup
    ofPushMatrix();
    ofSetColor(this->color);
    //ofRotateDeg(90*(this->heading-1));
    /*
        1 - West
        2 - North
        3 - East
        4 - South
    */

    switch(boundary){
        case 1:
            ofDrawBox(0,-100,1,l,w,h);
            break;
        case 2:
            ofDrawBox(-100,-0,1,w,l,h);
            break;
        case 3:
            ofDrawBox(0,100,1,l,w,h);
            break;
        case 4:
            ofDrawBox(100,0,1,w,l,h);
            break;
    }
    ofPopMatrix();

}

//Unnessesary
void boundaryWall::doAI(){}
