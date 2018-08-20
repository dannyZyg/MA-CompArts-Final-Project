#pragma once

#include "ofMain.h"
#include "Timer.hpp"

class EnvironmentTwoParticle {
public:
	float x, y;
	float xv, yv;
	float xf, yf;
    float r;
    
    ofVec2f origin;
    float externalRad;
    
    float damping;
    
    EnvironmentTwoParticle(float x, float y);
	void updatePosition();
	void resetForce();
	void bounceOffWalls();
    void bounceOffOuterCell(float outer);
    void bounceOffInnerCell(float inner);

	void addDampingForce();
	void draw();
    
    void displayParticle();
    ofVec2f vel;
    ofColor col;
    
    int cellState;
    float randomOffset;
    int colIndex;
    
    vector <float> cells;
    void receiveCells(vector <float> cells_);

    Timer wallTimer;
    
    bool stuckOnWall;
    float distFromWall;
    void returnFromWall();
    void applyForce(ofVec2f force);
    void accelerateTowardsTarget(ofVec2f _target);
    
    float mass;
    ofVec2f acceleration;
    float maxSpeed;
};
