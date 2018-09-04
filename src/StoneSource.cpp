//
//  StoneSource.cpp
//  FinalProjectMA
//
//  Created by Danny on 24/8/18.
//

#include "StoneSource.hpp"

StoneSource::StoneSource(){
    width = 800;
    height = 800;
    allocate(width, height);
    
    debug = false;
    ofBackground(255);
    gs = width/2;
    numCells = fbo->getHeight()/gs;
    rad = gs/2;
    binPower = 5;
    
    font.load("verdana.ttf", 80, true, true);
    font.setLineHeight(34.0f);
    font.setLetterSpacing(1.035);
}

void StoneSource::setup(){
    name = "generic stone";
    setupParticleSystems();
}

void SmallStones_1_4::setup(){
    name = "Small Stones 1-4";
    setupParticleSystems();
    labelOffset = 1;
    sizeInitial = "S";
}

void SmallStones_5_8::setup(){
    name = "Small Stones 5-8";
    setupParticleSystems();
    labelOffset = 5;
    sizeInitial = "S";
}

void SmallStones_9_12::setup(){
    name = "Small Stones 9-12";
    setupParticleSystems();
    labelOffset = 9;
    sizeInitial = "S";
}

void SmallStones_13_16::setup(){
    name = "Small Stones 13-16";
    setupParticleSystems();
    labelOffset = 13;
    sizeInitial = "S";
}

void MediumStones_1_4::setup(){
    name = "Medium Stones 1-4";
    setupParticleSystems();
    labelOffset = 1;
    sizeInitial = "M";
}

void MediumStones_5_8::setup(){
    name = "Medium Stones 5-8";
    setupParticleSystems();
    labelOffset = 5;
    sizeInitial = "M";
}

void LargeStones_1_4::setup(){
    name = "Large Stones 1-4";
    setupParticleSystems();
    labelOffset = 1;
    sizeInitial = "L";
}

void StoneSource::setupParticleSystems(){
    
    for(int i = 0; i < numCells; i++){
        for(int k = 0; k < numCells; k++){
            ofVec2f tempOrigin;
            tempOrigin.set (k*gs + gs/2, i*gs + gs/2);
            origins.push_back(tempOrigin);
        }
    }
    
    for(int i = 0; i < origins.size(); i ++){
        StoneSystem s;
        stones.push_back(s);
    }
    
    for(int i = 0; i < stones.size(); i ++){
        stones[i].origin = origins[i];
        stones[i].externalRad = rad;
        stones[i].setup(width, height, binPower);
        stones[i].active = true;
    }
}

void StoneSource::draw(){
    
    ofBackground(0);    //refresh background every frame

    for(int i = 0; i < stones.size(); i ++){
        stones[i].display();
    }
    
    if(debug) debugMode();
    
}

void StoneSource::debugMode(){
    // Draw white circles over all stepping stones and display their labels
    for(int i = 0; i < stones.size(); i ++){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofDrawCircle(origins[i], rad);
        ofSetColor(0);
        font.drawString(sizeInitial + ofToString(i + labelOffset), origins[i].x - 100, origins[i].y);
        ofPopStyle();
    }
}


