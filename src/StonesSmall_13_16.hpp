//
//  StonesSmall_13_16.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#ifndef StonesSmall_13_16_hpp
#define StonesSmall_13_16_hpp

#include <stdio.h>
#include "ofMain.h"
#include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* StonesSmall_13_16_hpp */


class StonesSmall_13_16 : public ofx::piMapper::FboSource{
    
public:
    
    StonesSmall_13_16();
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





