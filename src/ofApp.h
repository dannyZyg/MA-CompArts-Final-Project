#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "StoneParticleSystem.h"

#include "StonesSmall_1_4.hpp"
#include "StonesSmall_5_8.hpp"
#include "StonesSmall_9_12.hpp"
#include "StonesSmall_13_16.hpp"
#include "StonesMed_1_4.hpp"
#include "StonesMed_5_8.hpp"
#include "StonesLarge_1_4.hpp"
#include "EnvironmentOneSource.hpp"
#include "EnvironmentTwoSource.hpp"
#include "EnvironmentThreeSource.hpp"

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

    StonesSmall_1_4 small_stones_1_4;
    StonesSmall_5_8 small_stones_5_8;
    StonesSmall_9_12 small_stones_9_12;
    StonesSmall_13_16 small_stones_13_16;
    StonesMed_1_4 med_stones_1_4;
    StonesMed_5_8 med_stones_5_8;
    StonesLarge_1_4 large_stones;
    EnvironmentOneSource environmentOne;
    EnvironmentTwoSource environmentTwo;
    EnvironmentThreeSource environmentThree;
        
    bool drawTemplate;
    ofImage layout;
  
    bool displayCircleTemplate;
    
    bool debug;
    
    void debugDisplay();
    ofTrueTypeFont verdana30;
    
    float startTime;
    int timeSpacing;


};
