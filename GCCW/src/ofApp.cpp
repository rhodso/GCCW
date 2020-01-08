#include "ofApp.h"
#include "ofxGui.h"
#include "cycle.h"
#include <iostream>
#include <thread>

//static const dVector3 yunit = {0,1,0}, zunit = {0,0,1};

void ofApp::setup(){
    ofDisableArbTex();

    minimap = false;

    //UpVector
    ofVec3f upVector;
    upVector.set(0,0,1);

    //fbo
    fbo.allocate(250,250);
    fbo.begin();
    ofClear(0);
    fbo.end();

    //Minimap
    mm.setup();

    //Player camera
    cameraState = 0;
    playerCam.disableMouseInput();
    playerCam.setNearClip(0.01);
    playerCam.setPosition(10,10,10);
    playerCam.lookAt({0,0,0},upVector);
    playerCam.setFarClip(99999.0);

    //Overhead camera
    overheadCam.disableMouseInput();
    overheadCam.setPosition(1,1,200);
    overheadCam.lookAt({0,0,0},upVector);
    overheadCam.setOrientation({90,0,0,0});
    overheadCam.setNearClip(0.01);
    overheadCam.setFarClip(99999.0);

    //Graphics ground plane
    groundPlane.set(100,100);
    groundPlane.mapTexCoords(0,0,5,5);
    groundPlane.setResolution(128,128);

    //testCycle
    //testCycle.setX(120);
    testCycle.setColour(2);
    testCycle.setZ(0.6f);
    testCycle.setActive(true);
    testCycle.setIsAI(false);
    testCycle.assignModel();
    testCycle.setDebugDraw(true);
    testCycle.setHeading(1);

    //Update cameraObject
    cameraObject = &testCycle;

    //Lighting
    testCycleLight.setPosition(0,0,2);
    testCycleLight.lookAt({0,0,0});
    testCycleLight.setAmbientColor(ofColor::blue);
    testCycleLight.enable();

    testCycleIndicatorLight.setPosition(0,0,2);
    testCycleIndicatorLight.lookAt({0,0,0});
    testCycleIndicatorLight.setAmbientColor(ofColor::blue);
    testCycleIndicatorLight.enable();
}
void ofApp::update(){

    //Update camera, keypresses, and collisions in a seperate thread
    //std::thread camThread(&ofApp::updateCamera, this);
    //std::thread keyPressThread(&ofApp::handleKeyPress, this);
    std::thread collionsMainThread(&ofApp::collisions, this);

    updateCamera();
    handleKeyPress();
    /*
    collisions();
    */

    testCycleLight.setPosition(testCycle.getX(), testCycle.getY(), (testCycle.getZ() + 2));
    testCycleLight.lookAt({testCycle.getX(), testCycle.getY(), testCycle.getZ()});
    testCycleIndicatorLight.setPosition(testCycle.getX(), testCycle.getY(), (testCycle.getZ() + 15));
    testCycleIndicatorLight.lookAt({testCycle.getX(), testCycle.getY(), testCycle.getZ()});

    //std::cout<<"TestCycle:\nX:\t" << testCycle.getX() << "\nY:\t" << testCycle.getY() << std::endl << std::endl;

    if(minimap){//-------overhead camera-------
        //Startup
        fbo.begin();
        ofClear(0);
        overheadCam.begin();
        ofPushMatrix();
        ofEnableDepthTest();

        drawObjects();

        //Cleanup
        overheadCam.end();
        fbo.end();
    }

    //Make sure the threads have finished
    //camThread.join();
    //keyPressThread.join();
    collionsMainThread.join();

}
void ofApp::draw(){
    //Need to draw everything twice in one thread because I can't
    //make openGL calls in anything but the main thread YAY

    //-------Player camera-------
    //Startup
    playerCam.begin();
    ofEnableDepthTest();

    drawObjects();

    //Cleanup
    ofDisableDepthTest();

    playerCam.end();
    //ofPopMatrix();

    if(minimap){
        //Minimap area
        //ofSetColor(ofColor::white);
        //ofDrawRectangle(0,0, 320, 320);

        //Draw the minimap
        fbo.draw(0,0);
    }
}
void ofApp::drawObjects(){
    //Background
    ofBackground(20);

    //Grid
    ofSetColor(ofColor::lightCyan);
    ofDrawGrid(1, 100, false, false, false, true);

    //playerIndicator
    testCycle.drawIndicator();

    //Testcycle
    testCycle.draw();
}
void ofApp::keyPressed(int key){ keyArray[key] = 1; /*std::cout << key << std::endl; */}
void ofApp::keyReleased(int key){ keyArray[key] = 0; }
void ofApp::updateCamera(){

    //Get a vector for the camera, and an ofVec3f for the position to look at
    float camPos[3];
    ofVec3f pps;
    int len;

    //Get the XYZ
    camPos[0] = pps.x = cameraObject->getX();
    camPos[1] = pps.y = cameraObject->getY();
    camPos[2] = pps.z = cameraObject->getZ();

    if (cameraState == 1){
        camPos[2] += 2;
        len = 3;
        switch((int) cameraObject->getHeading()){
            case 1:
                camPos[1] += len;
                break;
            case 2:
                camPos[0] += len;
                break;
            case 3:
                camPos[1] -= len;
                break;
            case 4:
                camPos[0] -= len;
                break;
        }

    } else if (cameraState == -1){
        camPos[2] += 2;
        len = 3;
        switch((int) cameraObject->getHeading()){
            case 1:
                camPos[1] -= len;
                break;
            case 2:
                camPos[0] -= len;
                break;
            case 3:
                camPos[1] += len;
                break;
            case 4:
                camPos[0] += len;
                break;
        }

    } else{
        //Add 2 to height, and set behind offset to 7
        camPos[2] += 3;
        len = 7;

        //Find what direction to add the offset in
        switch((int) cameraObject->getHeading()){
            case 1:
                camPos[0] += len;
                break;
            case 2:
                camPos[1] += len;
                break;
            case 3:
                camPos[0] -= len;
                break;
            case 4:
                camPos[1] -= len;
                break;
        }
    }



    //Set the camera's position, and set where the camera looks at
    playerCam.setPosition(camPos[0], camPos[1], camPos[2]);
    playerCam.lookAt(pps, {0,0,1});

}
void ofApp::handleKeyPress(){
    /*  Buttons
        w = 119
        a = 97
        s = 115
        d = 100
        q = 113
        m = 109
        UpArrow = 57357
        DownArrow = 57359
        LeftArrow = 57356
        RightArrow = 57358
    */

    //Quit if q is pressed
    if(keyArray[113] == 1){
        ofExit();
    }
    if(keyArray[109] == 1){
        minimap = !minimap;
        keyArray[109] = 0;
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
    //LeftArrow = 57356
    //RightArrow = 57358
    if(keyArray[57356] == 1){
        cameraState = -1;
    } else if(keyArray[57358] == 1){
        cameraState = 1;
    } else {
        cameraState = 0;
    }
}
void ofApp::collisions(){
    //TODO

    //Get a list of all current gameObjects that collisions need to be processed for
    //Assign collision pairs
    //Create a thread to see if they collide
    //Get the result

}

/* What the collision result means
    0 - Not colliding
    1 - Object 1
    2 - Object 2
*/
/*
int ofApp::collide(gameObject obj1, gameObject obj2){

}
*/

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
