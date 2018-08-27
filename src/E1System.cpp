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
    centerAttraction = 0; //0.6;
    drawLines = true;
    impact = false;
    colourExchange = true;
    maxRad = 20;
    
    outputThreshold = 10;
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
    
    
    
    team1Base = ofColor(27,125, 204);
    team2Base = ofColor(0,125, 90);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}

//void E1System::update() {
//    int n = particles.size();
//    for(int i = 0; i < n; i++) {
//        particles[i].updatePosition();
////        particles[i].membraneRad = region;
//    }
//}

void E1System::particleInteractions(){
    
    //Send an output signal if a certain number of particles reach a particular size
    outputCondition = 0;
    
    float sinsz = ofMap(sin(ofGetFrameNum() * 0.1), -1, 1, -10, 10);
//    cout <<sinsz <<endl;
    for(int i = 0; i < particles.size(); i++) {
    
        
        
        // alter size
        
//        alterSize(particles[i]);
        
        
    
        

        
        particles[i].limitSize();
        particles[i].limitMembraneLife();
        particles[i].bounceOffWalls(true);
        particles[i].addDampingForce();
        
//        alterSize(particles[i]);
        
        
//                if(!particles[i].alone) {
//                    particles[i].r -= 0.1;
//                    particles[i].membraneLife ++;
//                }
//                else if (particles[i].alone) {
//                    particles[i].r += 2;
//                    particles[i].membraneLife -= 2;
//                }
//        cout<<particles[i].alone<<endl;

        
        
        if(particles[i].r > maxRad - 5) outputCondition ++;
        
        
        
        // team swap
        vector<Particle*> membranes = getNeighbors(particles[i].x, particles[i].y, region);// particles[i].membraneRad + maxRad);
        
//        cout<<particles[i].membraneLife<<endl;
        
        for(int j = 0; j < membranes.size(); j ++){
            
//            cout<< "membrane size" << membranes.size()<< endl;
            // global force on other particles
            addRepulsionForce(particles[i], particleNeighborhood, particleRepulsion);
            // forces on this particle
            
            if(membranes.size() < 3){
                addAttractionForce(particles[i], particleNeighborhood, particleAttraction);
            }
            
            
            
            if(particles[i].team == !membranes[j] -> team){
                if(particles[i].membraneLife > 15 && membranes[j] -> membraneLife > 15){
                    //                    cur.col = ofColor(255, 0, 0);
//                    ofSetColor(255, 0, 0);
//                    ofDrawLine(particles[i].x, particles[i].y, membranes[j] -> x, membranes[j] -> y);

//                    ofColor c1, c2;
//
//                    c1 = particles[i].col;
//                    c2 = membranes[j] -> col;
//
//                    particles[i].col = c2;
//                    membranes[j] -> col = c1;
//
                }
            }
        }
//        particles[i].r += sinsz;
    }
    
//    cout <<"clusters " <<clusterCount << endl;
}



void E1System::drawMembranes(){
    
//    int nearby;
//
//    maxRad = particles[0].maxSize;
////    vector<Particle*> closeNei = getNeighbors(cur_.x, cur_.y, region + 10);
//
//    ofPushStyle();
//    ofFill();
//    for(int i = 0; i < particles.size(); i ++){
//        region = particles[i].r + maxRad;
//        ofSetColor(255);
//        ofDrawCircle(particles[i].x, particles[i].y, region);
//    }
//
//    ofPopStyle();
    
    
    for(int i = 0; i < particles.size(); i ++){
        alterSize(particles[i]);
    }
}





void E1System::alterSize(Particle& cur_){
    
    int nearby;
    region = 45;
    
    maxRad = particles[0].maxSize;
    vector<Particle*> closeNei = getNeighbors(cur_.x, cur_.y, region);
    
    //alter size
    
    //test
    nearby = closeNei.size();
    
//    cout<<nearby<<endl;
    
    ofPushStyle();
    ofFill();
    ofSetColor(125, cur_.membraneLife);
    if(nearby > 1){
        ofDrawCircle(cur_.x, cur_.y, cur_.membraneRad);
        cur_.alone = false;
        //        addAttractionForce(cur_, region, 0.2);
    }
    else{
        cur_.alone = true;
    }
    
    
//    cout<< cur_.membraneLife<<endl;
    
    ofPopStyle();
    
    
    for(int j = 0; j < closeNei.size(); j ++){
        float dist = ofDist(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
        float overlap = cur_.r + closeNei[j] -> r;
        
        if(overlap < dist){
            //            addRepulsionForce(cur_, cur_.r, 1);
//            ofDrawLine(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
            
           
            //swap colours if in range
            if(colourExchange){
                ofColor c1, c2;
                
                c1 = cur_.col;
                c2 = closeNei[j] -> col;
                
                cur_.col = c2;
                closeNei[j] -> col = c1;
            }
  
            
        }
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


void E1System::outputConditions(){
    
    //////// TRIGGER FOR OUTPUT////////////
    float testVal = ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 20);
    
    // run the timer for the glow effect
    glowTimer.run();
    
    
    if(calibration){
        if(outputCondition > outputThreshold) trigger = true;
        else(trigger = false);
        
        // if these conditions are met, do this once only!
        
        if(trigger && !sequenceActive) {
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
    
    
    
    
}


void E1System::impactEffect(){
    
    if(impact){
        addRepulsionForce(origin.x, origin.y, 200, 3);
    }
    else{
        
    }
    
}



void E1System::seedWithRandomValues(){
    if(randomVals){
        //change rules
        
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
        }
        
        
   

        
        cout<< particleRepulsion <<endl;

        
        randomVals = false;
        colourExchange = ofRandom(2);
    }
}





