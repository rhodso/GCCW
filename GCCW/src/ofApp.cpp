#include "ofApp.h"
#include "ofxGui.h"
#include "cycle.h"
#include <iostream>
#include <thread>

//static const dVector3 yunit = {0,1,0}, zunit = {0,0,1};

void ofApp::setup(){
    ofDisableArbTex();

    //UpVector
    ofVec3f upVector;
    upVector.set(0,0,1);

    //fbo
    fbo.allocate(300,300);
    fbo.begin();
    ofClear(0);
    fbo.end();

    //Minimap
    mm.setup();
    //mm.add()

    //Player camera
    playerCam.setNearClip(0.01);
    playerCam.setPosition(10,10,10);
    playerCam.lookAt({0,0,0},upVector);
    playerCam.setFarClip(99999.0);

    //Overhead camera
    overheadCam.setPosition(1,1,200);
    overheadCam.lookAt({0,0,0},upVector);
    overheadCam.setNearClip(0.01);
    overheadCam.setFarClip(99999.0);

    //Graphics ground plane
    groundPlane.set(10,10);
    groundPlane.mapTexCoords(0,0,5,5);
    groundPlane.setResolution(128,128);

    //testCycle
    //testCycle.setX(120);
    testCycle.setColour(2);
    testCycle.setZ(0.6f);
    testCycle.setActive(true);
    testCycle.setIsAI(false);
    testCycle.assignModel();
    testCycle.setDebugDraw(false);
    testCycle.setHeading(1);
}
void ofApp::update(){

    std::thread camThread(&ofApp::updateCamera, this);

    /*  Buttons
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

    //std::cout << "Cycle:\tX: " << testCycle.getX() << "\n\tY: " << testCycle.getY() << "\n\tZ: " << testCycle.getZ() << std::endl << std::endl;



    //-------overhead camera-------
    //Startup
    fbo.begin();
    ofClear(0);
    overheadCam.begin();
    ofEnableDepthTest();
    ofPushMatrix();

    //Background
    ofBackground(20);

    //Grid
    ofSetColor(ofColor::lightCyan);
    ofDrawGrid(1, 100, false, false, false, true);

    //Testcycle
    testCycle.draw();

    //Cleanup
    ofDisableDepthTest();
    overheadCam.end();
    ofPopMatrix();
    fbo.end();

    camThread.join();

}
void ofApp::draw(){
    //Need to draw everything twice in one thread because I can't
    //make openGL calls in anything but the main thread YAY

    //-------Player camera-------
    //Startup
    playerCam.begin();
    ofEnableDepthTest();
    ofPushMatrix();

    //Background
    ofBackground(20);

    //Grid
    ofSetColor(ofColor::lightCyan);
    ofDrawGrid(1, 100, false, false, false, true);

    //Testcycle
    testCycle.draw();

    //Cleanup
    ofDisableDepthTest();
    playerCam.end();
    ofPopMatrix();

    fbo.draw(ofGetWidth()/2,ofGetHeight()/2);

}
void ofApp::keyPressed(int key){ keyArray[key] = 1; }
void ofApp::keyReleased(int key){ keyArray[key] = 0; }
void ofApp::updateCamera(){
    //Update player camera here
    //TODO

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
