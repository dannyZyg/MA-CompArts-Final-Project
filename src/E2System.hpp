//
//  E2System.hpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//

#pragma once
#ifndef E2System_hpp
#define E2System_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleSystem.h"
#endif /* E2System_hpp */


class E2System : public ParticleSystem{
public:
    virtual void setupParticles() override;
    virtual void particleInteractions() override;
    virtual void outputConditions() override;
    virtual void impactEffect() override;
    
};
