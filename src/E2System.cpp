//
//  E2System.cpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//

#include "E2System.hpp"

void E2System::setupParticles(){
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        E3Particle particle = E3Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particles.push_back(particle);
    }
    
    team1Base = ofColor(52,167, 173);
    team2Base = ofColor(255,211, 91);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}
