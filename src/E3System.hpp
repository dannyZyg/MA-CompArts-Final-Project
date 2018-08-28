//
//  E3System.hpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//
//// Derivative class of Particle System.


#pragma once
#ifndef E3System_hpp
#define E3System_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleSystem.h"

#endif /* E3System_hpp */

class E3System : public ParticleSystem{
public:
    E3System();
    void setupParticles() override;
    void particleInteractions() override;
    void outputConditions() override;
    void impactEffect() override;
    void seedWithRandomValues() override;
    void presetSelector(string preset) override;
    void drawTeamLines();
//    void seedWithRandomValues() override;
    
};
