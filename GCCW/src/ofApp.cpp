#include "ofApp.h"
#include "cycle.h"
#include <iostream>
#include <thread>

//static const dVector3 yunit = {0,1,0}, zunit = {0,0,1};

void ofApp::setup(){
    ofDisableArbTex();

    //Camera
    ofVec3f upVector;
    upVector.set(0,0,1);
    cam.setAutoDistance(false);
    cam.setNearClip(0.01);
    cam.setPosition(10,10,10);
    cam.lookAt({0,0,0},upVector);
    cam.setUpAxis(upVector);
    cam.setFarClip(99999.0);

    //Graphics ground plane
    groundPlane.set(8,8);
    groundPlane.mapTexCoords(0,0,4,4);
    groundPlane.setResolution(128,128);

    //testCycle
    testCycle.setColour(2);
    testCycle.setZ(0.6f);
    testCycle.setActive(true);
    testCycle.setIsAI(false);
    testCycle.assignModel();
    testCycle.setDebugDraw(false);
    gameObjects.push_back(testCycle);
}
void ofApp::update(){

    //Update camera in a new thread
    std::thread camThread(&updateCamera, this);

    /*
        w = 119
        a = 97
        s = 115
        d = 100
        q = 113
        UpArrow = 57357
        DownArrow = 57359
        LeftArrow = 57356
        RightArrow = 57358
    */

    //Quit if q is pressed
    if(keyArray[113] == 1){
        ofExit();
    }
    if(keyArray[97] == 1 && !testCycle.getLeftFlag()){
        //If left is pressed, turn left and then don't turn left again
        testCycle.setLeftFlag(true);
        testCycle.turnCycle(1);
    }
    else if (keyArray[100] == 1 && !testCycle.getRightFLag()){
        //If right is pressed, tutn right and don't turn right again
        testCycle.setRightFlag(true);
        testCycle.turnCycle(2);
    }
    if(testCycle.getLeftFlag() && keyArray[97] == 0){
        //Reset left flag when left is not pressed
        testCycle.setLeftFlag(false);
    }
    if(testCycle.getRightFLag() && keyArray[100] == 0){
        //Reset right flag when right is not pressed
        testCycle.setRightFlag(false);
    }
    if (keyArray[119]){
        testCycle.moveCycle(true);
    }
    if(!keyArray[119]){
        testCycle.moveCycle(false);
    }

    //Make sure the camera has finished
    camThread.join();
}
void ofApp::draw(){
    //Startup
    cam.begin();
    ofEnableDepthTest();
    ofPushMatrix();

    //Background
    ofBackground(20);

    //Grid
    ofSetColor(ofColor::lightCyan);
    ofDrawGrid(1.25, 100, false, false, false, true);

    //Testcycle
    testCycle.draw();

    //Cleanup
    ofDisableDepthTest();
    cam.end();
    ofPopMatrix();
}
void ofApp::keyPressed(int key){ keyArray[key] = 1; }
void ofApp::keyReleased(int key){ keyArray[key] = 0; }

void ofApp::updateCamera(){
    for(gameObject g : gameObjects){
        //Find the object the camera is locked to, and process the camera
        if(g.getDoFollowCam()){
            g.doCamera(cam);
            break;
        }
    }
}

//Unused
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
