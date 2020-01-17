#include "ofApp.h"
#include "cycle.h"
#include "time.h"
#include <iostream>
#include <thread>
#include <future>

//static const dVector3 yunit = {0,1,0}, zunit = {0,0,1};

void ofApp::setup(){

    srand (time(NULL));

    //Set some vars
    ofDisableArbTex();

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

    //p1
    p1.setX(80);
    p1.setColour(2);
    p1.setZ(0.6f);
    p1.setActive(true);
    p1.setIsAI(false);
    p1.assignModel();
    p1.setDebugDraw(false);
    p1.setHeading(1);

    //p2
    p2.setX(-80);
    p2.setColour(2);
    p2.setZ(0.6f);
    p2.setActive(true);
    p2.setIsAI(false);
    p2.assignModel();
    p2.setDebugDraw(false);
    p2.setHeading(3);

    //Update cameraObject
    cameraObject = &p1;

    //BoundaryWalls
    for(int i = 0; i < 4; i++){
        boundaryWall b = boundaryWall(ofColor::blue);
        b.setBoundary(i+1);
        boundaryWalls.push_back(b);
    }
    //Lighting
    p1Light.setPosition(0,0,2);
    p1Light.lookAt({0,0,0});
    p1Light.setAmbientColor(ofColor::blue);
    p1Light.enable();

    p2Light.setPosition(0,0,2);
    p2Light.lookAt({0,0,0});
    p2Light.setAmbientColor(ofColor::blue);
    p2Light.enable();
    aiObjects.push_back(p2);

    //Indicator light so that it doesn't look dark on the minimap
    p1IndicatorLight.setPosition(0,0,2);
    p1IndicatorLight.lookAt({0,0,0});
    p1IndicatorLight.setAmbientColor(ofColor::blue);
    p1IndicatorLight.enable();

    p2IndicatorLight.setPosition(0,0,2);
    p2IndicatorLight.lookAt({0,0,0});
    p2IndicatorLight.setAmbientColor(ofColor::blue);
    p2IndicatorLight.enable();

    debugInfo = true;
    minimap = true;
    winner = 0;
    b_resWatch = true;
}
void ofApp::update(){
    p1.moveCycle(true);
    p2.moveCycle(true);
    handleKeyPress();

    if(winner == 0){
        //Update camera collisions in a seperate thread, because efficiency
        std::thread collisionsMainThread(&ofApp::collisions, this);
        std::thread doGameObjectAI(&ofApp::doAIForObjects, this);
        std::thread doPlayerWalls(&ofApp::doWalls, this);

        //Update camera and handle keypresses in the main thread
        ofVec3f cameraPos = overheadCam.getPosition();
        p1.updateIndicatorPosition(cameraPos.x, cameraPos.y, cameraPos.z);
        p2.updateIndicatorPosition(cameraPos.x, cameraPos.y, cameraPos.z);
        updateCamera();

        //Lighitng
        p1Light.setPosition(p1.getX(), p1.getY(), (p1.getZ() + 2));
        p1Light.lookAt({p1.getX(), p1.getY(), p1.getZ()});
        p1IndicatorLight.setPosition(p1.getX(), p1.getY(), (p1.getZ() + 15));
        p1IndicatorLight.lookAt({p1.getX(), p1.getY(), p1.getZ()});

        p2Light.setPosition(p2.getX(), p2.getY(), (p2.getZ() + 2));
        p2Light.lookAt({p2.getX(), p2.getY(), p2.getZ()});
        p2IndicatorLight.setPosition(p2.getX(), p2.getY(), (p2.getZ() + 15));
        p2IndicatorLight.lookAt({p2.getX(), p2.getY(), p2.getZ()});

        //Overhead camera
        if(minimap){
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

        //Make sure the threads have finished
        collisionsMainThread.join();
        doGameObjectAI.join();
        doPlayerWalls.join();

        switch(collRes){
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
        }

        //Update cycle lastX/Y
        p1.updateLastX();
        p1.updateLastY();

        p2.updateLastX();
        p2.updateLastY();
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
            ss <<"p1:\nX:\t" << p1.getX() << "\nY:\t" << p1.getY() << "\nHeading:\t" << p1.getHeading() << " ";
            switch((int) p1.getHeading()){
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
        ofSetColor(ofColor::white);
        ofDrawBitmapString("A winner is you!", 50, 50);
    } else if(winner == 2){
        ofSetColor(ofColor::white);
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
    p1.drawIndicator();
    p2.drawIndicator();

    ofSetColor(ofColor::white);

    //players
    p1.draw();
    p2.draw();
}
void ofApp::keyPressed(int key){ keyArray[key] = 1; }
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
    if(keyArray[97] == 1 && !p1.getLeftFlag()){ //A
        //If left is pressed, turn left and then don't turn left again
        p1.setLeftFlag(true);
        p1.turnCycle(1);
    }
    else if (keyArray[100] == 1 && !p1.getRightFLag()){ //D
        //If right is pressed, turn right and don't turn right again
        p1.setRightFlag(true);
        p1.turnCycle(2);
    }
    if(p1.getLeftFlag() && keyArray[97] == 0){
        //Reset left flag when left is not pressed
        p1.setLeftFlag(false);
    }
    if(p1.getRightFLag() && keyArray[100] == 0){
        //Reset right flag when right is not pressed
        p1.setRightFlag(false);
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
void ofApp::doAIForObjects(){
    for(gameObject g : aiObjects){
        g.doAI();
    }
}
void ofApp::doWalls(){
    if((p1.getX() != p1.getLastX())||(p1.getY() != p1.getLastY())){
        ofColor newColor;
        switch(p1.getColour()){
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
        cycleWalls.push_back(cycleWall(newColor, &p1));
        //std::cout << "Placed new wall" << std::endl;
    }
    if((p2.getX() != p2.getLastX())||(p2.getY() != p2.getLastY())){
        ofColor newColor;
        switch(p2.getColour()){
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
        cycleWalls.push_back(cycleWall(newColor, &p2));
    }
}
void ofApp::collisions(){
    int res = 0;
    std::thread p1CollThread(&ofApp::doBodyCollisions, this, p1);
    std::thread p2CollThread(&ofApp::doBodyCollisions, this, p2);

    p1CollThread.join();
    p2CollThread.join();
}
void ofApp::doBodyCollisions(gameObject obj){

    gameObject* player1Pointer = &p1;
    gameObject* objPointer = &obj;

    for(boundaryWall w : boundaryWalls){
        collide(obj,w);
        if(b_res == 3){
            if(objPointer == player1Pointer){
                winner = 2;
            } else {
                winner = 1;
            }
        }
    }
    for(cycleWall w : cycleWalls){
        collide(obj,w);
        if(b_res == 3){
            if(objPointer == player1Pointer){
                winner = 2;
            } else {
                winner = 1;
            }
        }
    }
}
void ofApp::collide(gameObject obj1, gameObject obj2){

    b_res = 0;

    if((obj1.getX() + obj1.getL()) < obj2.getX()){
        if((obj1.getY() + obj1.getW()) < obj2.getY()){
                b_res = 3;
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
