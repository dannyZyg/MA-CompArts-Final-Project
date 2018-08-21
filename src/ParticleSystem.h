#pragma once

//#include "EnvironmentTwoParticle.h"
#include "Particle.h"
#include "ofxColorPalette.h"


#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES



class ParticleSystem {
protected:
	float timeStep;
	vector<Particle> particles;
	vector< vector<Particle*> > bins;
	int width, height, k, xBins, yBins, binSize;

public:
	ParticleSystem();

	void setup(int width, int height, int k);
	void setTimeStep(float timeStep);

	void add(Particle particle);
	vector<Particle*> getNeighbors(Particle& particle, float radius);
	vector<Particle*> getNeighbors(float x, float y, float radius);
	vector<Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
	unsigned size() const;
	Particle& operator[](unsigned i);
	void setupForces();
	void addRepulsionForce(const Particle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float radius, float scale);
	void addAttractionForce(const Particle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	void addForce(const Particle& particle, float radius, float scale);
	void addForce(float x, float y, float radius, float scale);
	void update(float lastTimeStep);
    
    

	void draw();
    
    void bounceOffCells();
	
	int getWidth() const;
	int getHeight() const;
    
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
    
    bool state;
    

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


class E2System : public ParticleSystem {
    public:
    
    vector<E2Particle> particles;
    void u();
    void behaviourState();
    void setupColours();
    void updateColours();
    void allocateCellState(E2Particle& particle);
    vector <float> cells;
    void receiveCells(vector <float> cells_);
    
    void cellWallRebound(E2Particle& particle);

    
    ofxColorPalette cell1Col;
    ofxColorPalette cell2Col;
    ofxColorPalette cell3Col;
    
    
};



