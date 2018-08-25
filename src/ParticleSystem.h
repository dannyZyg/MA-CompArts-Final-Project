#pragma once

#include "Particle.h"
#include "ofxColorPalette.h"
#include "Timer.hpp"

#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES

class ParticleSystem {
protected:

    
public:
    
    vector<Particle> particles;
    vector< vector<Particle*> > bins;
    int width, height, k, xBins, yBins, binSize;
    ParticleSystem();
    
    virtual void setup(int width, int height, int k);
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
    virtual void update();
    
    void draw();
    int getWidth() const;
    int getHeight() const;
    
    virtual void setupParticles();
    void setupColours();
    vector <ofxColorPalette> teamCols;
    ofColor team1Base, team2Base, team3Base, team4Base;
    ofxColorPalette::ColorChannel mode;
    float brightness;
    float saturation;
    
    float lineAlpha;
    
    int kParticles;
    float particleNeighborhood;
    float centerAttraction, particleRepulsion, particleAttraction;
    bool drawLines;
    
    void display();
    
    ofVec2f origin;
    float externalRad;
    float angle;
    ofVec2f impactTarget;

    vector <float> noiseSeed;
    
    Timer glowTimer;
    Timer outputTimer;
    
    bool rebound;
    bool trigger;
    bool sequenceTrigger;
    bool sequenceActive;
    bool impact;
    bool systemOutput;
    bool glow;
    
    int destination;
    int randomPath;
    int outputCondition;
    int outputThreshold;
    int clusterCount;
    
    virtual void particleInteractions();
    virtual void outputConditions();
    virtual void impactEffect();

    
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
    
};
