#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
    kParticles = 100;
    particleNeighborhood = 64;
    particleRepulsion = 0.3;
    centerAttraction = 0;
    drawLines = true;
    angle = 0;
    impactTarget = ofVec2f(origin.x + 100, origin.y + 100);
    
//    lineAlpha = 0;
    
    outputThreshold = 200;
    
    rebound = true;
    trigger = false;
    glow = false;
}

void ParticleSystem::setup(int width, int height, int k) {
    
    this->width = width;
    this->height = height;
    this->k = k;
    binSize = 1 << k;
    xBins = (int) ceilf((float) width / (float) binSize);
    yBins = (int) ceilf((float) height / (float) binSize);
    bins.resize(xBins * yBins);
    
    setupParticles();
    
    mode = ofxColorPalette::BRIGHTNESS;
    //    brightness = 200;
    //    saturation = 200;
    
    glowTimer.setup();
    outputTimer = Timer();
    outputTimer.setup();
    outputTimer.endTime = 30000;
    
}

void ParticleSystem::setupParticles(){
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        Particle particle = Particle();
        particle.origin = origin;
        particle.externalRad = externalRad;
        particles.push_back(particle);
    }
    
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
}


void ParticleSystem::setupColours(){
    
    for(int i = 0; i < 4; i ++){
        ofxColorPalette c;
        teamCols.push_back(c);
    }
    
    teamCols[0].setBaseColor(ofColor(team1Base));
    teamCols[1].setBaseColor(ofColor(team2Base));
    teamCols[2].setBaseColor(ofColor(team3Base));
    teamCols[3].setBaseColor(ofColor(team4Base));
    
    for(int i = 0; i < teamCols.size(); i ++){
        teamCols[i].generateAnalogous();
    }
    
    for(int i = 0; i < particles.size(); i++){
        
        ofxColorPalette& c = teamCols[particles[i].team];
        particles[i].col = ofColor(c[particles[i].colIndex]);
    }
}

void ParticleSystem::impactEffect(){
    //Placeholder function which will be overrided by child classes
}
void ParticleSystem::particleInteractions(){
    //Placeholder function which will be overrided by child classes
}
void ParticleSystem::outputConditions(){
    //Placeholder function which will be overrided by child classes
}

void ParticleSystem::seedWithRandomValues(){
    //Placeholder function which will be overrided by child classes
}

Particle& ParticleSystem::operator[](unsigned i) {
    return particles[i];
}

vector<Particle*> ParticleSystem::getNeighbors(Particle& particle, float radius) {
    return getNeighbors(particle.x, particle.y, radius);
}

vector<Particle*> ParticleSystem::getNeighbors(float x, float y, float radius) {
    vector<Particle*> region = getRegion(
                                           (int) (x - radius),
                                           (int) (y - radius),
                                           (int) (x + radius),
                                           (int) (y + radius));
    vector<Particle*> neighbors;
    int n = region.size();
    float xd, yd, rsq, maxrsq;
    maxrsq = radius * radius;
    for(int i = 0; i < n; i++) {
        Particle& cur = *region[i];
        xd = cur.x - x;
        yd = cur.y - y;
        rsq = xd * xd + yd * yd;
        if(rsq < maxrsq)
            neighbors.push_back(region[i]);
    }
    return neighbors;
}

vector<Particle*> ParticleSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
    vector<Particle*> region;
    back_insert_iterator< vector<Particle*> > back = back_inserter(region);
    unsigned minXBin = minX >> k;
    unsigned maxXBin = maxX >> k;
    unsigned minYBin = minY >> k;
    unsigned maxYBin = maxY >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& cur = bins[y * xBins + x];
            copy(cur.begin(), cur.end(), back);
        }
    }
    return region;
}

void ParticleSystem::setupForces() {
    int n = bins.size();
    for(int i = 0; i < n; i++) {
        bins[i].clear();
    }
    n = particles.size();
    unsigned xBin, yBin, bin;
    for(int i = 0; i < n; i++) {
        Particle& cur = particles[i];
        cur.resetForce();
        xBin = ((unsigned) cur.x) >> k;
        yBin = ((unsigned) cur.y) >> k;
        bin = yBin * xBins + xBin;
        if(xBin < xBins && yBin < yBins)
            bins[bin].push_back(&cur);
    }
}

