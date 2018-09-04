//
//  E3System.cpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//

#include "E3System.hpp"

E3System::E3System(){
    kParticles = 110;
    particleNeighborhood = 64;
    particleRepulsion = 0.3;
    centerAttraction = 0;
    impactTarget = ofVec2f(origin.x + 100, origin.y + 100);
    
    outputThreshold = 250;
    drawLines = true;
    rebound = true;
    trigger = false;

}
void E3System::setupParticles(){
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        Particle particle = Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particle.setupE3();
        particles.push_back(particle);
    }

    // setup colours for two teams
    team1Base = ofColor(52,167, 173);
    team2Base = ofColor(255,211, 91);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}

void E3System::particleInteractions(){

    outputCondition = 0;

    for(int i = 0; i < particles.size(); i++) {

        vector<Particle*> nei = getNeighbors(particles[i].x, particles[i].y, 50);
        
        for(int j = 0; j < nei.size(); j ++){
            
            addRepulsionForce(particles[i], particleNeighborhood, 0.04); // general repulsion force for all nearby particles
            
            // unique repulsion force for all nearby particles on the opposite team
            if(particles[i].team != nei[j] -> team){
                addRepulsionForce(particles[i], particleNeighborhood, particleRepulsion);
            }
            
            //Cluster Bheaviour
            // If particles belong to the same team...
            // bring particles together.
            if(particles[i].team == nei[j] -> team){
                
                //distance calculation used to keep clusters relatively central.
                float d1 = ofDist(particles[i].x, particles[i].y, origin.x, origin.y);
                float d2 = ofDist(nei[j] -> x, nei[j] -> y, origin.x, origin.y);

                if(d1 <= externalRad && d2 <= externalRad && clusterCount < 350){
                    addAttractionForce(particles[i], particleNeighborhood, 0.06);
                }
            }
        }
        
        //OUTPUT CONDITION
        //increment the ouput condition if the particles are clusered to a certain degree

        vector<Particle*> clusters = getNeighbors(particles[i].x, particles[i].y, 10);
        
        for(int j = 0; j < clusters.size(); j ++){
            if(particles[i].team == clusters[j] -> team){
                float d = ofDist(particles[i].x, particles[i].y, clusters[j] -> x, clusters[j] -> y);
                if(d < 10){
                    outputCondition ++;
                }
            }
        }
        
        // forces on this particle
        particles[i].bounceOffWalls(rebound);
        particles[i].addDampingForce();
        
    }
    
    if(outputCondition > outputThreshold) trigger = true;
    else(trigger = false);
}

//function called from environment three source
void E3System::drawTeamLines(){
    
    for(int i = 0; i < particles.size(); i++) {

        // find out which particles are nearby
        vector<Particle*> nei = getNeighbors(particles[i].x, particles[i].y, 50);

        for(int j = 0; j < nei.size(); j ++){
            
            // draw connecting lines if the teams are opposite
            if(particles[i].team != nei[j] -> team){
                ofPushStyle();
                ofSetColor(255);
                ofDrawLine(particles[i].x, particles[i].y, nei[j] -> x, nei[j] -> y);
                ofPopStyle();
            }
        }
    }
}

void E3System::impactEffect(){
    
    
    impactTarget.x = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01), -1, 1, origin.x - 300, origin.x + 300);
    impactTarget.y = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 500), -1, 1, origin.y - 300, origin.y + 300);
    
    
    if(impact){
        addRepulsionForce(impactTarget.x, impactTarget.y, 200, 1);

    }
}


void E3System::presetSelector(string preset){
    
    if(preset == "p1"){
        particleRepulsion = 0.5;
        centerAttraction = 0.9;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 0.4;
        }
    }
    
    if(preset == "p2"){
        particleRepulsion = 0.5;
        centerAttraction = 0;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 5;
        }
    }
    
    if(preset == "p3"){
        
        particleRepulsion = 0.4;
        particleAttraction = 0.3;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 10;
        }
    }
    
    if(preset == "r1"){
        randomVals = true;
    }
}

void E3System::seedWithRandomValues(){
    if(randomVals){
        //change rules
        particleRepulsion = ofRandom(0.1, 1.);
        particleAttraction = ofRandom(0.1, 0.5);
        centerAttraction = ofRandom(0, 0.5);
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = ofRandom(3);
            particles[i].vel.x = ofRandom(-2, 2);
            particles[i].vel.y = ofRandom(-2, 2);
            
        }
     
        randomVals = false;
    }
}


