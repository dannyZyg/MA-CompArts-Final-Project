#include "Particle.h"
#include "ofGraphics.h"

Particle::Particle(){
    r = ofRandom (2, 7);
    x = ofRandom(100, 500);
    y = ofRandom(100, 500);
    
    xv = ofRandom(-3, 3);
    yv = ofRandom(-3, 3);
    vel = ofVec2f(0,0);
    
    damping = .04;
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
    noiseSeed = ofRandom(1000);
    
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
    
    vel = ofVec2f(ofRandom(-2, 2), ofRandom(-2, 2));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
}

void Particle::setupE2(){
    life = 255;

    xv = ofRandom(-3, 3);
    yv = ofRandom(-3, 3);
    stuckOnWall = false;
    damping = .04;
    
    vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    maxSpeed = 3;
}

void Particle::setupE3(){
    r = ofRandom (4, 12);
    life = 255;
    xv = ofRandom(-1, 1);
    yv = ofRandom(-1, 1);
    team = ofRandom(2);
    damping = .04;
    
    membraneRad = 0;
    
    vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    
}

void Particle::setupStoneParticle(){
    r = ofRandom (5, 12);
    vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
    damping = ofRandom(0.01, 0.03);
    xv = ofRandom(-1, 1);
    yv = ofRandom(-1, 1);
    life = 255;
}

void Particle::updatePosition() {

    xv += xf;// apply forces
    yv += yf;// apply forces

    x += xv;
    y += yv;

    xv += acceleration.x;
    yv += acceleration.y;

    x += vel.x;
    y += vel.y;
    
    vel.limit(maxSpeed);

//     Add friction
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
    ofSetColor(col, life);
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
}


void Particle::limitSize(){
    if(r < minSize) r = minSize;
    if(r > maxSize) r = maxSize;
}

void Particle::limitMembraneLife(){
    if(membraneLife < minMembraneLife) membraneLife = minMembraneLife;
    if(membraneLife > maxMembraneLife) membraneLife = maxMembraneLife;
}


// Function that reverses velocity when hitting an outer cell wall (used for environment 2)
void Particle::bounceOffOuterCell(float outer){
    bool collision = false;
    
    float d = ofDist(x, y, origin.x, origin.y);
    if( (d > outer - r)){
        collision = true;
        ofVec2f posAtEdge = ofVec2f(x, y);
        ofVec2f ret = origin - posAtEdge;
        ret = ret.normalize();
        
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

// Function that reverses velocity when hitting an inner cell wall (used for environment 2)
void Particle::bounceOffInnerCell(float inner){
    bool collision = false;
    
    float d = ofDist(x, y, origin.x, origin.y);
    if( (d < inner + r)){
        collision = true;
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
        
        if(collision == true){
            xv *= 0.9;
            yv *= 0.9;
        }
    }
}

// for environment two, this function will retrieve the cell wall locations coming from the env 2 source class
void Particle::receiveCells(vector <float> cells_){
    cells = cells_;
}

// particles will accelerate towards the origin if they reach the edge of the wakk
void Particle::returnFromWall(){
    float distFromOrigin = ofDist(origin.x, origin.y, x, y);
    bool trigger;
    
    if(distFromOrigin > (externalRad - 50)){
        stuckOnWall = true;
    }
    else stuckOnWall = false;

    if(stuckOnWall){
        accelerateTowardsTarget(origin);
    }
}
