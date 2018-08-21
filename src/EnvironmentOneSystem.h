#pragma once

#include "Particle.h"
#include "ofxColorPalette.h"
#include "Timer.hpp"

#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES



class EnvironmentOneSystem {
protected:
	vector<E1Particle> particles;
	vector< vector<E1Particle*> > bins;
	int width, height, k, xBins, yBins, binSize;

public:
	EnvironmentOneSystem();
	void setup(int width, int height, int k);
	void add(E1Particle particle);
	vector<E1Particle*> getNeighbors(E1Particle& particle, float radius);
	vector<E1Particle*> getNeighbors(float x, float y, float radius);
	vector<E1Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
	unsigned size() const;
	E1Particle& operator[](unsigned i);

	void setupForces();
	void addRepulsionForce(const E1Particle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float radius, float scale);
	void addAttractionForce(const E1Particle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	void addForce(const E1Particle& particle, float radius, float scale);
	void addForce(float x, float y, float radius, float scale);
	void update();

	void draw();
    void setupColours();
    
    ofxColorPalette team1Col;
    ofxColorPalette team2Col;
    
    ofxColorPalette::ColorChannel mode;
    
    float brightness;
    float saturation;

    int kParticles;
    float particleNeighborhood, particleRepulsion;
    float centerAttraction;
    bool drawLines;
    
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
    bool trigger;
    bool sequenceTrigger;
    int destination;
    int randomPath;
    bool systemOutput;
    bool glow;
    Timer glowTimer;
    
    void alterSize(E1Particle& cur_);
    float maxRad, minRad;
    float region;
    
    void newRules(int option);
    void presetSelector(string preset);
    bool sizeFlux;
    
    
    void populate();
    
    bool attraction;
    
    int numActive;
//    bool positionReset;
    
//    void positionReset();
    
    int maxParticles;
    
    void updateNumParticles();
    
    int counter;
    
    int particlePushPopRate;
    
};




