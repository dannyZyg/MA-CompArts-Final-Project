////
////  E1System.cpp
////  FinalProjectMA
////
////  Created by Danny on 25/8/18.
////
//
// 


#include "E1System.hpp"

E1System::E1System(){
   
    kParticles = 90;
    particleNeighborhood = 80;
    particleRepulsion = 0.9;
    centerAttraction = 0;
    drawLines = true;
    impact = false;
    colourExchange = true;
    maxRad = 20;
    outputThreshold = 10; //75;
    outputCondition = 0;
}

void E1System::setupParticles(){
    
    for(int i = 0; i < kParticles; i++) {
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        Particle particle = Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particle.setupE1();
        particles.push_back(particle);
    }
    
// setup colours for two teams
    team1Base = ofColor(0,40, 150);
    team2Base = ofColor(15,125, 27);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}

void E1System::particleInteractions(){
    
    outputCondition = 0;
    
    float sinsz = ofMap(sin(ofGetFrameNum() * 0.1), -1, 1, -10, 10);
    for(int i = 0; i < particles.size(); i++) {
        particles[i].limitSize(); // limit size of particle when shrinking or growing
        particles[i].limitMembraneLife(); // limit membrane visibility to max
        particles[i].bounceOffWalls(true);
        particles[i].addDampingForce();
        
        //add to the output condition if the particle's membrane is active above 50;
        if(particles[i].membraneLife > 50) outputCondition ++;
        
        // get neighbours around the membranes.
        vector<Particle*> membranes = getNeighbors(particles[i].x, particles[i].y, region);
        
        for(int j = 0; j < membranes.size(); j ++){
            
            // global force on other particles
            addRepulsionForce(particles[i], particleNeighborhood, particleRepulsion);
            
            // forces on this particle
            // if particle has few neighbours, attract others.
            if(membranes.size() < 3){
                addAttractionForce(particles[i], particleNeighborhood, particleAttraction);
            }
        }
    }
    
    //////// TRIGGER FOR OUTPUT////////////
    
    if(outputCondition > outputThreshold) trigger = true;
    else(trigger = false);
}



void E1System::drawMembranes(){
    for(int i = 0; i < particles.size(); i ++){
        alterSize(particles[i]);
    }
}

void E1System::alterSize(Particle& cur_){
    
    int nearby;
    region = 45;
    
    maxRad = particles[0].maxSize;
    vector<Particle*> closeNei = getNeighbors(cur_.x, cur_.y, region);
    nearby = closeNei.size();
    
    //draw the membranes and classify as alone/ not alone
    ofPushStyle();
    ofFill();
    ofSetColor(125, cur_.membraneLife);
    if(nearby > 1){
        ofDrawCircle(cur_.x, cur_.y, cur_.membraneRad);
        cur_.alone = false;
    }
    else{
        cur_.alone = true;
    }
    
    ofPopStyle();
    
    
    //alter size
    for(int j = 0; j < closeNei.size(); j ++){
        float dist = ofDist(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
        float overlap = cur_.r + closeNei[j] -> r;
        
        if(overlap < dist){
            
            //swap colours if in range
            if(colourExchange){
                ofColor c1, c2;
                
                c1 = cur_.col;
                c2 = closeNei[j] -> col;
                
                cur_.col = c2;
                closeNei[j] -> col = c1;
            }
        }
        
        // grow if alone, shrink if not alone
        // membranes should thrive when not alone.
        
        if(!cur_.alone) {
            cur_.r -= cur_.membraneStep;
            cur_.membraneLife += cur_.membraneStep;
        }
        else if (cur_.alone) {
            cur_.r += cur_.membraneStep;
            cur_.membraneLife -= cur_.membraneStep;
        }
    }
    
}



void E1System::impactEffect(){

    if(impact){
        addRepulsionForce(origin.x, origin.y, 150, 3);
    }
    
}


void E1System::presetSelector(string preset){
    
    if(preset == "p1"){
        
        colourExchange = true;
        particleRepulsion = 0.5;
        centerAttraction = 0;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 0.4;
            particles[i].damping = 0.01;
            particles[i].membraneRad = 20;
            particles[i].membraneStep = 1;
            particles[i].maxMembraneLife = 200;
            particles[i].maxSize = 25;
            particles[i].minSize = 5;
        }
    }
    
    if(preset == "p2"){
        particleRepulsion = 0.5;
        centerAttraction = 0;
        colourExchange = true;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 5;
            particles[i].damping = 0.09;
            particles[i].membraneRad = 20;
            particles[i].membraneStep = 1;
            particles[i].maxMembraneLife = 200;
            particles[i].maxSize = 25;
            particles[i].minSize = 5;
        }
    }
    
    if(preset == "p3"){
        
        colourExchange = false;
        particleRepulsion = 0.4;
        particleAttraction = 0.3;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 10;
            particles[i].membraneRad = 25;
            particles[i].membraneStep = 0.1;
            particles[i].maxMembraneLife = 100;
            particles[i].damping = ofRandom(0.07);
            particles[i].maxSize = 20;
            particles[i].minSize = 5;
        }
    }
    
    if(preset == "r1"){
        randomVals = true;
    }
}

void E1System::seedWithRandomValues(){
    if(randomVals){
        //change rules with random vals
        float tempMemStep = ofRandom(0.01, 0.5);
        float tempMemRad = ofRandom(5, 25);
        float tempMaxMemLife = ofRandom(20, 150);
        
        particleRepulsion = ofRandom(0.1, 1.2);
        particleAttraction = ofRandom(0.1, 0.4);
        centerAttraction = ofRandom(-0.2 , 0.4);
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = ofRandom(3);
            particles[i].vel.x = ofRandom(-2, 2);
            particles[i].vel.y = ofRandom(-2, 2);
            particles[i].membraneRad = tempMemRad;
            particles[i].membraneStep = tempMemStep;
            particles[i].maxMembraneLife = tempMaxMemLife;
            particles[i].damping = ofRandom(0.01, 0.05);
            particles[i].maxSize = ofRandom(15, 30);
            particles[i].minSize = ofRandom(4, 9);
        }
        randomVals = false;
        colourExchange = ofRandom(2);
    }
}





