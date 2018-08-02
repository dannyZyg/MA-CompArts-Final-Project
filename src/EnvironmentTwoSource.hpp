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
#include "EnvironmentTwoSystem.h"

#endif /* EnvironmentTwoSource_hpp */


class EnvironmentTwoSource : public ofx::piMapper::FboSource{
    
    public:
    
    EnvironmentTwoSource();
    
    void setup();
    void update();
    void draw();
    
    float width, height;
    
    ofVec2f origin;
    float rad;
    
    EnvironmentTwoSystem enviro;
    
    bool debug;
    
};
