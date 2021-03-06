//
//  EnvironmentTwoSource.hpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#ifndef EnvironmentTwoSource_hpp
#define EnvironmentTwoSource_hpp

#include <stdio.h>
#include "FboSource.h"
#include "E2System.hpp"
#include "ParticleSystem.h"
#include "ofxBlur.h"
#include "ofxFboBlur.h"

#endif /* EnvironmentTwoSource_hpp */


class EnvironmentTwoSource : public ofx::piMapper::FboSource{
    
    public:
    
    EnvironmentTwoSource();
    
    void setup();
    void update();
    void draw();
    
    float width, height;
    
    E2System enviro;
    ofVec2f origin;
    float rad;
    
    vector <float> cells;
    vector <float> noiseSeed;
    
    bool debug;
    
    ofxBlur blur;

    ofxFboBlur gpuBlur;
    //choose your fbo settings
    ofFbo::Settings s;
    
    bool blur1, blur2;
    ofTrueTypeFont font;
    
    void drawCellDebug();
    
    bool active;
    
    float setScale;
    float setRotation;
    float blurOffset;
    float blurPasses;
    
};
