//
//  StoneSource.hpp
//  FinalProjectMA
//
//  Created by Danny on 24/8/18.
//

#ifndef StoneSource_hpp
#define StoneSource_hpp

#include <stdio.h>
#include "ofMain.h"
 #include "StoneParticleSystem.h"
#include "FboSource.h"

#endif /* StoneSource_hpp */

class StoneSource : public ofx::piMapper::FboSource{
    
public:
    
    StoneSource();
    
//    string name = "Stone Source";
    
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
    
    bool active;
    string sizeInitial;
    
    void setName();
};

class SmallStoneSource : public StoneSource {
    
//    SmallStoneSource();
    void setName();
};





