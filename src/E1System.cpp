////
////  E1System.cpp
////  FinalProjectMA
////
////  Created by Danny on 25/8/18.
////
//
#include "E1System.hpp"

E1System::E1System(){
   
    kParticles = 80;
    
    particleNeighborhood = 80;
    particleRepulsion = 0.9;// 0.5;
    centerAttraction = 0.1; //0.6;
    
    drawLines = true;
    
    impact = false;
    maxRad = 20;
    
    outputThreshold = 10;
    outputCondition = 0;
    
}

void E1System::setupParticles(){
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        E1Particle particle = E1Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particles.push_back(particle);
    }
    
    team1Base = ofColor(27,125, 204);
    team2Base = ofColor(0,125, 90);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}

void E1System::update() {
    int n = particles.size();
    for(int i = 0; i < n; i++) {
        particles[i].updatePosition();
        particles[i].membraneRad = region;
    }    
}

void E1System::alterSize(Particle cur_){
    
    int nearby;
    region = cur_.r + maxRad;
    
    maxRad = particles[0].maxSize;
    vector<Particle*> closeNei = getNeighbors(cur_.x, cur_.y, region + 10);
    
    //alter size
    
    //test
    nearby = closeNei.size();
    ofPushStyle();
    ofFill();
    ofSetColor(200, cur_.membraneLife);
    if(nearby > 1){
        ofDrawCircle(cur_.x, cur_.y, region);
        cur_.alone = false;
        //        addAttractionForce(cur_, region, 0.2);
    }
    else(cur_.alone = true);
    
    ofPopStyle();
    
    
    for(int j = 0; j < closeNei.size(); j ++){
        float dist = ofDist(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
        float overlap = cur_.r + closeNei[j] -> r;
        
        if(overlap < dist){
            //            addRepulsionForce(cur_, cur_.r, 1);
            ofDrawLine(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
        }
        if(!cur_.alone) {
            cur_.r -= cur_.membraneStep;
            cur_.membraneLife ++;
        }
        else if (cur_.alone) {
            cur_.r += cur_.membraneStep;
            cur_.membraneLife -= 2;
        }
    }
}


void E1System::outputConditions(){
    
    //////// TRIGGER FOR OUTPUT////////////
    float testVal = ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 20);
    
    // run the timer for the glow effect
    glowTimer.run();
    
    
    if(outputCondition > outputThreshold) trigger = true;
    else(trigger = false);
    
    // if these conditions are met, do this once only!
    
    if(trigger && !systemOutput) {
        systemOutput = true;
        ofSetColor(0, 255, 0);
        ofDrawCircle(origin, 50);
        glowTimer.reset();
        glowTimer.endTime = 5000;
        sequenceTrigger = true;
    }
    
    // if the timer is active, glow
    if(!glowTimer.reached){
        glow = true;
        
    }
    // if not, don't glow
    if (glowTimer.reached){
        glow = false;
    }
    
    
    
}


void E1System::impactEffect(){
    
    if(impact){
        addRepulsionForce(origin.x, origin.y, 200, 3);
    }
    else{
        
    }
    
}


void E1System::particleInteractions(){
    
    //Send an output signal if a certain number of particles reach a particular size
    outputCondition = 0;
    for(int i = 0; i < particles.size(); i++) {
        
//        E1Particle& cur = particles[i];
        
        vector<Particle*> membranes = getNeighbors(particles[i].x, particles[i].y, particles[i].membraneRad + maxRad);
        
        // global force on other particles
        addRepulsionForce(particles[i], particleNeighborhood, particleRepulsion);
        // forces on this particle
        addAttractionForce(particles[i], particleNeighborhood, 0.5);
        
        particles[i].limitSize();
        particles[i].limitMembraneLife();
        particles[i].bounceOffWalls(true);
        particles[i].addDampingForce();
        
        alterSize(particles[i]);
        
        if(particles[i].r > maxRad - 5) outputCondition ++;
        
        // team swap
        for(int j = 0; j < membranes.size(); j ++){
            if(particles[i].team == !membranes[j] -> team){
                if(particles[i].membraneLife > 30 && membranes[j] -> membraneLife > 30){
                    //                    cur.col = ofColor(255, 0, 0);
                    
                    ofColor c1, c2;
                    
                    c1 = particles[i].col;
                    c2 = membranes[j] -> col;
                    
                    particles[i].col = c2;
                    membranes[j] -> col = c1;
                    
                }
            }
        }
        
        
    }
    
    
}

