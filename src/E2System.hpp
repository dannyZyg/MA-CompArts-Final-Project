//
//  E2System.hpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//
//// Derivative class of Particle System.

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
    virtual void impactEffect() override;
    void updateColours();
    void seedWithRandomValues() override;
    void presetSelector(string preset) override;
    void receiveCells(vector <float> cells_);
    void cellWallRebound(Particle& particle);
    void allocateCellState(Particle& particle);
    void animateCellWalls();
    void drawCells();
    bool pingFromWalls;
    bool cellWallsActive;
    
    vector <float> scaledCells;
    vector <float> cells;
};
