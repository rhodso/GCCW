//Include the header file, and cstring for string handling
#include "gameobject.h"
#include <cstring>

int gameObject::followCamCount = 0;

//Constructor
gameObject::gameObject(){
    x = y = z = 0;
}

//Constructor with X, Y, and option for Z
gameObject::gameObject(float _x, float _y, float _z){
    x = _x;
    y = _y;
    z = _z;
}

//Deconstructor
gameObject::~gameObject(){
    if(modelFP) delete [] modelFP;
    if(textureFP) delete [] textureFP;
}

//Positional methods
float gameObject::getX(){ return x; }
float gameObject::getY(){ return y; }
float gameObject::getZ(){ return z; }
float gameObject::getHeading(){ return heading;}
ofQuaternion* gameObject::getRot(){ return &rot; }
bool gameObject::getDebugDraw(){ return debugDraw; }
bool gameObject::getDoFollowCam(){ return doFollowCam; }
int gameObject::getObjectType(){ return objectType; }

void gameObject::setX(float _x){ x = _x; }
void gameObject::setY(float _y){ y = _y; }
void gameObject::setZ(float _z){ z = _z; }
void gameObject::setHeading(float _heading){ heading = _heading; }
void gameObject::setRot(ofQuaternion _rot){
    for (int i = 0; i < 4; i++) {
        rot[i] = _rot[i];
    }
}
void gameObject::setDebugDraw(bool _debugDraw){ debugDraw = _debugDraw; }
void gameObject::setDoFollowCam(bool _doFollowCam){
    if(_doFollowCam){
        if(followCamCount == 0){
            doFollowCam = _doFollowCam;
            followCamCount += 1;
        }
    }
    else{
        if(followCamCount > 0){
            doFollowCam = _doFollowCam;
            followCamCount -= 1;
        }
    }
}
void gameObject::setObjectType(int _objectType){ objectType = _objectType; }

//Dimensional methods
float gameObject::getL(){ return l;}
float gameObject::getW(){ return w;}
float gameObject::getH(){ return h; }
float getLongestDim();

void gameObject::setL(float _l){ l = _l; this->setLongestDim();}
void gameObject::setW(float _w){ w = _w; this->setLongestDim(); }
void gameObject::setH(float _h){ h = _h; this->setLongestDim(); }

ofVec3f gameObject::getCenter(){
    ofVec3f centerPoint = {0,0,0};

    centerPoint.x = this->getX() + (this->getL()/2);
    centerPoint.y = this->getY() + (this->getW()/2);
    centerPoint.z = this->getZ() + (this->getH()/2);

    return centerPoint;
}

//Filepaths for Model and Texture
char* gameObject::getModelFP(){ return modelFP; }
char* gameObject::getTextureFP(){ return modelFP; }
void gameObject::setModelFP(char* _modelFP){
    //Delete modelFP, and recreate
    if (modelFP) delete [] modelFP;
    modelFP= new char[std::strlen(_modelFP)+1];
    strcpy(modelFP,_modelFP);
}

void gameObject::setTextureFP(char* _textureFP){
    //Delete textureFP and recreate
    if (textureFP) delete [] textureFP;
    textureFP= new char[std::strlen(_textureFP)+1];
    strcpy(textureFP,_textureFP);
}

void gameObject::setLongestDim(){
    if(l >= w){ //length is bigger than or equal to width
        if(l >=h){ //Length is bigger than or equal to height
            longestDim = l;
        } else{ //Length is bigger than or equal to width, but smaller than height
            longestDim = h;
        }
    } else if (w > l){ //Width is bigger than length
        if(w >= h){ //Width is bigger than or equal to height
            longestDim = w;
        } else{ //Height is bigger than width
            longestDim = h;
        }
    }
}

//Other methods that don't do anything, they're just here so they're inherited
void gameObject::draw(){}
void gameObject::doAI(){}
