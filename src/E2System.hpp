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
    E2System();
    virtual void setupParticles() override;
    virtual void particleInteractions() override;
    virtual void outputConditions() override;
    virtual void impactEffect() override;
    void updateColours();
    virtual void update() override;
    virtual void seedWithRandomValues() override;
    void newRules(int option);
    void presetSelector(string preset);
    void receiveCells(vector <float> cells_);
    void cellWallRebound(Particle& particle);
    void allocateCellState(Particle& particle);
    void animateCells();
    void drawCells();
    bool pingFromWalls;
    bool cellWallsActive;
    
    vector <float> scaledCells;
    vector <float> cells;
};
