//
//  E1System.hpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//
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
    vector<E1Particle> particles;
//    vector< vector<E1Particle*> > bins;
//    int width, height, k, xBins, yBins, binSize;
    
//    void draw();
    void setupColours();
    
//    vector<E1Particle*> getNeighbors(E1Particle& particle, float radius);
//    vector<E1Particle*> getNeighbors(float x, float y, float radius);
//    vector<E1Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);

    ofxColorPalette team1Col;
    ofxColorPalette team2Col;
    
    ofxColorPalette::ColorChannel mode;
    //    float brightness;
    //    float saturation;
    
    void alterSize(E1Particle& cur_);
    float maxRad, minRad;
    float region;
    void communicationCondition();
    
    void particleInteractions();
    void outputConditions();
    void impactEffect();
    
    float outputThreshold;
    float outputCondition;
    
    bool colourSwitch();
    
};
