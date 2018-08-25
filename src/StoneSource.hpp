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
    void setupParticleSystems();
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
    
    int labelOffset;
};

class SmallStones_1_4 : public StoneSource {
    void setup();
};

class SmallStones_5_8 : public StoneSource {
    void setup();
};

class SmallStones_9_12 : public StoneSource {
    void setup();
    void setName();
};

class SmallStones_13_16 : public StoneSource {
    void setup();
    void setName();
};

class MediumStones_1_4 : public StoneSource {
    void setup();
    void setName();
};

class MediumStones_5_8 : public StoneSource {
    void setup();
    void setName();
};

class LargeStones_1_4 : public StoneSource {
    void setup();
    void setName();
};





