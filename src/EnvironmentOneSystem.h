#include "EnvironmentOneParticle.h"
#include "ofxColorPalette.h"

#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES

#pragma once

class EnvironmentOneSystem {
protected:
	float timeStep;
	vector<EnvironmentOneParticle> particles;
	vector< vector<EnvironmentOneParticle*> > bins;
	int width, height, k, xBins, yBins, binSize;

public:
	EnvironmentOneSystem();

	void setup(int width, int height, int k);
	void setTimeStep(float timeStep);

	void add(EnvironmentOneParticle particle);
	vector<EnvironmentOneParticle*> getNeighbors(EnvironmentOneParticle& particle, float radius);
	vector<EnvironmentOneParticle*> getNeighbors(float x, float y, float radius);
	vector<EnvironmentOneParticle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
	unsigned size() const;
	EnvironmentOneParticle& operator[](unsigned i);

	void setupForces();
	void addRepulsionForce(const EnvironmentOneParticle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float radius, float scale);
	void addAttractionForce(const EnvironmentOneParticle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	void addForce(const EnvironmentOneParticle& particle, float radius, float scale);
	void addForce(float x, float y, float radius, float scale);
	void update(float lastTimeStep);

	void draw();
    void setupColours();
	
	int getWidth() const;
	int getHeight() const;
    
    
    ofxColorPalette team1Col;
    ofxColorPalette team2Col;
    
    ofxColorPalette::ColorChannel mode;
    float brightness;
    float saturation;

    
    
    int kParticles;
//    float timeStep;
    float particleNeighborhood, particleRepulsion;
    float centerAttraction;
    float padding;
    bool isMousePressed, slowMotion;
    bool drawBalls;
    
    void display();
    
    ofVec2f origin;
    float externalRad;
    

    inline float InvSqrt(float x){
        float xhalf = 0.5f * x;
        int i = *(int*)&x; // store floating-point bits in integer
        i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
        x = *(float*)&i; // convert new bits into float
        x = x*(1.5f - xhalf*x*x); // One round of Newton's method
        return x;
    }
    
    inline float smoothForce(float x) {
        const static float sharpness = 1;
        return 1. / (1. + expf((x - .5) * sharpness * -12));
    }
    
    bool impact;
    bool sequenceActive;
    
};




