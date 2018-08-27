//
//  E3System.hpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//


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
    virtual void setupParticles() override;
    virtual void particleInteractions() override;
    virtual void outputConditions() override;
    virtual void impactEffect() override;
    void drawTeamLines();
//    void seedWithRandomValues() override;
    
};
