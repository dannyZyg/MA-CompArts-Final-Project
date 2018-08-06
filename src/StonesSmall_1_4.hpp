//
//  StonesSmall_1_4.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//
#pragma once

#ifndef StonesSmall_1_4_hpp
#define StonesSmall_1_4_hpp

#include <stdio.h>
#include "ofMain.h"
//#include "SteppingStoneType1.hpp"
#include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* StonesSmall_1_4_hpp */


class StonesSmall_1_4 : public ofx::piMapper::FboSource{
    
public:
    
    StonesSmall_1_4();
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