void ParticleSystem::addRepulsionForce(const Particle& particle, float radius, float scale) {
    addRepulsionForce(particle.x, particle.y, radius, scale);
}

void ParticleSystem::addRepulsionForce(float x, float y, float radius, float scale) {
    addForce(x, y, radius, scale);
}

void ParticleSystem::addAttractionForce(const Particle& particle, float radius, float scale) {
    addAttractionForce(particle.x, particle.y, radius, scale);
}

void ParticleSystem::addAttractionForce(float x, float y, float radius, float scale) {
    addForce(x, y, radius, -scale);
}

void ParticleSystem::addForce(const Particle& particle, float radius, float scale) {
    addForce(particle.x, particle.y, radius, -scale);
}

void ParticleSystem::addForce(float targetX, float targetY, float radius, float scale) {
    float minX = targetX - radius;
    float minY = targetY - radius;
    float maxX = targetX + radius;
    float maxY = targetY + radius;
    if(minX < 0)
        minX = 0;
    if(minY < 0)
        minY = 0;
    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    float xd, yd, length, maxrsq;
#ifdef USE_INVSQRT
    float xhalf;
    int lengthi;
#else
    float effect;
#endif
    maxrsq = radius * radius;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                Particle& curBinnedParticle = *(curBin[i]);
                xd = curBinnedParticle.x - targetX;
                yd = curBinnedParticle.y - targetY;
                length = xd * xd + yd * yd;
                if(length > 0 && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curBinnedParticle.x, curBinnedParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= radius;
                    length = 1 / length;
                    length = (1 - length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    length *= scale;
                    xd *= length;
                    yd *= length;
                    curBinnedParticle.xf += xd;
                    curBinnedParticle.yf += yd;
#else
                    length = sqrtf(length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    xd /= length;
                    yd /= length;
                    effect = (1 - (length / radius)) * scale;
                    curBinnedParticle.xf += xd * effect;
                    curBinnedParticle.yf += yd * effect;
#endif
                }
            }
        }
    }
}

void ParticleSystem::update() {
    int n = particles.size();
    for(int i = 0; i < n; i++) {
        particles[i].updatePosition();
    }
}

void ParticleSystem::draw() {
    int n = particles.size();
    glBegin(GL_POINTS);
    for(int i = 0; i < n; i++)
        particles[i].draw();
    glEnd();
}

void ParticleSystem::display(){

    outputTimer.run();
    //    cout << outputTimer.reached << endl;

    // do this once per frame
    setupForces();
    
    impactEffect();
    fadeConnectingLines();
    seedWithRandomValues();
    
    // apply per-particle forces
    if(drawLines) {
        
        clusterCount = 0;
    
        for(int i = 0; i < particles.size(); i ++){
            vector<Particle*> nei = getNeighbors(particles[i].x, particles[i].y, 50);
            for(int j = 0; j < nei.size(); j ++){
                float d = ofDist(particles[i].x, particles[i].y, nei[j] -> x, nei[j] -> y);
                if(d < 5) clusterCount ++;
            }
            
            if(clusterCount > 500) addRepulsionForce(particles[i], 20, 0.7);
        }
        
        
        cout <<"cluster ct = " << clusterCount << endl;
        
        
        
        
        float lineLerp = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 255), -1, 1, 0, 1);
        ofColor lineCol = team1Base.getLerped(team2Base, lineLerp);
        
        
//        float lineLerp = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 255), -1, 1, 0, 1);
//        ofColor lineCol = team1Base.getLerped(team2Base, lineLerp);
//
        ofSetColor(lineCol, lineAlpha);
//        ofSetColor(255, lineAlpha);
        ofSetLineWidth(2);
        
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    particleInteractions();


    if(drawLines) {
        glEnd();
    }



    // single-pass global forces
    addAttractionForce(origin.x, origin.y, 200, centerAttraction);
    update();

    // draw all the particles
    for(int i = 0; i < particles.size(); i++) {
        particles[i].displayParticle();
    }

    outputConditions();

}

void ParticleSystem::fadeConnectingLines(){
    if(impact){
        lineAlpha += 2;
    }
    else{
        lineAlpha -= 2;
    }
    if(lineAlpha > 255) lineAlpha = 255;
    if (lineAlpha < 0) lineAlpha = 0;
    
//    lineAlpha = ofMap(ofGetMouseX(), 0 , ofGetWidth(), 0, 255);
    
}



