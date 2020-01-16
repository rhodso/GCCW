#pragma once

#include <thread>
#include <future>
#include "ofMain.h"
#include "cycle.h"
#include "ofxAssimpModelLoader.h"
#include "wall.h"
#include "boundarywall.h"
#include "cyclewall.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void drawObjects();
    void updateCamera();
    void handleKeyPress();
    void collisions();
    void collide(gameObject obj1, gameObject obj2);

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
    vector<cycleWall> cycleWalls;
    vector<boundaryWall> boundaryWalls;
    ofPlanePrimitive groundPlane;
    cycle testCycle;
    boundaryWall bWallN;
    boundaryWall bWallE;
    boundaryWall bWallS;
    boundaryWall bWallW;

    //Camera stuff
    ofEasyCam playerCam;
    ofEasyCam overheadCam;
    gameObject* cameraObject;
    ofFbo fbo;
    int cameraState;

    //Vars
    int keyArray[65536];
    bool minimap;
    bool debugInfo;
    int winner;

    //Multithreading vars
    int collRes;
    bool b_res;
    bool b_resWatch;

    //Lighting
    ofLight testCycleLight;
    ofLight testCycleIndicatorLight;

};
