#include "cyclewall.h"
#include "cycle.h"

cycleWall::cycleWall(){}
cycleWall::cycleWall(ofColor _color, cycle* c){
    this->color = _color;
}
cycleWall::~cycleWall(){}

void cycleWall::placeWallFromCoords(float _x, float _y, float _l, float _w){
    this->x = _x;
    this->y = _y;
    this->l = _l;
    this->w = _w;
    this->z = 0.0f;
    this->h = 1.0f;
}

void cycleWall::placeTrailingWall(cycle* c){

    float l = 0.0f;
    float w = 0.0f;
    float x = 0.0f;
    float y = 0.0f;

    switch((int) c->getHeading()){
        case 1:
            l = c->getTurnX() - c->getX();
            y = c->getY();
            x = (c->getX()+c->getTurnX())/2;
            w = 1.0f;
            break;
        case 2:
            w = c->getTurnY() - c->getY();
            x = c->getX();
            y = (c->getY() + c->getTurnY())/2;
            l = 1.0f;
            break;
        case 3:
            l = c->getTurnX() - c->getX();
            y = c->getY();
            y = c->getY();
            x = (c->getX()+c->getTurnX())/2;
            w = 1.0f;
            break;
        case 4:
            w = c->getTurnY() - c->getY();
            x = c->getX();
            y = (c->getY() + c->getTurnY())/2;
            l = 1.0f;
            break;
    }

    placeWallFromCoords(x, y, l, w);
}
void cycleWall::draw(){
    //ofSetColor(this->color,128);
    ofSetColor(ofColor::yellow, 128);
    //std::cout << "Box x,y,z,w,h,l: " << x << " " << y << " " << " " << 0 << " " << w << " " << h << " " << l << std::endl;
    ofDrawBox(x,y,0.5,l,w,h);

}
