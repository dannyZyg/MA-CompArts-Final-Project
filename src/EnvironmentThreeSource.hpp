//
//  EnvironmentThreeSource.hpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//
#pragma once

#ifndef EnvironmentThreeSource_hpp
#define EnvironmentThreeSource_hpp

#include <stdio.h>
#include "FboSource.h"
#include "EnvironmentThreeSystem.h"
#include "ofxBlur.h"
#include "ofxFboBlur.h"

#endif /* EnvironmentThreeSource_hpp */


class EnvironmentThreeSource : public ofx::piMapper::FboSource{
    
    public:
    
    EnvironmentThreeSource();
    
    void setup();
    void update();
    void draw();
    
    float width, height;
    
    ofVec2f origin;
    float rad;
    
    EnvironmentThreeSystem enviro;
    bool debug;
    
    ofxBlur blur;
    bool blur1, blur2;
    bool active;
    
    ofxFboBlur gpuBlur;
    //choose your fbo settings
    ofFbo::Settings s;
    
    // debug font
    ofTrueTypeFont font;
    
    float setScale;
    float setRotation;
    float blurOffset;
    float blurPasses;

    

};
