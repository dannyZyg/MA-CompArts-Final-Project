//
//  EnvironmentSource.hpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#pragma once

#ifndef EnvironmentSource_hpp
#define EnvironmentSource_hpp

#include <stdio.h>
#include "ofMain.h"
#include "FboSource.h"
//#include "EnvironmentOneSystem.h"
#include "EnvironmentTwoSystem.h"
#include "ParticleSystem.h"
#include "E1System.hpp"
#include "E3System.hpp"
#include "ofxFboBlur.h"
#include "ofxBlur.h"



#endif /* EnvironmentSource_hpp */

class EnvironmentSource : public ofx::piMapper::FboSource{
    
public:
    
    EnvironmentSource();
    void setup();
    void update();
    void draw();
    
    float width, height;
    
    ofVec2f origin;
    float rad;
    
    
    bool debug;
    
    // debug font
    ofTrueTypeFont font;
    bool blur1, blur2;
    
    ofxBlur blur;
    
    ofxFboBlur gpuBlur;
    //choose your fbo settings
    ofFbo::Settings s;
    
    float startTime;
    bool active;
    
    float setScale;
    float setRotation;
    float blurOffset;
    float blurPasses;
    
    bool test;
    void newRules();
//    EnvironmentOneSystem enviro;
    E1System enviro;

};

class EnvironmentTestSource : public EnvironmentSource{
//    EnvironmentOneSystem enviro;
    void setup();
//    e1v2test enviro;

    
};

class EnvironmentTest2Source : public EnvironmentSource{
    void setup();
//    EnvironmentTwoSystem enviro;

};
//
//class EnvironmentThreeSource : public EnvironmentSource{
//    void setup();
//};
