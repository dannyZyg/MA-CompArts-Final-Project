#include "EnvironmentThreeSystem.h"

EnvironmentThreeSystem::EnvironmentThreeSystem() :
	timeStep(100) {

        
}

void EnvironmentThreeSystem::setup(int width, int height, int k) {
	this->width = width;
	this->height = height;
	this->k = k;
	binSize = 1 << k;
	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
	bins.resize(xBins * yBins);
    
    
    mode = ofxColorPalette::BRIGHTNESS;
    brightness = 200;
    saturation = 200;
    
    
    kParticles = 80;
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        E3Particle particle = E3Particle();
        
//        E3Particle particle();
        
        particles.push_back(particle);
        
//        particleSystem.add(particle);
        setupColours();
    }
    
    padding = 128;
    timeStep = 100;
    isMousePressed = false;
    slowMotion = true;
    particleNeighborhood = 64;
    particleRepulsion = 0.3;
    centerAttraction = 0;
    drawBalls = true;
    
    angle = 0;
    target = ofVec2f(origin.x + 100, origin.y + 100);
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
    
    glowTimer.setup();
    trigger = false;
    glow = false;
}


void EnvironmentThreeSystem::setupColours(){
    ofColor team1Base = ofColor(52,167, 173);
    ofColor team2Base = ofColor(255,211, 91);
    
//        ofColor team1Base = ofColor(255,0, 0);
//        ofColor team2Base = ofColor(0,0, 255);

    
    team1Col.setBaseColor(team1Base);
    team1Col.generateAnalogous();
    
    team2Col.setBaseColor(team2Base);
    team2Col.generateAnalogous();

    
    for(int i = 0; i < particles.size(); i++){
        
        if(particles[i].team == 0){
            particles[i].col = ofColor(team1Col[ofRandom(team1Col.size())]);
//            particles[i].col = ofColor(team1Base);

        }
        else if (particles[i].team == 1){
            particles[i].col = ofColor(team2Col[ofRandom(team2Col.size())]);
//            particles[i].col = ofColor(team2Base);
        }
            
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
    
}


void EnvironmentThreeSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

void EnvironmentThreeSystem::add(E3Particle particle) {
	particles.push_back(particle);
}

unsigned EnvironmentThreeSystem::size() const {
	return particles.size();
}

E3Particle& EnvironmentThreeSystem::operator[](unsigned i) {
	return particles[i];
}

vector<E3Particle*> EnvironmentThreeSystem::getNeighbors(E3Particle& particle, float radius) {
	return getNeighbors(particle.x, particle.y, radius);
}

vector<E3Particle*> EnvironmentThreeSystem::getNeighbors(float x, float y, float radius) {
	vector<E3Particle*> region = getRegion(
		(int) (x - radius),
		(int) (y - radius),
		(int) (x + radius),
		(int) (y + radius));
	vector<E3Particle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		E3Particle& cur = *region[i];
		xd = cur.x - x;
		yd = cur.y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

vector<E3Particle*> EnvironmentThreeSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<E3Particle*> region;
	back_insert_iterator< vector<E3Particle*> > back = back_inserter(region);
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
			vector<E3Particle*>& cur = bins[y * xBins + x];
			copy(cur.begin(), cur.end(), back);
		}
	}
	return region;
}

void EnvironmentThreeSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	n = particles.size();
	unsigned xBin, yBin, bin;
	for(int i = 0; i < n; i++) {
		E3Particle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.x) >> k;
		yBin = ((unsigned) cur.y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(&cur);
	}
}

void EnvironmentThreeSystem::addRepulsionForce(const E3Particle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentThreeSystem::addRepulsionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, scale);
}

void EnvironmentThreeSystem::addAttractionForce(const E3Particle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentThreeSystem::addAttractionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, -scale);
}

void EnvironmentThreeSystem::addForce(const E3Particle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, radius, -scale);
}

