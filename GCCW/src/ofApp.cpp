#include "ofApp.h"
#include "ofxGui.h"
#include "cycle.h"
#include <iostream>
#include <thread>

//static const dVector3 yunit = {0,1,0}, zunit = {0,0,1};

void ofApp::setup(){

    //Set some vars
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
    testCycle.setDebugDraw(false);
    testCycle.setHeading(1);

    //Update cameraObject
    cameraObject = &testCycle;

    //BoundaryWalls
    bWallN.setBoundary(1);
    bWallN.setColor(ofColor::red);
    bWallE.setBoundary(2);
    bWallE.setColor(ofColor::blue);
    bWallS.setBoundary(3);
    bWallS.setColor(ofColor::green);
    bWallW.setBoundary(4);
    bWallW.setColor(ofColor::yellow);

    //Lighting
    testCycleLight.setPosition(0,0,2);
    testCycleLight.lookAt({0,0,0});
    testCycleLight.setAmbientColor(ofColor::blue);
    testCycleLight.enable();

    //Test cycle indicator light so that it doesn't look dark on the minimap
    testCycleIndicatorLight.setPosition(0,0,2);
    testCycleIndicatorLight.lookAt({0,0,0});
    testCycleIndicatorLight.setAmbientColor(ofColor::blue);
    testCycleIndicatorLight.enable();
}
void ofApp::update(){
    //Update camera collisions in a seperate thread, because efficiency
    std::thread collionsMainThread(&ofApp::collisions, this);

    //Update camera and handle keypresses in the main thread
    ofVec3f cameraPos = overheadCam.getPosition();
    testCycle.updateIndicatorPosition(cameraPos.x, cameraPos.y, cameraPos.z);
    updateCamera();
    handleKeyPress();

    //Add walls
    std::cout << "TestCycle vars X/LX/Y/LY: " << testCycle.getX() << " " << testCycle.getLastX() << " " << testCycle.getY() << " " << testCycle.getLastY() << std::endl;
    if((testCycle.getX() != testCycle.getLastX())||(testCycle.getY() != testCycle.getLastY())){
        ofColor newColor;
        switch(testCycle.getColour()){
            case 0:
                newColor = ofColor::blue;
                break;
            case 1:
                newColor = ofColor::red;
                break;
            case 2:
                newColor = ofColor::yellow;
                break;
        }
        cycleWalls.push_back(cycleWall(newColor, testCycle));
        std::cout << "Placed new wall" << std::endl;
    }

    //std::cout<<"TestCycle:\nX:\t" << testCycle.getX() << "\nY:\t" << testCycle.getY() << std::endl << std::endl;

    if(minimap){//-------overhead camera-------
        //Startup
        fbo.begin();
        ofClear(0);
        overheadCam.begin();

        //Draw the objects in the main thread because I can't
        //make OpenGL calls in anything but the main thread
        drawObjects();

        //Cleanup
        overheadCam.end();
        fbo.end();
    }

    //Update lights
    testCycleLight.setPosition(testCycle.getX(), testCycle.getY(), (testCycle.getZ() + 2));
    testCycleLight.lookAt({testCycle.getX(), testCycle.getY(), testCycle.getZ()});
    testCycleIndicatorLight.setPosition(testCycle.getX(), testCycle.getY(), (testCycle.getZ() + 15));
    testCycleIndicatorLight.lookAt({testCycle.getX(), testCycle.getY(), testCycle.getZ()});

    //Make sure the threads have finished
    collionsMainThread.join();

    //Update cycle lastX/Y
    testCycle.updateLastX();
    testCycle.updateLastY();

}
void ofApp::draw(){
    //Need to draw everything twice in one thread because I can't
    //make openGL calls in anything but the main thread YAY

    //-------Player camera-------
    //Startup
    playerCam.begin();
    ofEnableDepthTest();

    //Draw the objects in the main thread
    drawObjects();

    //Cleanup
    ofDisableDepthTest();
    playerCam.end();

    if(minimap){ //Draw the minimap
        fbo.draw(0,0);
    }
}
void ofApp::drawObjects(){
    //Background
    ofBackground(20);

    //Grid
    ofSetColor(ofColor::lightCyan);
    ofDrawGrid(1, 100, false, false, false, true);

    //Walls
    bWallN.draw();
    bWallE.draw();
    bWallS.draw();
    bWallW.draw();

    //CycleWalls
    for(cycleWall w : cycleWalls){
        std::cout << "Drawing wall" << std::endl;
        w.draw();
    }

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
    camPos[2] = pps.z = cameraObject->getZ(); //Add 2 to Z for perspective

    //Set len for camera X or Y offset, and set camPos[2] for Z offset
    len = 3;
    camPos[2] += 2;

    if (cameraState == 1){ //Looking Right, so move camera to the left
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

    } else if (cameraState == -1){ //Looking left so move the camera to the right
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

    } else if (cameraState == 2){ //Looking behind so move camera in front
        switch((int) cameraObject->getHeading()){
            case 1:
                camPos[0] -= len;
                break;
            case 2:
                camPos[1] -= len;
                break;
            case 3:
                camPos[0] += len;
                break;
            case 4:
                camPos[1] += len;
                break;
        }
    } else{ //Looking ahead so move camera behind
        len = 5; //Change len so that player can look more ahead
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

    if(keyArray[113] == 1){ //Q
        ofExit();
    }
    if(keyArray[109] == 1){ //Minimap
        minimap = !minimap;
        keyArray[109] = 0;
    }
    if(keyArray[97] == 1 && !testCycle.getLeftFlag()){ //A
        //If left is pressed, turn left and then don't turn left again
        testCycle.setLeftFlag(true);
        testCycle.turnCycle(1);
    }
    else if (keyArray[100] == 1 && !testCycle.getRightFLag()){ //D
        //If right is pressed, turn right and don't turn right again
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
    if (keyArray[119]){ //W
        testCycle.moveCycle(true);
    }
    if(!keyArray[119]){
        testCycle.moveCycle(false);
    }
    if(keyArray[57358] == 1){ //Right Arrow
        cameraState = -1;
    } else if(keyArray[57356] == 1){ //Left Arrow
        cameraState = 1;
    } else if (keyArray[57359] == 1){ //Backwards Arrow
        cameraState = 2;
    } else { //Reset when not being held
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
    // TODO
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
