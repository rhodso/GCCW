#include "ofApp.h"
#include "cycle.h"
#include <iostream>
#include <thread>
#include <future>

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
    for(int i = 0; i < 4; i++){
        boundaryWall b = boundaryWall(ofColor::blue);
        b.setBoundary(i+1);
        boundaryWalls.push_back(b);
    }
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

    debugInfo = true;
    winner = 0;
    b_resWatch = true;
}
void ofApp::update(){
    if(winner == 0){
        //Update camera collisions in a seperate thread, because efficiency
        //std::thread collisionsMainThread(&ofApp::collisions, this);
        collisions();

        //Update camera and handle keypresses in the main thread
        ofVec3f cameraPos = overheadCam.getPosition();
        testCycle.updateIndicatorPosition(cameraPos.x, cameraPos.y, cameraPos.z);
        updateCamera();
        handleKeyPress();

        //Add walls
        //std::cout << "TestCycle vars X/LX/Y/LY: " << testCycle.getX() << " " << testCycle.getLastX() << " " << testCycle.getY() << " " << testCycle.getLastY() << std::endl;
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
            cycleWalls.push_back(cycleWall(newColor, &testCycle));
            //std::cout << "Placed new wall" << std::endl;
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
        //collisionsMainThread.join();

        switch(collRes){
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
        }

        //Update cycle lastX/Y
        testCycle.updateLastX();
        testCycle.updateLastY();
    }
}
void ofApp::draw(){
    if(winner == 0){
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

        if(debugInfo){
            ofSetColor(ofColor::white);
            stringstream ss;
            ss << "FPS: " << ofToString(ofGetFrameRate(),0) << std::endl << std::endl;
            ss <<"TestCycle:\nX:\t" << testCycle.getX() << "\nY:\t" << testCycle.getY() << "\nHeading:\t" << testCycle.getHeading() << " ";
            switch((int) testCycle.getHeading()){
            case 1:
                ss << "";
                break;
            case 2:
                ss << "(-ve Y)";
                break;
            case 3:
                ss << "";
                break;
            case 4:
                ss << "(+ve Y)";
                break;
            }
            ss << std::endl << std::endl;
            ss << "Camera X/Y/Z: " << cameraObject->getX() << "/" << cameraObject->getY() << "/" << cameraObject->getZ() << std::endl << std::endl;

            ofDrawBitmapString(ss.str().c_str(), 300,20);
        }
    } else if(winner == 1){
        ofDrawBitmapString("A winner is you!", 50, 50);
    } else if(winner == 2){
        ofDrawBitmapString("A winner isn't you!", 50, 50);
    }else {
        winner = 0;
    }
}
void ofApp::drawObjects(){
    //Background
    ofBackground(20);

    //Grid
    ofSetColor(ofColor::lightCyan);
    ofDrawGrid(1, 100, false, false, false, true);

    //Walls
    for(boundaryWall b : boundaryWalls){
        b.draw();
    }

    //CycleWalls
    for(cycleWall w : cycleWalls){
        w.draw();
    }

    //playerIndicator
    testCycle.drawIndicator();

    ofSetColor(ofColor::white);

    //Testcycle
    testCycle.draw();
}
void ofApp::keyPressed(int key){ keyArray[key] = 1; /*std::cout << key << std::endl;*/ }
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
    if(keyArray[57346]){//F3
        debugInfo = !debugInfo;
        keyArray[57346] = 0;
    }
}

/*
 * Result of collisions
 * 0 - Nobody dies today
 * 1 - Player 1 needs to die
 * 2 - Player 2 needs to die
*/
void ofApp::collisions(){
    int res = 0;

    //TODO

    //Get a list of all current gameObjects that collisions need to be processed for
    //Assign collision pairs
    //Create a thread to see if they collide
    //Get the result
    //std::thread testCycleCollThread(&ofApp::collide, this);
    //testCycleCollThread.join();


    for(boundaryWall w : boundaryWalls){
        collide(testCycle,w);
        if(b_res){
            winner = 1;
        }
    }
    for(cycleWall w : cycleWalls){
        collide(testCycle,w);
        if(b_res){
            winner = 1;
        }
    }
}

/* What the collision result means
    0 - Not colliding
    1 - Object 1
*/
void ofApp::collide(gameObject obj1, gameObject obj2){

    b_res = false;

    if((obj1.getX() + obj1.getL()) < obj2.getX()){
        if((obj1.getY() + obj1.getW()) < obj2.getY()){
                b_res = true;
        }
    }


    if(b_res != b_resWatch){
        std::cout << "b_res " << b_res << std::endl;
        b_resWatch = b_res;
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
