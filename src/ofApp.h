#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"

#include "SteppingStone_1_4.hpp"
#include "SteppingStone_Large.hpp"
#include "SteppingStone_Med.hpp"
#include "EnvironmentOneSource.hpp"
#include "EnvironmentTwoSource.hpp"
#include "EnvironmentThreeSource.hpp"


//#include "SteppingStone_5_8.hpp"
//#include "SteppingStoneType1.hpp"



#define NUM_BYTES 2

class ofApp : public ofBaseApp{
public:
//    ofApp(){}
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    
    void scheduler();

    ofxPiMapper piMapper;

    // By using a custom source that is derived from FboSource
    // you will be able to see the source listed in sources editor

    SteppingStone_1_4 stones_1_4;
    SteppingStone_Large large_stones;
    SteppingStone_Med med_stones;
    EnvironmentOneSource environmentOne;
    EnvironmentTwoSource environmentTwo;
    EnvironmentThreeSource environmentThree;
//    SteppingStone_5_8 stones_5_8;
//    SteppingStoneType1 type1;
    

    
    
    ofTrueTypeFont        font;
    
    bool drawTemplate;
    ofImage layout;
  
    bool displayCircleTemplate;
    
    bool debug;
    
   

};