void EnvironmentThreeSystem::addForce(float targetX, float targetY, float radius, float scale) {
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
			vector<E3Particle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				E3Particle& curBinnedParticle = *(curBin[i]);
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

void EnvironmentThreeSystem::update(float lastTimeStep) {
	int n = particles.size();
//    float curTimeStep = lastTimeStep * timeStep;
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition();
	}
    
//    particleRepulsion = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, 0.01, 0.5);
    angle += 10;
}

void EnvironmentThreeSystem::draw() {
	int n = particles.size();
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
        particles[i].draw();
	glEnd();
    
    
}

int EnvironmentThreeSystem::getWidth() const {
	return width;
}

int EnvironmentThreeSystem::getHeight() const {
	return height;
}


void EnvironmentThreeSystem::display(){

    
    setTimeStep(timeStep);
    // do this once per frame
    setupForces();
    
    ofPushMatrix();
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(origin.x, origin.y);
    ofRotate(angle);
    ofSetColor(255, 0, 0   );
    if(impact){
//        ofDrawLine(0, 0, 100, 100);
        ofDrawCircle(200, 200, 5);
        addRepulsionForce(200, 200, 500, 3);
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    
    // apply per-particle forces
    if(!drawBalls) {
        ofSetColor(24, 124, 174);
        ofSetLineWidth(0.1);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    
    float tempRad = 50;
    
    for(int i = 0; i < particles.size(); i++) {
        E3Particle& cur = particles[i];
        // global force on other particles
        
        
        
//        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        
        
        vector<E3Particle*> nei = getNeighbors(cur.x, cur.y, tempRad);
        
        vector<E3Particle*> clusters = getNeighbors(cur.x, cur.y, 10);
        
        vector<E3Particle*> global = getNeighbors(cur.x, cur.y, externalRad/3);

        for(int j = 0; j < nei.size(); j ++){

            
            addRepulsionForce(cur, particleNeighborhood, 0.04);
            
            
            if(cur.team != nei[j] -> team){
                ofSetColor(255);
                ofDrawLine(cur.x, cur.y, nei[j] -> x, nei[j] -> y);
                addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
            }
            
            if(cur.team == nei[j] -> team){
                addAttractionForce(cur, particleNeighborhood, 0.06);
            }
        }
        
        for(int j = 0; j < clusters.size(); j ++){
            if(cur.team == clusters[j] -> team){
                clusterCount ++;
            }
        }
        
        
        for(int j = 0; j < global.size(); j ++){
            if(glow){
                ofPushStyle();
                float alpha = ofMap(sin(ofGetFrameNum() * 0.01 + noiseSeed[i]), -1, 1, 0, 200);
                ofColor c = cur.col;
                ofSetColor(c, alpha);

                ofDrawLine(cur.x, cur.y, global[j] -> x, global[j] -> y);
                ofPopStyle();
            }
        }
 
        
// TRIGGER FOR OUTPUT
        float testVal = ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 20);
    
// run the timer for the glow effect
        glowTimer.run();

        
        if(clusterCount > 4) trigger = true;
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

        // forces on this particle
        cur.bounceOffWalls();
        cur.addDampingForce();
        
// reset the cluster tally
        clusterCount = 0;
        
    }
    if(!drawBalls) {
        glEnd();
    }
    
    // single-pass global forces
    addAttractionForce(origin.x, origin.y, 200, centerAttraction);
    if(isMousePressed) {
    addRepulsionForce(ofGetMouseX(), ofGetMouseY(), 200, 1);
    }
    update(ofGetLastFrameTime());
    
    
    
    
    // draw all the particles
    if(drawBalls) {
        for(int i = 0; i < particles.size(); i++) {
            //            ofDrawCircle(particleSystem[i].x, particleSystem[i].y,particleSystem[i].r); // particleNeighborhood * 0.05);
            
            particles[i].displayParticle();
//            ofNoFill();
//            ofDrawCircle(particles[i].x, particles[i].y, tempRad);
        }
    }
    
    
//    particleSystem.display();
    ofPopMatrix();

    
}


//void EnvironmentThreeSystem::receiveGeometry(ofVec2f origin_, float externalRad_){
//    
//    origin = origin_;
//    externalRad = externalRad_;
//    
//}

