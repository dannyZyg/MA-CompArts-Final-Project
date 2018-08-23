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
    void bounceOffWalls(bool rebound_);
	void addDampingForce();
	void draw();
    void displayParticle();

    void applyForce(ofVec2f force);
    void accelerateTowardsTarget(ofVec2f _target);
    
    float damping;
    ofVec2f friction;
    float cF;
    
    int cellState;
    float randomOffset;
    int colIndex;
    
    
    float life;
    
    ofVec2f vel;
    
    bool rebound;
    
    float blurOffset;

};


class E1Particle: public Particle {
public:
    E1Particle();
    
    float minSize, maxSize;
    void limitSize();
    void limitMembraneLife();
    
    
    float membraneRad;
    int team;
    bool alone;
    float membraneLife;
    float membraneStep;
    
    

};

class E2Particle: public Particle {
    
public:
    E2Particle();
    void returnFromWall();
    void bounceOffOuterCell(float outer);
    void bounceOffInnerCell(float inner);
    vector <float> cells;
    void receiveCells(vector <float> cells_);
    Timer wallTimer;
    bool stuckOnWall;
    float distFromWall;
};

class E3Particle: public Particle {
public:
    E3Particle();
    
    
     int team;
    
};

class StoneParticle: public Particle {
public:
    StoneParticle();
//    void bounceOffWalls();
    
};

