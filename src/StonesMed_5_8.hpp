//
//  StonesMed_5_8.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//
#pragma once

#ifndef StonesMed_5_8_hpp
#define StonesMed_5_8_hpp

#include <stdio.h>
#include "ofMain.h"
//#include "StoneParticleSystem.h"
//#include "ParticleSystem.h"
#include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* StonesMed_5_8_hpp */


class StonesMed_5_8 : public ofx::piMapper::FboSource{
    
public:
    
    StonesMed_5_8();
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





