//
//  StonesSmall_5_8.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//
#pragma once

#ifndef StonesSmall_5_8_hpp
#define StonesSmall_5_8_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleSystem.h"
#include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* StonesSmall_5_8_hpp */


class StonesSmall_5_8 : public ofx::piMapper::FboSource{
    
public:
    
    StonesSmall_5_8();
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





