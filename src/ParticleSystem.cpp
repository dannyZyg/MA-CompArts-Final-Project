#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
    kParticles = 300;
    particleNeighborhood = 32;
    rebound = true;
    fade = false;
}

E1System::E1System(){
    kParticles = 80;
    particleNeighborhood = 80;
    particleRepulsion = 0.8;// 0.5;
    centerAttraction = 0.1; //0.6;
    drawLines = false;
    impact = false;
    maxRad = 20;
}

StoneSystem::StoneSystem(){
    kParticles = 50;
    showParticleSpacing = 5;
    numToDisplay = 0;
    particleNeighborhood = 32;
    particleRepulsion = 0.7;
    centerAttraction = 0;
    rebound = false;
    fade = false;
}

void ParticleSystem::setup(int width, int height, int k) {
	this->width = width;
	this->height = height;
	this->k = k;
	binSize = 1 << k;
	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
	bins.resize(xBins * yBins);
    
    mode = ofxColorPalette::BRIGHTNESS;
    
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        Particle particle = Particle();
        particles.push_back(particle);
    }
    
    setupColours();
}


void ParticleSystem::setupColours(){
    
    
}

void ParticleSystem::particleInteractions(){
    
}

void ParticleSystem::outputConditions(){
    
}
void ParticleSystem::impactEffect(){
    
}

void E1System::setupColours(){
    ofColor team1Base = ofColor(27,125, 204);
    
    team1Col.setBaseColor(team1Base);
    team1Col.generateAnalogous();
    
    for(int i = 0; i < particles.size(); i++){
        particles[i].col = ofColor(team1Col[ofRandom(team1Col.size())], particles[i].life);
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
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
//    particleRepulsion = ofMap(sin(ofGetFrameNum() * 0.02 + 500), -1, 1, 0.2, 1);
//    centerAttraction = ofMap(sin(ofGetFrameNum() * 0.02), -1, 1, 0.2, 1);
    
}

void ParticleSystem::draw() {
	int n = particles.size();
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
        particles[i].draw();
	glEnd();
    
    
}


void ParticleSystem::display(){

    
    ofPushStyle();
    ofPushMatrix();
    
    impactEffect();
    // do this once per frame
    setupForces();
    
    
    
    // apply per-particle forces
    if(drawLines) {
        ofSetColor(24, 124, 174);
        ofSetLineWidth(0.1);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }

    
    particleInteractions();
    
    if(drawLines) {
        glEnd();
    }
    
    // single-pass global forces
    addAttractionForce(origin.x, origin.y, 200, centerAttraction);

    update();
    
    outputConditions();
    
    ofPopStyle();
    ofPopMatrix();
}



void E1System::alterSize(E1Particle& cur_){
    
    int nearby;
    region = cur_.r + maxRad;
    
    maxRad = particles[0].maxSize;
    vector<Particle*> closeNei = getNeighbors(cur_.x, cur_.y, region);
    
    //alter size
    
    //test
    nearby = closeNei.size();
    ofPushStyle();
    ofFill();
    ofSetColor(50, cur_.membraneLife);
    if(nearby > 1){
        ofDrawCircle(cur_.x, cur_.y, region);
        cur_.alone = false;
        //        addAttractionForce(cur_, region, 0.2);
    }
    else(cur_.alone = true);
    
    ofPopStyle();
    
    
    for(int j = 0; j < closeNei.size(); j ++){
        float dist = ofDist(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
        float overlap = cur_.r + closeNei[j] -> r;
        
        if(overlap < dist){
            //            addRepulsionForce(cur_, cur_.r, 1);
            ofDrawLine(cur_.x, cur_.y, closeNei[j] -> x, closeNei[j] -> y);
        }
        if(nearby > 1) {
            cur_.r -= cur_.membraneStep;
            cur_.membraneLife --;
        }
        else if (nearby <= 1) {
            cur_.r += cur_.membraneStep;
            cur_.membraneLife ++;
        }
    }
}


void E1System::outputConditions(){
    
    //////// TRIGGER FOR OUTPUT////////////
    float testVal = ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 20);
    
    // run the timer for the glow effect
    glowTimer.run();
    
    
    if(testVal > 4) trigger = true;
    else(trigger = false);
    
    // if these conditions are met, do this once only!
    
    if(trigger && !systemOutput) {
        systemOutput = true;
        ofSetColor(0, 255, 0);
        ofDrawCircle(origin, 50);
        glowTimer.reset();
        glowTimer.endTime = 5000;
        sequenceTrigger = true;
    }
    
    // if the timer is active, glow
    if(!glowTimer.reached){
        glow = true;
        
    }
    // if not, don't glow
    if (glowTimer.reached){
        glow = false;
    }
    
}


void E1System::impactEffect(){
    
    if(impact){
        drawLines = true;
        addRepulsionForce(origin.x, origin.y, 200, 3);
    }
    else{
        drawLines = false;
    }
    
}


void E1System::particleInteractions(){
    for(int i = 0; i < particles.size(); i++) {
        
        E1Particle& cur = particles[i];
        // global force on other particles
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        addAttractionForce(cur, particleNeighborhood, 0.5);
        
        particles[i].limitSize();
        particles[i].limitMembraneLife();
        cur.bounceOffWalls(true);
        cur.addDampingForce();
        
        alterSize(cur);
    }
    
}


