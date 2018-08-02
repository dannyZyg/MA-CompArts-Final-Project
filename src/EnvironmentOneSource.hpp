//
//  EnvironmentOneSource.hpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#ifndef EnvironmentOneSource_hpp
#define EnvironmentOneSource_hpp

#include <stdio.h>
#include "FboSource.h"
#include "EnvironmentOneSystem.h"

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
    
};
