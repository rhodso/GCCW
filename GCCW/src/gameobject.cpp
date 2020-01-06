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

//Dimensional methods
float gameObject::getL(){ return l;}
float gameObject::getW(){ return w;}
float gameObject::getH(){ return h; }

void gameObject::setL(float _l){ l = _l; }
void gameObject::setW(float _w){ w = _w; }
void gameObject::setH(float _h){ h = _h; }

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

//Other methods that don't do anything, they're just here so they're inherited
void gameObject::draw(){}
void gameObject::doAI(){}
void gameObject::doCamera(ofEasyCam){}
