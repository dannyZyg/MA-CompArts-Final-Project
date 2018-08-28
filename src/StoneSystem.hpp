//
//  StoneSystem.hpp
//  FinalProjectMA
//
//  Created by Danny on 28/8/18.
//
#pragma once
#ifndef StoneSystem_hpp
#define StoneSystem_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleSystem.h"
#endif /* StoneSystem_hpp */


class StoneSystem : public ParticleSystem {
public:
    
    StoneSystem();
    
    void setupParticles() override;
    void display() override;
    void particlesInOut(int start, bool active, float& display);
    void pushPopParticles();
    void addParticle(int team_);
    
    Timer activeTimer;
    Timer releaseTimer;
    
    unsigned int timer;
    int showParticleSpacing;
    int e1StartIndex, e2StartIndex, e3StartIndex, sensStartIndex;
    int environmentDivision;
    float env1Display, env2Display, env3Display, sensorDisplay;

    bool env1, env2, env3, sens;
    bool active;
    bool timeIntervalPassed;

    
};
