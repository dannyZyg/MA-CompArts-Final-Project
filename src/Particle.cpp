#include "Particle.h"
#include "ofGraphics.h"

Particle::Particle(float x, float y, float xv, float yv) :
	x(x), y(y),
	xv(xv), yv(yv) {

        r = ofRandom (4, 12);
        vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        
        team = ofRandom(2);
}

void Particle::updatePosition(float timeStep) {
//     f = ma, m = 1, f = a, v = int(a)
    
    xv += xf;// * timeStep;
    yv += yf;// * timeStep;
    x += xv;// * timeStep;
    y += yv;// * timeStep;
//
//    vel.x += xf;
//    vel.y += yf;
//
    x += vel.x;
    y += vel.y;
//
}

void Particle::resetForce() {
	xf = 0;
	yf = 0;
}

void Particle::bounceOffWalls(float damping) {
	bool collision = false;

    ofVec2f cent = ofVec2f(328, 328);
    int d = ofDist(x, y, origin.x, origin.y);
    if( (d > externalRad - r )){  //|| (d > 220) || (d == 200)) {
        
        ofVec2f out = ofVec2f(x, y);
        ofVec2f ret = origin - out;
        ret = ret.normalize();
//        ret *= 0;
        
        x += ret.x;
        y += ret.y;

        
        xv *= -1;
        yv *= -1;

        vel.x *= -1;
        vel.y *= -1;
        
        
        collision = true;
    }
}

void Particle::addDampingForce(float damping) {
	xf -= xv * damping;
	yf -= yv * damping;
}

void Particle::draw() {
    
    glVertex2f(x, y);
}

void Particle::displayParticle(){
//    ofNoFill();
    ofSetColor(col);
    ofDrawCircle(x, y, r);
}

