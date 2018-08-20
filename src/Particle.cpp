#include "Particle.h"
#include "ofGraphics.h"

Particle::Particle(float x_, float y_){
    x = x_;
    y = y_;
    xv = ofRandom(-1, 1);
    yv = ofRandom(-1, 1);
    
    r = ofRandom (4, 12);
}

Particle::Particle(){
    x = ofRandom(-1, 1);;
    y = ofRandom(-1, 1);
    
    xv = ofRandom(-1, 1);
    yv = ofRandom(-1, 1);
    
    r = ofRandom (4, 12);
    
    
}

void Particle::updatePosition() {
//     f = ma, m = 1, f = a, v = int(a)
    
    xv += xf;
    yv += yf;
    x += xv;
    y += yv;

}

void Particle::draw() {
    
    glVertex2f(x, y);
}

void Particle::displayParticle(){
    //    ofNoFill();
    ofSetColor(col);
    ofDrawCircle(x, y, r);
}

void Particle::resetForce() {
	xf = 0;
	yf = 0;
}

void Particle::applyForce(ofVec2f force){
    ofVec2f f = ofPoint(force);
    f /= mass;
    xf += f.x;
    yf += f.y;
}

void Particle::accelerateTowardsTarget(ofVec2f _target){
    
    ofVec2f target = _target;
    acceleration.x = target.x - x;
    acceleration.y = target.y - y;
    acceleration.limit(maxSpeed);
    
}

void Particle::addDampingForce(float damping) {
    if(xv > 0.5) xf -= xv * damping;
    if(yv > 0.5) yf -= yv * damping;
}



//////// Environment 2 Particle Functions////////////////////////


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
        //        ofDrawLine(origin.x, origin.y, x, y);
        //        ofPoint cent = ofPoint(origin)
        //        applyForce(- origin);
        //        accelerateTowardsTarget(origin);
        
        
    }
}
