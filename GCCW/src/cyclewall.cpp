#include "cyclewall.h"
#include "cycle.h"

cycleWall::cycleWall(){}
cycleWall::cycleWall(ofColor _color, cycle c){
    this->color = _color;
    this->placeWall(c);
}
cycleWall::~cycleWall(){}

void cycleWall::placeWall(cycle c){
    ofVec3f cCenter = c.getCenter();
    int cycleHeading = (int) c.getHeading();

    switch(cycleHeading){
        case 1:
            this->x = cCenter.x;
            this->y = cCenter.y;
            this->l = cCenter.z - c.getLastX();
            if(this->l < 0){ this->l *= -1; }
            this->w = 0.3;
            this->h = 1.0;
            break;
        case 2:
            this->x = c.getX();
            this->y = c.getY();
            this->l = c.getX() - c.getLastX();
            if(this->l < 0){ this->l *= -1; }
            this->w = 0.3;
            this->h = 1.0;
            break;
        case 3:
            this->x = c.getX();
            this->y = c.getY();
            this->l = c.getX() - c.getLastX();
            if(this->l < 0){ this->l *= -1; }
            this->w = 0.3;
            this->h = 1.0;
            break;
        case 4:
            this->x = c.getX();
            this->y = c.getY();
            this->l = c.getX() - c.getLastX();
            if(this->l < 0){ this->l *= -1; }
            this->w = 0.3;
            this->h = 1.0;
            break;
    }
}

void cycleWall::draw(){
    ofSetColor(this->color,128);
    std::cout << "Box x,y,z,w,h,l: " << x << " " << y << " " << " " << 0 << " " << w << " " << h << " " << l << std::endl;
    ofDrawBox(x,y,0,w,h,l);
}
