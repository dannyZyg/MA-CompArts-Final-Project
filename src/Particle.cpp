#include "Particle.h"
#include "ofGraphics.h"

Particle::Particle(){
    r = ofRandom (2, 7);
    x = ofRandom(100, 500);
    y = ofRandom(100, 500);
    
    xv = ofRandom(-3, 3);
    yv = ofRandom(-3, 3);
    vel = ofVec2f(0,0);
    
    damping = .01;
    randomOffset = ofRandom(-5, 10);
    mass = 1;
    acceleration = ofVec2f(0,0);
    maxSpeed = 5;
    blurOffset = 5;
    colIndex = ofRandom(5);
    team = ofRandom(2);
    
    
    minSize = 4;
    maxSize = 50;
    life = 255;
    membraneLife = 0;
    membraneStep = 1;
    membraneRad = 20;

    
}


void Particle::setupE1(){

    r = ofRandom (3, 15);
    minSize = 4;
    maxSize = 20;
    life = 255;
    
    team = ofRandom(2);
    membraneLife = 20;
    membraneStep = 1;
    membraneRad = 30;
    minMembraneLife = 20;
    maxMembraneLife = 80;
    
    
    vel = ofVec2f(ofRandom(-5, 5), ofRandom(-5, 5));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
}

void Particle::setupE2(){
    xv = ofRandom(-3, 3);
    yv = ofRandom(-3, 3);
    wallTimer = Timer();
    wallTimer.setup();
    wallTimer.endTime = 3000;
    stuckOnWall = false;
    damping = .02;
    
    vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    
}

void Particle::setupE3(){
    r = ofRandom (4, 12);
//    r = 30;
    xv = ofRandom(-3, 3);
    yv = ofRandom(-3, 3);
    team = ofRandom(2);
    damping = .01;
    
    membraneRad = 0;
    
}

StoneParticle::StoneParticle(){
    
    r = ofRandom (5, 15);
}

void Particle::updatePosition() {
    //     f = ma, m = 1, f = a, v = int(a)
    
    xv += xf;// apply forces
    yv += yf;// apply forces
    
//    acceleration.x += xf;// apply forces
//    acceleration.y += yf;// apply forces
    
    x += xv;//
    y += yv;//
    
    
    xv += acceleration.x;
    yv += acceleration.y;
    
    
    //
    //    vel.x += xf;
    //    vel.y += yf;
    //
    
    
    x += vel.x;
    y += vel.y;
    
    
//     Add friction
    friction.x = xv;
    friction.y = yv;
    friction.normalize();
    cF = 0.1;
    friction *= cF;
    applyForce(friction);
    
    acceleration *= 0;
    
//    xf = 0;
//    yf = 0;
    
    if( xv > maxSpeed) xv = maxSpeed;
    if( yv > maxSpeed) yv = maxSpeed;

    
        
}

void Particle::resetForce() {
    xf = 0;
    yf = 0;
}

void Particle::bounceOffWalls(bool rebound_) {
    bool collision = false;
    rebound = rebound_;
    
    int d = ofDist(x, y, origin.x, origin.y);
    
    if ((d > externalRad - r - blurOffset)){
        
        collision = true;
        
        ofVec2f out = ofVec2f(x, y);
        ofVec2f offset = ofVec2f(ofRandom( -50, 50), ofRandom(-50, 50));
        ofVec2f newOrigin = origin + offset;
        ofVec2f ret = origin - out;
        
        
        
        if(rebound) ret = ret.normalize();
        
        if(rebound) applyForce(ret);
        
        x += ret.x;
        y += ret.y;

        xv *= -1;
        yv *= -1;
        
        vel.x *= -1;
        vel.y *= -1;
        
        if(collision && rebound){
            xv *= 0.9;
            yv *= 0.9;
        }
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
//    collectStuckParticles();
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


void Particle::limitSize(){
    if(r < minSize) r = minSize;
    if(r > maxSize) r = maxSize;
}

void Particle::limitMembraneLife(){
    if(membraneLife < minMembraneLife) membraneLife = minMembraneLife;
    if(membraneLife > maxMembraneLife) membraneLife = maxMembraneLife;
    
}


// Function that reverses velocity when hitting an outer cell wall
void Particle::bounceOffOuterCell(float outer){
    bool collision = false;
    
    float d = ofDist(x, y, origin.x, origin.y);
    if( (d > outer - r)){
        collision = true;
        ofVec2f posAtEdge = ofVec2f(x, y);
        ofVec2f ret = origin - posAtEdge;
        ret = ret.normalize();
//        applyForce(ret);
        
        x += ret.x;
        y += ret.y;
        
        xv *= -1;
        yv *= -1;
        
        vel.x *= -1;
        vel.y *= -1;
        
        if(collision == true){
            xv *= 0.9;
            yv *= 0.9;
        }
    }
}

// Function that reverses velocity when hitting an inner cell wall
void Particle::bounceOffInnerCell(float inner){
    bool collision = false;
    
    float d = ofDist(x, y, origin.x, origin.y);
    if( (d < inner + r)){
        collision = true;
        ofVec2f posAtEdge = ofVec2f(x, y);
        ofVec2f ret = origin - posAtEdge;
        ret *= -1;
        ret = ret.normalize();
//        applyForce(ret);
        
        x += ret.x;
        y += ret.y;
        
        xv *= -1;
        yv *= -1;
        
        vel.x *= -1;
        vel.y *= -1;
        
        if(collision == true){
            xv *= 0.9;
            yv *= 0.9;
        }
    }
}

void Particle::receiveCells(vector <float> cells_){
    cells = cells_;
}

void Particle::returnFromWall(){
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
//        ofSetColor(255);
//                ofDrawLine(origin.x, origin.y, x, y);
        //        ofPoint cent = ofPoint(origin)
        //        applyForce(- origin);
        accelerateTowardsTarget(origin);
        
        
    }
}


void Particle::collectStuckParticles(){
    
    
    
    bool collision = false;
    
    int d = ofDist(x, y, origin.x, origin.y);
    if ((d >= externalRad )){
        
        collision = true;
        
        ofVec2f out = ofVec2f(x, y);
        ofVec2f ret = origin - out;
        
//        ret.normalize();
//        ret *= 10;
        applyForce(origin);
        
//        if(rebound) ret = ret.normalize();
        
//        if(rebound) applyForce(ret);
        
//        x += ret.x;
//        y += ret.y;
        
        xv *= -1;
        yv *= -1;
        
        vel.x *= -1;
        vel.y *= -1;
        
        if(collision){
//            addDampingForce();
        }
    }
    
    
    
    
}




