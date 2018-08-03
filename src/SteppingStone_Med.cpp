//
//  SteppingStone_Med.cpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#include "SteppingStone_Med.hpp"



SteppingStone_Med::SteppingStone_Med(){
    
    name = "Medium Stones";
    width = 800;
    height = 800;
    allocate(width, height);
    
    
}


void SteppingStone_Med::setup(){
    debug = false;
    ofBackground(255);
    gs = width/2;
    numCells = fbo->getHeight()/gs;
    rad = gs/2;
    
    binPower = 5;

    for(int i = 0; i < numCells; i++){
        for(int k = 0; k < numCells; k++){
            ofVec2f tempOrigin;
            tempOrigin.set (k*gs + gs/2, i*gs + gs/2);
            origins.push_back(tempOrigin);

        }
    }

    for(int i = 0; i < origins.size(); i ++){
        StoneParticleSystem s;
        stones.push_back(s);


    }
    
    for(int i = 0; i < stones.size(); i ++){
        stones[i].baseColour = ofColor (200, 100, 30);
        stones[i].origin = origins[i];
        stones[i].externalRad = rad;
        stones[i].setup(width, height, binPower);
//        stones[i].centerAttraction = ofRandom(0.1, 2);
//        stones[i].particleRepulsion = ofRandom(0.4, 2);
        
    }
}




void SteppingStone_Med::update(){
    
    
//    sequence2();
    
    for(int i = 0; i < stones.size(); i ++){
//        stones[i].update();
        
    }
    
//    cout<<stones.size()<<endl;
    
}


void SteppingStone_Med::draw(){
    

    ofBackground(255);
    

    
    
    for(int i = 0; i < origins.size(); i ++){

        ofPushStyle();
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofDrawCircle(origins[i], rad);
        ofSetColor(255);
        ofNoFill();
        ofDrawCircle(origins[i], rad);
        ofPopStyle();

    }


    for(int i = 0; i < stones.size(); i ++){
        
        if(stones[i].active) stones[i].display();
    }
    
    if(debug) debugMode();
    
    
}



void SteppingStone_Med::sequence1(){
    
//    // instructions for the illumination of particular stepping stones in order and in a timed sequence
//
//    stones[0].active = true;
//
//    if (ofGetElapsedTimeMillis() - startTime > 3000){
//
//        stones[1].active = true;
//    }
//    if (ofGetElapsedTimeMillis() - startTime > 6000){
//
//        stones[2].active = true;
//    }
//    if (ofGetElapsedTimeMillis() - startTime > 9000){
//
//        stones[3].active = true;
//    }
}


void SteppingStone_Med::sequence2(){
    
    // instructions for the illumination of particular stepping stones in order and in a timed sequence
    
    
    stones[3].active = true;
    

    if (ofGetElapsedTimeMillis() - startTime > 3000){

        stones[1].active = true;
    }
    if (ofGetElapsedTimeMillis() - startTime > 6000){

        stones[2].active = true;
    }
    if (ofGetElapsedTimeMillis() - startTime > 9000){

        stones[0].active = true;
     
    }
}

void SteppingStone_Med::reset(){
    
//    for(int i = 0; i < stones.size(); i ++){
//
//        startTime = ofGetElapsedTimeMillis();
////        stones[i].reset();
//    }
    
}


void SteppingStone_Med::debugMode(){
    

    
    
    for(int i = 0; i < stones.size(); i ++){
        ofPushMatrix();
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofDrawCircle(origins[i], rad);
        ofTranslate( origins[i].x, origins[i].y);
//        ofScale(10, 10);
        ofSetColor(0);
        ofDrawBitmapString(ofToString(i + 1),0, 0);
        ofPopStyle();
        ofPopMatrix();
        
    }
}


