//
//  E3System.cpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//

#include "E3System.hpp"

E3System::E3System(){
    kParticles = 100;
    particleNeighborhood = 64;
    particleRepulsion = 0.3;
    centerAttraction = 0;
//    drawLines = true;
    angle = 0;
    impactTarget = ofVec2f(origin.x + 100, origin.y + 100);
    
//    lineAlpha = 0;
    
    outputThreshold = 100;
    drawLines = true;
    rebound = true;
    trigger = false;
    glow = false;

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
    
    team1Base = ofColor(52,167, 173);
    team2Base = ofColor(255,211, 91);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}

void E3System::particleInteractions(){
    
    
    
    for(int i = 0; i < particles.size(); i++) {
//        Particle& particles[i] = particles[i];
        // global force on other particles
        
//        cout <<particles[i].team << endl;
        vector<Particle*> nei = getNeighbors(particles[i].x, particles[i].y, 50);
        
        vector<Particle*> clusters = getNeighbors(particles[i].x, particles[i].y, 10);
        
        vector<Particle*> global = getNeighbors(particles[i].x, particles[i].y, 80);//externalRad/3);
        
        for(int j = 0; j < nei.size(); j ++){
            
            addRepulsionForce(particles[i], particleNeighborhood, 0.04);
            
            
            if(particles[i].team != nei[j] -> team){
                ofPushStyle();
//                ofSetColor(255, 0, 0);
//                ofDrawLine(particles[i].x, particles[i].y, nei[j] -> x, nei[j] -> y);
                addRepulsionForce(particles[i], particleNeighborhood, particleRepulsion);
                ofPopStyle();
            }
            
            if(particles[i].team == nei[j] -> team){
                
                float d1 = ofDist(particles[i].x, particles[i].y, origin.x, origin.y);
                float d2 = ofDist(nei[j] -> x, nei[j] -> y, origin.x, origin.y);

                if(d1 <= externalRad && d2 <= externalRad && clusterCount < 350){
                    addAttractionForce(particles[i], particleNeighborhood, 0.06);
                }
                
                
                
                ofPushStyle();
//                ofSetColor(0, 255, 0);
//                ofDrawLine(particles[i].x, particles[i].y, nei[j] -> x, nei[j] -> y);
//                addRepulsionForce(particles[i], particleNeighborhood, particleRepulsion);
                ofPopStyle();
                
            }
        }
        
        //OUTPUT CONDITION
        //Send an output if the particles are clusered to a certain degree
        
        for(int j = 0; j < clusters.size(); j ++){
            if(particles[i].team == clusters[j] -> team){
                float d = ofDist(particles[i].x, particles[i].y, clusters[j] -> x, clusters[j] -> y);
                if(d < 10){
                    outputCondition ++;
                }
            }
        }
        
        //        cout<< clusterCount << endl;
        
        for(int j = 0; j < global.size(); j ++){
            if(glow){
                ofPushStyle();
                float alpha = ofMap(sin(ofGetFrameNum() * 0.01 + noiseSeed[i]), -1, 1, 0, 200);
                ofColor c = particles[i].col;
                ofSetColor(c, alpha);
//                ofDrawLine(particles[i].x, particles[i].y, global[j] -> x, global[j] -> y);
                ofPopStyle();
            }
        }
        
        // forces on this particle
        particles[i].bounceOffWalls(rebound);
        particles[i].addDampingForce();
        
    }
    
    //    cout<< "e3 output con" << outputCondition << endl;
    
}

void E3System::drawTeamLines(){
    
    for(int i = 0; i < particles.size(); i++) {

        vector<Particle*> nei = getNeighbors(particles[i].x, particles[i].y, 50);
        

        for(int j = 0; j < nei.size(); j ++){
            
            if(particles[i].team != nei[j] -> team){
                ofPushStyle();
                ofSetColor(255);
                ofDrawLine(particles[i].x, particles[i].y, nei[j] -> x, nei[j] -> y);
                ofPopStyle();
            }
            
            if(particles[i].team == nei[j] -> team){
                ofPushStyle();
//                ofSetColor(0, 255, 0);
//                ofDrawLine(particles[i].x, particles[i].y, nei[j] -> x, nei[j] -> y);
                ofPopStyle();
                
            }
        }
    
    }
}

void E3System::outputConditions(){
    // TRIGGER FOR OUTPUT
    
    // run the timer for the glow effect
    glowTimer.run();
//    if(calibration){
        if(outputCondition > outputThreshold) trigger = true;
        else(trigger = false);
        
        //    if(outputTimer.reached) trigger = true;
        //    else if(!outputTimer.reached) trigger = false;
        
        //    cout<<"trig "<<trigger << endl;
        
        // if these conditions are met, do this once only!
        
        if(trigger && !sequenceActive) {
            systemOutput = true;
            ofSetColor(0, 255, 0);
            ofDrawCircle(origin, 50);
            glowTimer.reset();
            glowTimer.endTime = 5000;
            sequenceTrigger = true;
            outputTimer.reset();
        }
        
        
        // if the timer is active, glow
        if(!glowTimer.reached){
            glow = true;
            
        }
        // if not, don't glow
        if (glowTimer.reached){
            glow = false;
        }
        
        // reset the cluster tally
        outputCondition = 0;
        
        
        
//    }
    
    
}


void E3System::impactEffect(){
    //    ofPushStyle();
    
    
    impactTarget.x = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01), -1, 1, origin.x - 300, origin.x + 300);
    impactTarget.y = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 500), -1, 1, origin.y - 300, origin.y + 300);
    
    
    //    ofSetColor(255, 0, 0   );
    if(impact){
        //              ofDrawLine(0, 0, 100, 100);
        //        ofDrawCircle(impactTarget, 200);
        addRepulsionForce(impactTarget.x, impactTarget.y, 200, 1);

    }
    else{
    }
  
}


