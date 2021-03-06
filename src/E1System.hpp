////
////  E1System.hpp
////  FinalProjectMA
////
////  Created by Danny on 25/8/18.
////
//// Derivative class of Particle System.

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
    void setupParticles() override;
    void particleInteractions() override;
    void impactEffect() override;
    void alterSize(Particle& cur_);
    void seedWithRandomValues() override;
    void presetSelector(string preset) override;
    
    void drawMembranes();
    
    float maxRad, minRad;
    float region;
    bool colourExchange;
};
