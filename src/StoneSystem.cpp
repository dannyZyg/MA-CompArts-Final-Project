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
    timeIntervalPassed = 0;
    drawLines = true;
    rebound = false;
    environmentDivision = 30;
    activateDraw = false;
    alpha = 0;
//    dimForSensorPath = false;
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
    
    for(int i = 0; i < particles.size(); i++){
        if(i >= 0 && i <= 30) particles[i].team = 0;
        if(i > 30 && i <= 60) particles[i].team = 1;
        if(i > 60 && i <= 90) particles[i].team = 2;
        if(i > 90){
            particles[i].team = 3;
            particles[i].r = ofRandom(18, 28);
//            particles[i].vel.x = ofRandom(-3, 3);
//            particles[i].vel.y = ofRandom(-3, 3);
            particles[i].damping = 0.01;
        }
    }
    
//    team1Base = ofColor(27,125, 204);
//    team2Base = ofColor(145,49, 191);
//    team3Base = ofColor(42, 211, 126);
//    team4Base = ofColor(255,211, 91);
    
    
    team1Base = ofColor(20,201, 23);
    team2Base = ofColor(242,24, 249);
    team3Base = ofColor(239, 171, 93);
    team4Base = ofColor(193, 10, 7);
    setupColours();
}

void StoneSystem::display(){
    

//    activeTimer.run();
//
//    releaseTimer.run();
//
    
    if(env1 || env2 || env3 || sens) counter = 0;
    else {
        counter ++;
        if(counter > 250) activateDraw = false;;
    }

    if(counter == 0) activateDraw = true;
    

//    activateDraw = false;
    //If a system has sent a signal, allow the following, otherwise save CPU.
    if(activateDraw){
        ofPushStyle();
        ofPushMatrix();
        
    for(int i = 90; i < particles.size(); i ++){
        particles[i].life = ofMap(ofNoise(ofGetFrameNum() * ( i * 0.0001) + (i * 100)), 0, 1, 150, 255);
    }
        
//        if(sensorDisplay > 91){
//
//
//            alpha ++;
//            if(alpha > 200) alpha = 200;
//
//            for(int i = 90; i < particles.size(); i ++){
//                vector<Particle*> nearbyParticles = getNeighbors(particles[i].x, particles[i].y, particleNeighborhood);
//
//                for(int j = 0; j < nearbyParticles.size(); j ++ ){
//                    ofPushStyle();
////                    float alpha = ofMap(ofNoise(ofGetFrameNum() * (0.01 * i) + j), 0, 1, 100, 255);
//                    ofSetLineWidth(5);
//                    ofSetColor(team4Base, alpha);
//                    ofDrawLine(particles[i].x, particles[i].y, nearbyParticles[j] -> x, nearbyParticles[j] -> y);
//                    ofPopStyle();
//                }
//            }
//
//
//        }
//
//        if(sensorDisplay < 92){
//            alpha --;
//            if(alpha < 0) alpha = 0;
//
//        }
//
//        cout << alpha << endl;

        
        
        
        
        // do this once per frame
        setupForces();
        
        // apply per-particle forces
//        if(drawLines) {
//            ofPushStyle();
//            ofSetColor(24, 124, 174);
//            ofSetLineWidth(3);
//            glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
//            ofPopStyle();
//        }
        
        // functions which fades in and out particles based on which system sends a signal
        particlesInOut(e1StartIndex, env1, env1Display);
        particlesInOut(e2StartIndex, env2, env2Display);
        particlesInOut(e3StartIndex, env3, env3Display);
        particlesInOut(sensStartIndex, sens, sensorDisplay);
        
        for(int i = 0; i < particles.size(); i++) {
            //        particles[i].displayParticle();
            Particle& cur = particles[i];
            // global force on other particles
            addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
            // forces on this particle
            
//            if(i > 90) rebound = true;
//            else rebound = false;

            cur.bounceOffWalls(rebound);
            cur.addDampingForce();
            
        }
//        if(drawLines) {
//            glEnd();
//        }
        
        

        
        
        // single-pass global forces
        addAttractionForce(origin.x, origin.y, 200, centerAttraction);

        update();
        
        ofPopStyle();
        ofPopMatrix();
        
        
//        ofDrawRectangle(origin, 100, 100);
        
    }
    
    
    timer ++;
    if(timer > 50000)timer = 0;
    
//    ofSetColor(255);
//    if(!releaseTimer.reached) ofDrawCircle(origin, 50);
//    string pct = ofToString(releaseTimer.pct);
//    ofSetColor(255, 0, 0);
//    ofDrawBitmapString(pct, origin.x, origin.y);
    
//    if(dimForSensorPath) dimParticles();
//    else brightenParticles();
    
    
}


//
//void StoneSystem::reset(){
//
//    particles.clear();
//}
//
//// Make changes based on which system sent the signal
//void StoneSystem::originSystem(string originSystem_){
//
//}


void StoneSystem::particlesInOut(int start, bool active, float& display){
    
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

void StoneSystem::pushPopParticles(){
    
    if(particles.size() < 50){
        if(timer % showParticleSpacing == 0){
            addParticle(ofRandom(4));
        }
    }
}

void StoneSystem::addParticle(int team_){
    
    Particle p = Particle();
    p.origin = origin;
    p.externalRad = externalRad;
    p.team = team_;
    p.x = origin.x;
    p.y = origin.y;
    
    particles.push_back(p);
    
}


void StoneSystem::timerForDrawDeactivation(bool& systemBool, bool& resetBool){

    if(systemBool){
        if(!resetBool){
            releaseTimer.reset();
            resetBool = true;
        }
    }
}


//void StoneSystem::dimParticles(){
//
//
//    for(int i = 0; i < particles.size() - 30; i ++){
//        particles[i].life --;
//        if(particles[i].life < 0) particles[i].life = 0;
//    }
//
//}
//
//void StoneSystem::brightenParticles(){
//    for(int i = 0; i < particles.size() - 30; i ++){
//        particles[i].life ++;
//        if(particles[i].life > 255) particles[i].life = 255;
//    }
//
//}
