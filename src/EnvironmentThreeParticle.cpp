#include "EnvironmentThreeParticle.h"

#include "ofGraphics.h"

EnvironmentThreeParticle::EnvironmentThreeParticle(float x, float y, float xv, float yv) :
	x(x), y(y),
	xv(xv), yv(yv) {
        
//        r = 3;
        r = ofRandom (4, 10);
//        col = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        
        
//        xv = 1;
         vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        if(vel.x == 0 && vel.y == 0) vel = ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        
        team = ofRandom(2);

}

void EnvironmentThreeParticle::updatePosition(float timeStep) {
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

void EnvironmentThreeParticle::resetForce() {
	xf = 0;
	yf = 0;
}

void EnvironmentThreeParticle::bounceOffWalls(float damping) {
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

        
        collision = true;
        
        
    }

    
    
//    if(d > 200) ofSetColor(255, 0, 0);
//    else{
//        ofSetColor(255);
//    }
    
    
//    if (x > right){
//        x = right;
//        xv *= -1;
//        collision = true;
//    } else if (x < left){
//        x = left;
//        xv *= -1;
//        collision = true;
//    }
//
//    if (y > bottom){
//        y = bottom;
//        yv *= -1;
//        collision = true;
//    } else if (y < top){
//        y = top;
//        yv *= -1;
//        collision = true;
//    }
//
//    if (collision == true){
//        xv *= damping;
//        yv *= damping;
//    }
}

void EnvironmentThreeParticle::addDampingForce(float damping) {
	xf -= xv * damping;
	yf -= yv * damping;
}

void EnvironmentThreeParticle::draw() {
    
    glVertex2f(x, y);
}


void EnvironmentThreeParticle::displayParticle(){
//    ofNoFill();
    ofSetColor(col);
    ofDrawCircle(x, y, r);
    
    
}


//void EnvironmentThreeParticle::receiveGeometry(ofVec2f origin_, float externalRad_){
//    origin = origin_;
//    externalRad = externalRad_;
//    
//}
