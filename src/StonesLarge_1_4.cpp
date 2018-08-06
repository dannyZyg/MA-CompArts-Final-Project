//
//  StonesLarge_1_4.cpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#include "StonesLarge_1_4.hpp"



StonesLarge_1_4::StonesLarge_1_4(){

    name = "Large Stones 1-4";
    width = 800;
    height = 800;
    allocate(width, height);

}


void StonesLarge_1_4::setup(){
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
        stones[i].baseColour = ofColor (216, 41, 32);
        stones[i].origin = origins[i];
        stones[i].externalRad = rad;
        stones[i].setup(width, height, binPower);
//        stones[i].centerAttraction = ofRandom(0.1, 2);
//        stones[i].particleRepulsion = ofRandom(0.4, 2);

    }
}




void StonesLarge_1_4::update(){


//    sequence2();

    for(int i = 0; i < stones.size(); i ++){
//        stones[i].update();

    }

//    cout<<stones[1].timeIntervalPassed<<endl;
//    cout<<stones[1].timer<<endl;

}


void StonesLarge_1_4::draw(){


    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
    ofBackground(255);
    
    
    //refresh background circle colour every frame, for all stepping stones in the fbo source
    
    for(int i = 0; i < origins.size(); i ++){
        ofPushStyle();
        ofSetColor(0);
        ofNoFill();
        ofDrawRectangle(origins[i], gs, gs);
        ofFill();
        ofDrawCircle(origins[i], rad);
        ofPopStyle();
    }


    for(int i = 0; i < stones.size(); i ++){

       stones[i].display();
    }

    if(debug) debugMode();


}



void StonesLarge_1_4::sequence1(){

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


void StonesLarge_1_4::sequence2(){

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

void StonesLarge_1_4::reset(){

//    for(int i = 0; i < stones.size(); i ++){
//
//        startTime = ofGetElapsedTimeMillis();
////        stones[i].reset();
//    }

}


void StonesLarge_1_4::debugMode(){

    // Draw white circles over all stepping stones and display their position numbers
    
    for(int i = 0; i < stones.size(); i ++){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofDrawCircle(origins[i], rad);
        ofSetColor(0);
        font.drawString("L" + ofToString(i + 1),origins[i].x - 100, origins[i].y);
        ofPopStyle();
        
    }
}


