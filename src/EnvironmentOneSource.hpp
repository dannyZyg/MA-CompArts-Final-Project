//
//  EnvironmentOneSource.hpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#pragma once

#ifndef EnvironmentOneSource_hpp
#define EnvironmentOneSource_hpp

#include <stdio.h>
#include "FboSource.h"
#include "EnvironmentOneSystem.h"
#include "ofxFboBlur.h"
#include "ofxBlur.h"

#endif /* EnvironmentOneSource_hpp */


class EnvironmentOneSource : public ofx::piMapper::FboSource{
    
    public:
    
    EnvironmentOneSource();
    
    void setup();
    void update();
    void draw();
    
    float width, height;
    
    ofVec2f origin;
    float rad;
    
    EnvironmentOneSystem enviro;
    
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
    
    string s1, s2, s3, s4;
    
//    float setScale;
//    float setRotation;
//    float blurOffset;
//    float blurPasses;
    bool test;
    float setScale;
    
    void newRules();
};
