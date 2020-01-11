#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ofMain.h"
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
    bool getDoFollowCam();
    int getObjectType();

    void setX(float _x);
    void setY(float _y);
    void setZ(float _z);
    void setHeading(float _heading);
    void setRot(ofQuaternion _rot);
    void setDoFollowCam(bool _doFollowCam);
    void setObjectType(int _objectType);

    //Dimensions
    float getL();
    float getW();
    float getH();
    float getLongestDim();

    void setL(float _l);
    void setW(float _w);
    void setH(float _h);

    ofVec3f getCenter();

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
    float longestDim;
    void setLongestDim();

    //Filepath for model
    char* modelFP;
    char* textureFP;

    //Model
    ofxAssimpModelLoader model;

    //Other vars
    bool debugDraw = false;
    bool doFollowCam = false;
    static int followCamCount;
    int objectType;
};

/*
 * 0 - Cycle
 * 1 - boundaryWall
 * 2 -
*/

#endif // GAMEOBJECT_H
