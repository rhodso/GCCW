#pragma once

#include "ofMain.h"
#include "cycle.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void updateCamera();
    void handelKeyPress();
    void collisions();
    int collide(gameObject obj1, gameObject obj2);

    //Unused
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


private:
    //Objects
    vector<gameObject> objects;
    ofPlanePrimitive groundPlane;
    cycle testCycle;

    //Camera stuff
    ofCamera playerCam;
    ofCamera overheadCam;
    gameObject* cameraObject;
    ofFbo fbo;

    //Vars
    int keyArray[65536];
    bool minimap;

    ofxPanel mm;
    ofImage map;

};
