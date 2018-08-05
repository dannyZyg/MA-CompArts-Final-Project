#pragma once

#include "ofMain.h"

class EnvironmentTwoParticle {
public:
	float x, y;
	float xv, yv;
	float xf, yf;
    float r;
    
    ofVec2f origin;
    float externalRad;
    
    float damping;
    
	EnvironmentTwoParticle(float x = 0, float y = 0, float xv = 0, float yv = 0);
	void updatePosition(float timeStep);
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

    
};
