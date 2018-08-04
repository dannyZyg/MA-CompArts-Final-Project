#include "EnvironmentTwoParticle.h"

#include "ofGraphics.h"

EnvironmentTwoParticle::EnvironmentTwoParticle(float x, float y, float xv, float yv) :
	x(x), y(y),
	xv(xv), yv(yv) {
        
//        r = 3;
        r = ofRandom (2, 7);
//        col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        
        
//        xv = 1;
         vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        
        team = ofRandom(2);
        

}

void EnvironmentTwoParticle::updatePosition(float timeStep) {
//     f = ma, m = 1, f = a, v = int(a)
   
//    ofSetColor(255, 0, 0);
//    ofSetColor(col);
    
    
//    ofSetColor(col);

    
    xv += xf;// * timeStep;
    yv += yf;// * timeStep;
    x += xv;// * timeStep;
    y += yv;// * timeStep;
//
//    vel.x += xf;
//    vel.y += yf;
//
//    x += vel.x;
//    y += vel.y;
//
}

void EnvironmentTwoParticle::resetForce() {
	xf = 0;
	yf = 0;
}

void EnvironmentTwoParticle::bounceOffWalls(float damping) {
	bool collision = false;

    ofVec2f cent = ofVec2f(328, 328);
    int d = ofDist(x, y, origin.x, origin.y);
    if( (d > externalRad - r )){  //|| (d > 220) || (d == 200)) {
        
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


void EnvironmentTwoParticle::bounceOffCells(float damping, float outer, float inner){
    bool collision = false;
    
    ofVec2f cent = ofVec2f(328, 328);
    int d = ofDist(x, y, origin.x, origin.y);
    if( (d > externalRad - r )){  //|| (d > 220) || (d == 200)) {
        
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

void EnvironmentTwoParticle::addDampingForce(float damping) {
	xf -= xv * damping;
	yf -= yv * damping;
}

void EnvironmentTwoParticle::draw() {
    
    glVertex2f(x, y);
}


void EnvironmentTwoParticle::displayParticle(){
//    ofNoFill();
    ofSetColor(col);
    ofDrawCircle(x, y, r);
    
    
}


//void EnvironmentTwoParticle::receiveGeometry(ofVec2f origin_, float externalRad_){
//    origin = origin_;
//    externalRad = externalRad_;
//    
//}
