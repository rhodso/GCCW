#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ofxAssimpModelLoader.h"

class gameObject
{
public:
    //Constructor
    gameObject();
    gameObject(float _x, float _y, float _z=0);
    virtual ~gameObject();

    //Look and feel
    char* getModelFP();
    char* getTextureFP();

    void setModelFP(char* _modelFP);
    void setTextureFP(char* _textureFP);

    //Positioning
    float getX();
    float getY();
    float getZ();
    float getHeading();
    bool getDebugDraw();
    ofQuaternion* getRot();

    void setX(float _x);
    void setY(float _y);
    void setZ(float _z);
    void setHeading(float _heading);
    void setRot(ofQuaternion _rot);

    //Dimensions
    float getL();
    float getW();
    float getH();

    void setL(float _l);
    void setW(float _w);
    void setH(float _h);

    void setDebugDraw(bool _debugDraw);

    //Related functions
    virtual void draw();
    virtual void doAI();


protected:
    //position
    float x;
    float y;
    float z;
    float heading;
    ofQuaternion rot;

    //dimensions
    float l;
    float w;
    float h;

    //Filepath for model
    char* modelFP;
    char* textureFP;

    //Model
    ofxAssimpModelLoader model;

    bool debugDraw = false;

};

#endif // GAMEOBJECT_H
