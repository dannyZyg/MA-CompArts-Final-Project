//
//  E2System.cpp
//  FinalProjectMA
//
//  Created by Danny on 25/8/18.
//

#include "E2System.hpp"


E2System::E2System(){
    kParticles = 300;
    particleNeighborhood = 32;
    particleRepulsion = 0.5;
    centerAttraction = 0;
    particleAttraction = 0;
//    drawLines = true;
    cellWallsActive = true;
    lineAlpha = 0;
    outputThreshold = 80;
    
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
    
    
    glowTimer.setup();
    glow = false;
    
    outputTimer = Timer();
    outputTimer.setup();

    
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

//void E2System::update() {
//    int n = particles.size();
//    for(int i = 0; i < n; i++) {
//
//    }
//}

void E2System::particleInteractions(){
//    int numOutside = 0;
    
    for(int i = 0; i < particles.size(); i++) {
        Particle& cur = particles[i];
        
//        float d = ofDist(cur.x, cur.y, origin.x, origin.y);
//        if (d > externalRad + 10) numOutside ++;
//
        
        
//        particles[i].updatePosition();
        particles[i].receiveCells(cells);
        if(pingFromWalls) particles[i].returnFromWall();
        
        // global force on other particles
        
        addAttractionForce(cur, particleNeighborhood, particleAttraction);
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        //        cur.bounceOffWalls();
        
        cur.addDampingForce();
        
        //        if(ofGetElapsedTimeMillis() % 1000 == 0) state = !state;
        //        cout<<state<<endl;
        if(cellWallsActive){
            allocateCellState(cur);
            cellWallRebound(cur);
        }
        if(!cellWallsActive){
            cur.bounceOffWalls(true);
        }
    }
//    cout << "Num Outside " << numOutside << endl;

}

void E2System::outputConditions(){
    // TRIGGER FOR OUTPUT
    
    // run the timer for the glow effect
    glowTimer.run();
    
//    cout << cells[1] << endl;
    
    //    cout <<"trigger " <<trigger << endl;
    //    cout <<"sequenceTrigger " << sequenceTrigger <<endl;
    
//    if(cells[1] < outputThreshold) trigger = true;
    
    
    if(ofGetMouseX() < 100) trigger = true;
    else(trigger = false);
    
    //    if(outputTimer.reached) trigger = true;
    //    else if(!outputTimer.reached) trigger = false;
    
    //    cout<<"trig "<<trigger << endl;
    
    // if these conditions are met, do this once only!
    
    if(trigger && !sequenceActive) {
        ofSetColor(0, 255, 0);
        ofDrawCircle(origin, 50);
        sequenceTrigger = true;
    }
    
    
//    // if the timer is active, glow
//    if(!glowTimer.reached){
//        glow = true;
//        
//    }
//    // if not, don't glow
//    if (glowTimer.reached){
//        glow = false;
//    }
    
    // reset the cluster tally
    outputCondition = 0;
    
    
    
    
    
}


void E2System::impactEffect(){
    
    ofVec2f target;
    target.x = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, origin.x - externalRad/2, origin.x + externalRad/2, true);
    target.y = ofMap(sin(ofGetFrameNum() * 0.01 + 654), -1, 1, origin.y - externalRad/2, origin.y + externalRad/2, true);
    
//    ofDrawCircle(target, 50);
    
    if(impact){
        addRepulsionForce(target.x, target.y, 250, 1);

    }
    else{

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

//void E2System::newRules(int option){
//    
//    // predefined behaviours
//    if(option == 0) presetSelector("p1");
//    if(option == 1) presetSelector("p2");
//    if(option == 2) presetSelector("p3");
//    
//    //random behaviours
//    if(option == 3) presetSelector("r1");
//    if(option == 4) presetSelector("r2");
//    if(option == 5) presetSelector("r3");
//}

void E2System::presetSelector(string preset){
    
    // cell walls active. Outside cells retreat from outer wall.
    if(preset == "p1"){
        particleRepulsion = 0.5;
        centerAttraction = 0;
        cellWallsActive = true;
        pingFromWalls = true;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 0.4;
            
            
        }
        cout<< "P1" << endl;
    }
    
    if(preset == "p2"){
        pingFromWalls = true;
        cellWallsActive = false;
        particleRepulsion = 0.5;// 0.5;
        centerAttraction = 0;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 5;
        }
        cout<< "P2" << endl;
    }
    
    if(preset == "p3"){
        
        cellWallsActive = false;
        particleRepulsion = 0.4;
        particleAttraction = 0.3;
        pingFromWalls = false;
        
        
        //        cout<<particles[0].xv<<endl;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 10;
            //            particles[i].vel.x = 3;
            //            particles[i].vel.y = 3;
            //            particles[i].vel.x = ofMap(ofSignedNoise(ofGetFrameNum() + i * 25), -1, 1, -2, 2);
            //            particles[i].vel.y = ofMap(ofSignedNoise(ofGetFrameNum() + i * 25 + 500), -1, 1, -2, 2);
            //            particles[i].xv = ofRandom(-2, 2);
            //            particles[i].yv = ofRandom(-2, 2);
        }
        //        numActive = 200;
        //        maxParticles = 150;
        
        cout<< "P3" << endl;
    }
    
    if(preset == "r1"){
        randomVals = true;
        cout<< "R1" << endl;
    }
    
    if(preset == "r2"){
        
    }
    
    if(preset == "r3"){
        
    }
    
}

void E2System::seedWithRandomValues(){
    if(randomVals){
        //change rules
        
//        particleRepulsion = ofRandom(0.1, 1.);
//        particleAttraction = ofRandom(0.1, 0.5);
//        centerAttraction = ofRandom(0, 0.5);
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = ofRandom(3);
//            particles[i].vel.x = ofRandom(-2, 2);
//            particles[i].vel.y = ofRandom(-2, 2);
            
        }
        int r1 = ofRandom(2);
        int r2 = ofRandom(2);
        if(r1 == 1) pingFromWalls = true;
        if(r1 == 0) pingFromWalls = false;
        if(r2 == 1) cellWallsActive = true;
        if(r2 == 0) cellWallsActive = false;
        
        cout<< particleRepulsion <<endl;
        cout<< "ping " << pingFromWalls << endl;
        cout <<"cellWallsActive " << cellWallsActive <<endl;
        
        randomVals = false;
    }
}



//void E2System::animateCellWalls(){
//
//    for(int i = 0; i < cells.size(); i ++){
//        float noise = ofSignedNoise(ofGetFrameNum() * 0.01 + noiseSeed[i]) * 1.2;
//        cells[i] += noise;
//    }
//}
//
//void E2System::drawCells(){
//    //  draw the noisy circle guide
//    ofPushStyle();
//    for(int i = 0; i < cells.size(); i ++){
//        ofSetColor(255);
//        ofNoFill();
//        ofDrawCircle(origin, cells[i]);
//        ofDrawBitmapString(ofToString(i), origin.x + cells[i], origin.y);
//    }
//    ofPopStyle();
//
//}
