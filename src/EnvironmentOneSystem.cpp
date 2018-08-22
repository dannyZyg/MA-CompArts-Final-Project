#include "EnvironmentOneSystem.h"

EnvironmentOneSystem::EnvironmentOneSystem(){
    kParticles = 80;
        
}

void EnvironmentOneSystem::setup(int width, int height, int k) {
	this->width = width;
	this->height = height;
	this->k = k;
	binSize = 1 << k;
	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
	bins.resize(xBins * yBins);
    
    
//    mode = ofxColorPalette::BRIGHTNESS;
//    brightness = 200;
//    saturation = 200;
    
    
    
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        E1Particle particle = E1Particle();
        
        particles.push_back(particle);
        setupColours();
    }
    
    particleNeighborhood = 80;
    particleRepulsion = 0.8;// 0.5;
    centerAttraction = 0.1; //0.6;
    
    drawBalls = true;
    
    impact = false;
    maxRad = 20;
    
}


void EnvironmentOneSystem::setupColours(){
    ofColor team1Base = ofColor(27,125, 204);
    
    team1Col.setBaseColor(team1Base);
    team1Col.generateAnalogous();

    
    for(int i = 0; i < particles.size(); i++){
        particles[i].col = ofColor(team1Col[ofRandom(team1Col.size())], particles[i].life);
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
    
}




void EnvironmentOneSystem::add(E1Particle particle) {
	particles.push_back(particle);
}

unsigned EnvironmentOneSystem::size() const {
	return particles.size();
}

E1Particle& EnvironmentOneSystem::operator[](unsigned i) {
	return particles[i];
}

vector<E1Particle*> EnvironmentOneSystem::getNeighbors(E1Particle& particle, float radius) {
	return getNeighbors(particle.x, particle.y, radius);
}

vector<E1Particle*> EnvironmentOneSystem::getNeighbors(float x, float y, float radius) {
	vector<E1Particle*> region = getRegion(
		(int) (x - radius),
		(int) (y - radius),
		(int) (x + radius),
		(int) (y + radius));
	vector<E1Particle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		E1Particle& cur = *region[i];
		xd = cur.x - x;
		yd = cur.y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

vector<E1Particle*> EnvironmentOneSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<E1Particle*> region;
	back_insert_iterator< vector<E1Particle*> > back = back_inserter(region);
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
			vector<E1Particle*>& cur = bins[y * xBins + x];
			copy(cur.begin(), cur.end(), back);
		}
	}
	return region;
}

void EnvironmentOneSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	n = particles.size();
	unsigned xBin, yBin, bin;
	for(int i = 0; i < n; i++) {
		E1Particle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.x) >> k;
		yBin = ((unsigned) cur.y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(&cur);
	}
}

void EnvironmentOneSystem::addRepulsionForce(const E1Particle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentOneSystem::addRepulsionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, scale);
}

void EnvironmentOneSystem::addAttractionForce(const E1Particle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentOneSystem::addAttractionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, -scale);
}

void EnvironmentOneSystem::addForce(const E1Particle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, radius, -scale);
}

void EnvironmentOneSystem::addForce(float targetX, float targetY, float radius, float scale) {
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
			vector<E1Particle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				E1Particle& curBinnedParticle = *(curBin[i]);
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

void EnvironmentOneSystem::update() {
	int n = particles.size();
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition();
        particles[i].membraneRad = region;
	}
    
    particleRepulsion = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, 0.2, 1);

}

void EnvironmentOneSystem::draw() {
	int n = particles.size();
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
        particles[i].draw();
	glEnd();
    
    
}

int EnvironmentOneSystem::getWidth() const {
	return width;
}

int EnvironmentOneSystem::getHeight() const {
	return height;
}


void EnvironmentOneSystem::display(){

    
//    setTimeStep(timeStep);
    // do this once per frame
    setupForces();
    
    
    if(impact){
        addRepulsionForce(origin.x, origin.y, 200, 3);
    }
    
    
    ofPushMatrix();
    
    // apply per-particle forces
    if(!drawBalls) {
        ofSetColor(24, 124, 174);
        ofSetLineWidth(0.1);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    
    

    
    for(int i = 0; i < particles.size(); i++) {
        E1Particle& cur = particles[i];
        // global force on other particles
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        addAttractionForce(cur, particleNeighborhood, 0.5);
        cur.bounceOffWalls(true);
        cur.addDampingForce();
        
        alterSize(cur);
        
        ofFill();
    }
    if(!drawBalls) {
        glEnd();
    }
    
    // single-pass global forces
    addAttractionForce(origin.x, origin.y, 200, centerAttraction);
    update();
    
    
    
    
    // draw all the particles
//    if(drawBalls) {
        for(int i = 0; i < particles.size(); i++) {
            //            ofDrawCircle(particleSystem[i].x, particleSystem[i].y,particleSystem[i].r); // particleNeighborhood * 0.05);
            ofSetColor(0, 255, 0);
            particles[i].displayParticle();
            particles[i].limitSize();
            particles[i].limitMembraneLife();
        }
//    }
    
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
    
    
    
    
//    particleSystem.display();
    ofPopMatrix();

    
}



void EnvironmentOneSystem::alterSize(E1Particle& cur_){
    
    int nearby;
    region = cur_.r + maxRad;
    
    maxRad = particles[0].maxSize;
    vector<E1Particle*> closeNei = getNeighbors(cur_.x, cur_.y, region);
    
    
    //alter size
    
    
    //test
    nearby = closeNei.size();
//    string nearbyNum = ofToString(nearby);
    ofPushStyle();
//    ofSetColor(255);
//    ofDrawBitmapString(nearbyNum, cur_.x, cur_.y);
    ofFill();
    ofSetColor(255, cur_.membraneLife);
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



void EnvironmentOneSystem::communicationCondition(){
    
    
}


