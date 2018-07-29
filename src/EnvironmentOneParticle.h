#pragma once

#include "ofMain.h"

class EnvironmentOneParticle {
public:
	float x, y;
	float xv, yv;
	float xf, yf;
    float r;
    
    ofVec2f origin;
    float externalRad;
    
    
    
	EnvironmentOneParticle(float x = 0, float y = 0, float xv = 0, float yv = 0);
	void updatePosition(float timeStep);
	void resetForce();
	void bounceOffWalls(float damping = .3);
	void addDampingForce(float damping = .01);
	void draw();
    
    void displayParticle();
    ofVec2f vel;
    ofColor col;
    
    int team;

    
};
