//
//  StonesSmall_1_4.cpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#include "StonesSmall_1_4.hpp"



StonesSmall_1_4::StonesSmall_1_4(){
    
    name = "Small Stones 1-4";
    width = 800;
    height = 800;
    allocate(width, height);
    
}


void StonesSmall_1_4::setup(){
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
        stones[i].baseColour = ofColor(47,191, 57);
        stones[i].origin = origins[i];
        stones[i].externalRad = rad;
        stones[i].setup(width, height, binPower);
    }
}




void StonesSmall_1_4::update(){
    
    
//    sequence2();
    
    for(int i = 0; i < stones.size(); i ++){
//        stones[i].update();
        
    }
    
//    cout<<stones.size()<<endl;
    
}


void StonesSmall_1_4::draw(){
    
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
        stones[i].fadeParticles();
       stones[i].display();
    }
    
    if(debug) debugMode();
    
    
}

void StonesSmall_1_4::reset(){
    
//    for(int i = 0; i < stones.size(); i ++){
//
//        startTime = ofGetElapsedTimeMillis();
////        stones[i].reset();
//    }
    
}

void StonesSmall_1_4::debugMode(){
    
// Draw white circles over all stepping stones and display their position numbers
    
    for(int i = 0; i < stones.size(); i ++){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofDrawCircle(origins[i], rad);
        ofSetColor(0);
        font.drawString("S" + ofToString(i + 1),origins[i].x - 100, origins[i].y);
        ofPopStyle();
        
    }
}


