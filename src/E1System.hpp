////
////  E1System.hpp
////  FinalProjectMA
////
////  Created by Danny on 25/8/18.
////

#pragma once

#ifndef E1System_hpp
#define E1System_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleSystem.h"
#endif /* E1System_hpp */


class E1System : public ParticleSystem {

    
public:
    E1System();
    virtual void setupParticles() override;
    virtual void update() override;
    virtual void particleInteractions() override;
    virtual void outputConditions() override;
    virtual void impactEffect() override;
    void alterSize(Particle& cur_);
    
    
    void drawMembranes();
    
    float maxRad, minRad;
    float region;
};
