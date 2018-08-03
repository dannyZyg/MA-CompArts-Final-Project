//
//  SteppingStone_Large.hpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#ifndef SteppingStone_Large_hpp
#define SteppingStone_Large_hpp

#include <stdio.h>
#include "ofMain.h"
//#include "SteppingStoneType1.hpp"
#include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* SteppingStone_Large_hpp */


class SteppingStone_Large : public ofx::piMapper::FboSource{
    
public:
    
    SteppingStone_Large();
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





