//
//  StonesSmall_9_12.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//
#pragma once

#ifndef StonesSmall_9_12_hpp
#define StonesSmall_9_12_hpp

#include <stdio.h>
#include "ofMain.h"
#include "StoneParticleSystem.h"
#include "ParticleSystem.h"
#include "FboSource.h"

#endif /* StonesSmall_9_12_hpp */


class StonesSmall_9_12 : public ofx::piMapper::FboSource{
    
public:
    
    StonesSmall_9_12();
    void setup();
    void update();
    void draw();
    void sequence1();
    void sequence2();
    void reset();
    void debugMode();
    
    vector <ofVec2f> origins;
    vector <StoneParticleSystem> stones;
    float gs;
    float numCells;
    float cellSize;
    float startTime;
    
    float rad;
    
    float width, height;
    
    int binPower;
    
    bool debug;
    
    
    // debug font
    ofTrueTypeFont font;
    
};





