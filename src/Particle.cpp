#include "Particle.h"
#include "ofGraphics.h"

Particle::Particle(){
    r = ofRandom (2, 7);
    x = ofRandom(100, 500);
    y = ofRandom(100, 500);
    
    xv = ofRandom(-0.5, 0.5);
    yv = ofRandom(-0.5, 0.5);
    
    
    vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    
    
    damping = .01;
    randomOffset = ofRandom(-5, 10);
    mass = 1;
    acceleration = ofVec2f(0,0);
    maxSpeed = 5;
    
}

E1Particle::E1Particle(){

    
}

E2Particle::E2Particle(){
    xv = ofRandom(-1, 1);
    yv = ofRandom(-1, 1);
    wallTimer = Timer();
    wallTimer.setup();
    wallTimer.endTime = 3000;
    stuckOnWall = false;
    damping = .02;
    
}

E3Particle::E3Particle(){
    r = ofRandom (4, 12);
    xv = ofRandom(-1, 1);
    yv = ofRandom(-1, 1);
    team = ofRandom(2);
    damping = .01;
    
}


void Particle::updatePosition() {
    //     f = ma, m = 1, f = a, v = int(a)
    
    xv += xf;// apply forces
    yv += yf;// apply forces
    x += xv;//
    y += yv;//
    
    
    xv += acceleration.x;
    yv += acceleration.y;
    
    //
    //    vel.x += xf;
    //    vel.y += yf;
    //
//    x += vel.x;
//    y += vel.y;
    
    
    // Add friction
    friction.x = xv;
    friction.y = yv;
    friction.normalize();
    cF = 0.1;
    friction *= cF;
    applyForce(friction);
    
    acceleration *= 0;
    
}

void Particle::resetForce() {
    xf = 0;
    yf = 0;
}

void Particle::bounceOffWalls() {
    bool collision = false;
    
    int d = ofDist(x, y, origin.x, origin.y);
    if( (d > externalRad - r )){
        
        ofVec2f out = ofVec2f(x, y);
        ofVec2f ret = origin - out;
        ret = ret.normalize();
        
        x += ret.x;
        y += ret.y;
        
        
        xv *= -1;
        yv *= -1;
        
        vel.x *= -1;
        vel.y *= -1;
        
//        vel.x *= -1;
//        vel.y *= -1;
        
        collision = true;
        
//        addDampingForce();
    }
}


void Particle::addDampingForce() {
    xf -= xv * damping;
    yf -= yv * damping;
}

void Particle::draw() {
    
    glVertex2f(x, y);
}


void Particle::displayParticle(){
    ofSetColor(col);
    ofDrawCircle(x, y, r);
    
}

void Particle::accelerateTowardsTarget(ofVec2f _target){
    
    ofVec2f target = _target;
    acceleration.x = target.x - x;
    acceleration.y = target.y - y;
    acceleration.limit(maxSpeed);
    
}

void Particle::applyForce(ofVec2f force){
    ofVec2f f = ofPoint(force);
    f /= mass;
    acceleration += f;
//    yv += f.y;
}
// Function that reverses velocity when hitting an outer cell wall
void E2Particle::bounceOffOuterCell(float outer){
    bool collision = false;
    
    float d = ofDist(x, y, origin.x, origin.y);
    if( (d > outer - r)){
        
        ofVec2f posAtEdge = ofVec2f(x, y);
        ofVec2f ret = origin - posAtEdge;
        ret = ret.normalize();
        
        x += ret.x;
        y += ret.y;
        
        xv *= -1;
        yv *= -1;
        
        collision = true;
    }
}

// Function that reverses velocity when hitting an inner cell wall
void E2Particle::bounceOffInnerCell(float inner){
    bool collision = false;
    
    float d = ofDist(x, y, origin.x, origin.y);
    if( (d < inner + r)){
        
        ofVec2f posAtEdge = ofVec2f(x, y);
        ofVec2f ret = origin - posAtEdge;
        ret *= -1;
        ret = ret.normalize();
        
        x += ret.x;
        y += ret.y;
        
        xv *= -1;
        yv *= -1;
        
        collision = true;
    }
}

void E2Particle::receiveCells(vector <float> cells_){
    cells = cells_;
}

void E2Particle::returnFromWall(){
    float distFromOrigin = ofDist(origin.x, origin.y, x, y);
    //    distFromWall = ofDist(externalRad, distFromOrigin);
    bool trigger;
    
    if(distFromOrigin > (externalRad - 50)){
        //        trigger = true;
        stuckOnWall = true;
    }
    else stuckOnWall = false;
    
    //    if(distFromOrigin < (externalRad - 10)){
    //        trigger = false;
    //    }
    //
    //    if(trigger == true){
    //        wallTimer.run();
    //    }
    //
    //    if(wallTimer.reached) stuckOnWall = true;
    //    else if(!wallTimer.reached) stuckOnWall - false;
    
    
    if(stuckOnWall){
        ofSetColor(255);
//                ofDrawLine(origin.x, origin.y, x, y);
        //        ofPoint cent = ofPoint(origin)
        //        applyForce(- origin);
                accelerateTowardsTarget(origin);
        
        
    }
}
