//
//  SteppingStone_Med.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#ifndef SteppingStone_Med_hpp
#define SteppingStone_Med_hpp

#include <stdio.h>
#include "ofMain.h"
//#include "SteppingStoneType1.hpp"
#include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* SteppingStone_Med_hpp */


class SteppingStone_Med : public ofx::piMapper::FboSource{
    
public:
    
    SteppingStone_Med();
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
    
};





