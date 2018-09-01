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


    int team;
    float minSize, maxSize;
    void limitSize();
    void limitMembraneLife();
    
    float membraneRad;
    float membraneLife;
    float membraneStep;
    bool alone;
    
    void receiveCells(vector <float> cells_);
    vector <float> cells;
    
    
    void returnFromWall();
    void bounceOffOuterCell(float outer);
    void bounceOffInnerCell(float inner);
    Timer wallTimer;
    bool stuckOnWall;
    float distFromWall;
    
    int noiseSeed;
    
    void collectStuckParticles();
    float minMembraneLife, maxMembraneLife;
    
    void noisyMovement();
    
    
};
