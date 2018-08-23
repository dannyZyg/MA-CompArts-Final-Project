

//#include "StoneParticle.h"
#include "Particle.h"
#include "ofxColorPalette.h"
#include "ofxBlur.h"

#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES

#pragma once

class StoneParticleSystem {
protected:
	float timeStep;
	vector<StoneParticle> particles;
	vector< vector<StoneParticle*> > bins;
	int width, height, k, xBins, yBins, binSize;

public:
	StoneParticleSystem();

	void setup(int width, int height, int k);
	void setTimeStep(float timeStep);

	void addParticle();
	vector<StoneParticle*> getNeighbors(StoneParticle& particle, float radius);
	vector<StoneParticle*> getNeighbors(float x, float y, float radius);
	vector<StoneParticle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
	unsigned size() const;
	StoneParticle& operator[](unsigned i);

	void setupForces();
	void addRepulsionForce(const StoneParticle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float radius, float scale);
	void addAttractionForce(const StoneParticle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	void addForce(const StoneParticle& particle, float radius, float scale);
	void addForce(float x, float y, float radius, float scale);
	void update();

	void draw();
    void setupColours(ofColor base_);
    void updateColours();
	
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
    void reset();
    void fadeParticles();

    
    ofVec2f origin;
    float externalRad;
    bool sensor;
    
    bool active;
    bool drawLines;
    ofColor baseColour;
    
    int numToDisplay;
    bool timeIntervalPassed;
    unsigned int timer;
    
    int showParticleSpacing;
    
    
    void originSystem(string originSystem_);
    void newColours(string originSystem_);
    bool env1, env2, env3, sens;
    
    int env1Display, env2Display, env3Display, sensorDisplay;
    int environmentDivision;
    
    
    
    ofColor env1Col;
    ofColor env2Col;
    ofColor env3Col;
    ofColor sensorCol;
    
    
    
    
};

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
