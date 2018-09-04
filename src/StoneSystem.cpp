//
//  StoneSystem.cpp
//  FinalProjectMA
//
//  Created by Danny on 28/8/18.
//

#include "StoneSystem.hpp"


StoneSystem::StoneSystem(){

    showParticleSpacing = 5;
    activeTimer = Timer();
    releaseTimer = Timer();
    activeTimer.setup();
    releaseTimer.setup();
    
    releaseTimer.endTime = 1000;
    
    e1StartIndex = 0;
    e2StartIndex = 31;
    e3StartIndex = 61;
    sensStartIndex = 91;
    env1Display = e1StartIndex;
    env2Display = e2StartIndex;
    env3Display = e3StartIndex;
    sensorDisplay = sensStartIndex;
    
    env1 = false;
    env2 = false;
    env3 = false;
    sens = false;
    
    kParticles = 120;

    particleNeighborhood = 32;
    particleRepulsion = 0.1;
    centerAttraction = 0;
    
    active = false;
    timer = 0;
    drawLines = true;
    rebound = false;
    environmentDivision = 30;
    activateDraw = false;
}


void StoneSystem::setupParticles(){
    
    for(int i = 0; i < kParticles; i++) {
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        Particle particle = Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particle.setupStoneParticle();
        particles.push_back(particle);
    }
    
    // divide up the particles and assign them to teams (which environment sequence the particles will activate for)
    for(int i = 0; i < particles.size(); i++){
        if(i >= 0 && i <= 30) particles[i].team = 0;
        if(i > 30 && i <= 60) particles[i].team = 1;
        if(i > 60 && i <= 90) particles[i].team = 2;
        if(i > 90){
            particles[i].team = 3;
            particles[i].r = ofRandom(18, 28);
            particles[i].damping = 0.01;
        }
    }
    
    // decide on the base colours to generate variations from
    team1Base = ofColor(20,201, 23);
    team2Base = ofColor(242,24, 249);
    team3Base = ofColor(239, 171, 93);
    team4Base = ofColor(193, 10, 7);
    setupColours();
}

void StoneSystem::display(){

// This display function overrides the base class display function. It uses a custom function to fade in and out a collection of particles based on which environment (or sensor) activates the stone. Multiple environments can activate collections of particles simultaneously.
    
    if(env1 || env2 || env3 || sens) counter = 0;
    else {
        counter ++;
        if(counter > 250) activateDraw = false; // the counter will determin how long the display function remains active
    }

    if(counter == 0) activateDraw = true;
    

    //If a system has sent a signal, allow the following, otherwise save CPU.
    if(activateDraw){
        
        for(int i = 90; i < particles.size(); i ++){
            // the particles assigned to the sensor sequences will have a changing alpha value
            particles[i].life = ofMap(ofNoise(ofGetFrameNum() * ( i * 0.0001) + (i * 100)), 0, 1, 150, 255);
        }
        
        // do this once per frame
        setupForces();
        
        // functions which fades in and out particles based on which system sends a signal
        particlesInOut(e1StartIndex, env1, env1Display);
        particlesInOut(e2StartIndex, env2, env2Display);
        particlesInOut(e3StartIndex, env3, env3Display);
        particlesInOut(sensStartIndex, sens, sensorDisplay);
        
        for(int i = 0; i < particles.size(); i++) {
            Particle& cur = particles[i];
            // force on other particles
            addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
            // forces on this particle
            cur.bounceOffWalls(rebound);
            cur.addDampingForce();
            
        }
        
        // single-pass global forces
        addAttractionForce(origin.x, origin.y, 200, centerAttraction);
        
        update();
        
    }
    
    timer ++;
    if(timer > 50000)timer = 0;

    
}

// Fade in and out particles when triggered in a particular stone sequence.
void StoneSystem::particlesInOut(int start, bool active, float& display){
    
// the input is the starting index, an activation toggle and a counter for the particles to display
    
    float max = start + 30;
    
    for(int i = start; i < display; i++) {
        particles[i].displayParticle();
    }
    
    if(active){
        
        if(timer % showParticleSpacing == 0){
            display ++;
            if(display >= max){
                display = max;
            }
        }
    }
    
    if(!active){
        if(timer % showParticleSpacing == 0){
            display --;
            if(display <= start){
                display = start;
            }
        }
    }
    
}

