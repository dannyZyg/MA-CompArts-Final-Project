//
//  E2System.cpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//

#include "E2System.hpp"


E2System::E2System(){
    kParticles = 350;
    particleNeighborhood = 32;
    particleRepulsion = 0.5;
    centerAttraction = 0;
    particleAttraction = 0;
    cellWallsActive = true;
    lineAlpha = 0;
    outputThreshold = 80;
    
    sequenceActive = false;
    sequenceTrigger = false;
    
}


void E2System::setupParticles(){
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        Particle particle = Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particle.setupE2();
        particles.push_back(particle);
    }
    
    team1Base = ofColor(145,49, 191);
    team2Base = ofColor(0,49, 220);
    team3Base = ofColor(255,49, 191);
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
    
    
    int num = 3;
    for(int i = 0; i < num; i ++){
        float r = externalRad / num * i;
        cells.push_back(r);
        
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}

void E2System::updateColours(){
    
    for(int i = 0; i < particles.size(); i ++){
        
        float d = ofDist(particles[i].x, particles[i].y, origin.x, origin.y);
        
        if(d > 0 && d < cells[1]){
            ofxColorPalette c = teamCols[0];
            particles[i].col = ofColor(c[particles[i].colIndex]);
        }
        else if(d > cells[1] && d < cells[2]){
            ofxColorPalette c = teamCols[1];
            particles[i].col = ofColor(c[particles[i].colIndex]);
        }
        else if(d > cells[2] && d < externalRad){
            ofxColorPalette c = teamCols[2];
            particles[i].col = ofColor(c[particles[i].colIndex]);
        }
    }
}

void E2System::particleInteractions(){
        
    for(int i = 0; i < particles.size(); i++) {
        Particle& cur = particles[i];
        
        particles[i].receiveCells(cells);
        if(pingFromWalls) particles[i].returnFromWall();
        
        // global force on other particles
        
        addAttractionForce(cur, particleNeighborhood, particleAttraction);
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        
        // forces on this particle
        cur.addDampingForce();
        
        if(cellWallsActive){
            allocateCellState(cur);
            cellWallRebound(cur);
        }
        if(!cellWallsActive){
            cur.bounceOffWalls(true);
        }
    }
    
    // TRIGGER FOR OUTPUT
    
    // if size of the inner cell exceeds 100, send out a signal
    if(cells[1] > 100) trigger = true;
    else(trigger = false);

}



void E2System::impactEffect(){
    
    ofVec2f target;
    target.x = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, origin.x - externalRad/2, origin.x + externalRad/2, true);
    target.y = ofMap(sin(ofGetFrameNum() * 0.01 + 654), -1, 1, origin.y - externalRad/2, origin.y + externalRad/2, true);
    
    
    if(impact){
        addRepulsionForce(target.x, target.y, 250, 1);
    }

}


void E2System::receiveCells(vector <float> cells_){
    cells = cells_;
    
    scaledCells = cells_;
    
    for(int i = 0; i < cells.size(); i ++){
        scaledCells[i] = ofMap(scaledCells[i], -50, 50, 0, 1);
    }    
}


void E2System::cellWallRebound(Particle& particle){
    
    // if statements telling the particles to stay within their allocated cell walls
    
    if(particle.cellState == 0){
        particle.bounceOffOuterCell(cells[1]);
    }
    
    else if(particle.cellState == 1){
        particle.bounceOffInnerCell(cells[1]);
        particle.bounceOffOuterCell(cells[2]);
    }
    else if(particle.cellState == 2){
        particle.bounceOffInnerCell(cells[2]);
        particle.bounceOffWalls(true);
    }
    
}

void E2System::allocateCellState(Particle& particle){
    
    // allocate particles to cells based on their position
    float d = ofDist(particle.x, particle.y, origin.x, origin.y);
    if(d > 0 && d < cells[1]){
        particle.cellState = 0;
    }
    else if(d > cells[1] && d < cells[2]){
        particle.cellState = 1;

    }
    else if(d > cells[2] && d < externalRad){
        particle.cellState = 2;
    }
}

void E2System::presetSelector(string preset){
    
    if(preset == "p1"){
        particleRepulsion = 0.5;
        centerAttraction = 0;
        cellWallsActive = true;
        pingFromWalls = true;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 0.4;
            particles[i].damping = 0.1;
        }
    }
    
    if(preset == "p2"){
        pingFromWalls = true;
        cellWallsActive = false;
        particleRepulsion = 0.5;
        centerAttraction = 0;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 5;
            particles[i].damping = 0.1;

        }
    }
    
    if(preset == "p3"){
        
        cellWallsActive = false;
        particleRepulsion = 0.4;
        particleAttraction = 0.3;
        pingFromWalls = false;

        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 10;
            particles[i].damping = 0.07;
        }
    }
    
    if(preset == "r1"){
        randomVals = true;
    }
}

void E2System::seedWithRandomValues(){
    if(randomVals){
        //change rules

        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = ofRandom(3);
            particles[i].damping = ofRandom(0.04, 0.08);
        }
        
        int r1 = ofRandom(2);
        int r2 = ofRandom(2);
        if(r1 == 1) pingFromWalls = true;
        if(r1 == 0) pingFromWalls = false;
        if(r2 == 1) cellWallsActive = true;
        if(r2 == 0) cellWallsActive = false;

        randomVals = false;
    }
}
