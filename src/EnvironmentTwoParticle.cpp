#include "EnvironmentTwoParticle.h"

#include "ofGraphics.h"

EnvironmentTwoParticle::EnvironmentTwoParticle(float x, float y, float xv, float yv) :
x(x), y(y),
xv(xv), yv(yv) {
        
//        r = 3;
        r = ofRandom (2, 7);
//        col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    
//        xv = 1;
//    xv = 0;
//    yv = 0;
//        xv = ofRandom(-1, 1);
//        yv = ofRandom(-1, 1);
//        if(xv == 0) xv = ofRandom(-0.5, 0.5);
    
         vel = ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1));
        if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        
//        cellState = ofRandom(3);
        damping = .3;
        randomOffset = ofRandom(-5, 10);
        wallTimer = Timer();
        wallTimer.setup();
        wallTimer.endTime = 3000;
        stuckOnWall = false;
        mass = 1;
        acceleration = ofVec2f(0,0);
        maxSpeed = 5;
}

void EnvironmentTwoParticle::updatePosition(float timeStep) {
//     f = ma, m = 1, f = a, v = int(a)
   
    
    
    xv += xf;// * timeStep;
    yv += yf;// * timeStep;
    x += xv;// * timeStep;
    y += yv;// * timeStep;
    
    
//    xv += acceleration.x;
//    yv += acceleration.y;
//
//    vel.x += xf;
//    vel.y += yf;
//
//    x += vel.x;
//    y += vel.y;
//
    acceleration *=0;
}

void EnvironmentTwoParticle::resetForce() {
	xf = 0;
	yf = 0;
}

void EnvironmentTwoParticle::bounceOffWalls() {
	bool collision = false;

    ofVec2f cent = ofVec2f(328, 328);
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
        
        collision = true;
    }
}

// Function that reverses velocity when hitting an outer cell wall
void EnvironmentTwoParticle::bounceOffOuterCell(float outer){
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

        vel.x *= -1;
        vel.y *= -1;

        collision = true;
    }
}

// Function that reverses velocity when hitting an inner cell wall
void EnvironmentTwoParticle::bounceOffInnerCell(float inner){
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
        
        vel.x *= -1;
        vel.y *= -1;
        
        collision = true;
    }
}
    



void EnvironmentTwoParticle::addDampingForce() {
	xf -= xv * damping;
	yf -= yv * damping;
}

void EnvironmentTwoParticle::draw() {
    
    glVertex2f(x, y);
}


void EnvironmentTwoParticle::displayParticle(){
    ofSetColor(col);
    ofDrawCircle(x, y, r);
    returnFromWall();
}

void EnvironmentTwoParticle::receiveCells(vector <float> cells_){
    cells = cells_;
}
void EnvironmentTwoParticle::accelerateTowardsTarget(ofVec2f _target){
    
    ofVec2f target = _target;
    acceleration.x = target.x - x;
    acceleration.y = target.y - y;
    acceleration.limit(maxSpeed);
    
}

void EnvironmentTwoParticle::applyForce(ofVec2f force){
    ofVec2f f = ofPoint(force);
    f /= mass;
    xf += f.x;
    yf += f.y;
}


void EnvironmentTwoParticle::returnFromWall(){
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
