#include "cyclewall.h"
#include "cycle.h"

cycleWall::cycleWall(){}
cycleWall::cycleWall(ofColor _color, cycle* c){
    this->color = _color;
    this->placeWall(c);
}
cycleWall::~cycleWall(){}

void cycleWall::placeWall(cycle* c){
    ofVec3f cCenter = c->getCenter();
    int cycleHeading = (int) c->getHeading();

    this->heading = cycleHeading;

    switch(cycleHeading){
        case 1:
            this->x = cCenter.x;
            this->x += 0.45;
            this->y = cCenter.y;
            this->y -= 0.37;
            this->w = cCenter.x - c->getLastX();
            if(this->w < 0){ this->w *= -1; }
            this->h = 0.5f;
            this->l = 1.0;
            break;
        case 2:
            this->x = cCenter.x;
            this->x -= 1.442;
            this->y = cCenter.y;
            this->y += 1.5;
            this->l = cCenter.x - c->getLastX();
            if(this->l < 0){ this->l *= -1; }
            this->w = 0.5f;
            this->h = 1.0;
            break;
        case 3:
            this->x = cCenter.x;
            this->x -= 3.75;
            this->y = cCenter.y;
            this->y -= 0.45;
            this->w = cCenter.x - c->getLastX();
            if(this->w < 0){ this->w *= -1; }
            this->h = 0.5f;
            this->l = 1.0;
            break;
        case 4:
            this->x = cCenter.x;
            this->x -= 1.422;
            this->y = cCenter.y;
            this->y -= 2.35;
            this->l = cCenter.x - c->getLastX();
            if(this->l < 0){ this->l *= -1; }
            this->w = 0.5f;
            this->h = 1.0;
            break;
    }
}

void cycleWall::draw(){
    ofSetColor(this->color,128);
    //std::cout << "Box x,y,z,w,h,l: " << x << " " << y << " " << " " << 0 << " " << w << " " << h << " " << l << std::endl;
    ofDrawBox(x,y,0.6,w,h,l);
}
