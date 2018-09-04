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
    
    void setupE1();
    void setupE2();
    void setupE3();
    void setupStoneParticle();
    
	void updatePosition();
	void resetForce();
    void bounceOffWalls(bool rebound_);
	void addDampingForce();
	void draw();
    void displayParticle();

    void applyForce(ofVec2f force);
    void accelerateTowardsTarget(ofVec2f _target);
    void returnFromWall();
    void bounceOffOuterCell(float outer);
    void bounceOffInnerCell(float inner);
    void limitSize();
    void limitMembraneLife();
    void receiveCells(vector <float> cells_);
    
    vector <float> cells;
    ofVec2f vel;
    ofVec2f friction;
    
    float cF;
    float life;
    float minSize, maxSize;
    float membraneRad;
    float membraneLife;
    float membraneStep;
    float minMembraneLife, maxMembraneLife;
    float distFromWall;
    float blurOffset;
    float damping;

    int cellState;
    int colIndex;
    int noiseSeed;
    int team;
    
    bool alone;
    bool rebound;
    bool stuckOnWall;
};
