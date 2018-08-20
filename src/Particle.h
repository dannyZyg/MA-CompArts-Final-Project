#pragma once

#include "ofMain.h"
#include "Timer.hpp"

class Particle {
public:
	float x, y; //position
	float xv, yv; //velocity
	float xf, yf; //force impact
    float r;
    float mass;
    float maxSpeed;
    ofVec2f acceleration;
    ofColor col;
    ofVec2f origin; //center of the bounding circle
    float externalRad; // radius of the bounding circle
    
    Particle();
	Particle(float x_, float y_);
    
	void updatePosition();
	void resetForce();
//    void bounceOffWalls(float damping = .3);
    void bounceOffOuterCell(float outer);
	void addDampingForce(float damping = .01);
	void draw();
    void displayParticle();

    void applyForce(ofVec2f force);
    void accelerateTowardsTarget(ofVec2f _target);
    


};

class E2Particle: public Particle {
public:
    
    int cellState;
    float randomOffset;
    int colIndex;
    
    vector <float> cells;
    void receiveCells(vector <float> cells_);
    
    Timer wallTimer;
    
    bool stuckOnWall;
    float distFromWall;
    void returnFromWall();
    
    void bounceOffInnerCell(float inner);
    
};





class BabyParticle: public Particle {
public:

    
    int team;
    void test();
    float length;
    
};
