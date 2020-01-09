#include "wall.h"
#include "ofMain.h"

//Constructors
wall::wall(){
    this->color = ofColor::white;
}
wall::wall(ofColor _color){
    this->color = _color;
}

//Deconstructor
wall::~wall(){}

//Getters and setters
void wall::setColor( ofColor _color){ color = _color; }
ofColor wall::getColor(){ return color; }

//Inherited methods
void wall::draw(){}
void wall::doAI(){}

