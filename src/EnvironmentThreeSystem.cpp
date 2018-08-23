#include "EnvironmentThreeSystem.h"

EnvironmentThreeSystem::EnvironmentThreeSystem(){
    kParticles = 100;
    particleNeighborhood = 64;
    particleRepulsion = 0.3;
    centerAttraction = 0;
    drawLines = false;
    angle = 0;
    impactTarget = ofVec2f(origin.x + 100, origin.y + 100);
    
    lineAlpha = 0;
    
    outputThreshold = 200;
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
//    brightness = 200;
//    saturation = 200;
    
    
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        E3Particle particle = E3Particle();
        particles.push_back(particle);
    }
    
    setupColours();
    
    for(int i = 0; i < kParticles; i ++){
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
    
    glowTimer.setup();
    trigger = false;
    glow = false;
}


void EnvironmentThreeSystem::setupColours(){
    team1Base = ofColor(52,167, 173);
    team2Base = ofColor(255,211, 91);
    
//        ofColor team1Base = ofColor(255,0, 0);
//        ofColor team2Base = ofColor(0,0, 255);

    
    team1Col.setBaseColor(team1Base);
    team1Col.generateAnalogous();
    
    team2Col.setBaseColor(team2Base);
    team2Col.generateAnalogous();

    
    for(int i = 0; i < particles.size(); i++){
        
        if(particles[i].team == 0){
            particles[i].col = ofColor(team1Col[ofRandom(team1Col.size())]);
        }
        else if (particles[i].team == 1){
            particles[i].col = ofColor(team2Col[ofRandom(team2Col.size())]);
        }
            
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
    
}


void EnvironmentThreeSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
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

void EnvironmentThreeSystem::update() {
	int n = particles.size();
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition();
	}
//    particleRepulsion = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, 0.01, 0.5);
}

void EnvironmentThreeSystem::draw() {
	int n = particles.size();
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
        particles[i].draw();
	glEnd();
}

void EnvironmentThreeSystem::display(){

    // do this once per frame
    setupForces();
    impactEffect();
    // apply per-particle forces
    if(drawLines) {
        
        
        float lineLerp = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 255), -1, 1, 0, 1);
        ofColor lineCol = team1Base.getLerped(team2Base, lineLerp);
        ofSetColor(lineLerp, lineAlpha);
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

void EnvironmentThreeSystem::particleInteractions(){
    

    for(int i = 0; i < particles.size(); i++) {
        E3Particle& cur = particles[i];
        // global force on other particles
    
        
        vector<E3Particle*> nei = getNeighbors(cur.x, cur.y, 50);
        
        vector<E3Particle*> clusters = getNeighbors(cur.x, cur.y, 10);
        
        vector<E3Particle*> global = getNeighbors(cur.x, cur.y, 80);//externalRad/3);
        
        for(int j = 0; j < nei.size(); j ++){
            
            addRepulsionForce(cur, particleNeighborhood, 0.04);
            
            
            if(cur.team != nei[j] -> team){
                ofPushStyle();
                ofSetColor(255);
                ofDrawLine(cur.x, cur.y, nei[j] -> x, nei[j] -> y);
                addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
                ofPopStyle();
            }
            
            if(cur.team == nei[j] -> team){
                addAttractionForce(cur, particleNeighborhood, 0.06);
            }
        }
        
//OUTPUT CONDITION
        //Send an output if the particles are clusered to a certain degree

        for(int j = 0; j < clusters.size(); j ++){
            if(cur.team == clusters[j] -> team){
                float d = ofDist(cur.x, cur.y, clusters[j] -> x, clusters[j] -> y);
                if(d < 10){
                    outputCondition ++;
                }
            }
        }
        
//        cout<< clusterCount << endl;
        
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
        
        // forces on this particle
        cur.bounceOffWalls(true);
        cur.addDampingForce();
        
    }
    
}

void EnvironmentThreeSystem::outputConditions(){
    // TRIGGER FOR OUTPUT
    
    // run the timer for the glow effect
    glowTimer.run();
    
    if(outputCondition > outputThreshold) trigger = true;
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
    
    // reset the cluster tally
    outputCondition = 0;
    
}


void EnvironmentThreeSystem::impactEffect(){
    
//    ofPushStyle();

    
    impactTarget.x = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01), -1, 1, origin.x - 300, origin.x + 300);
    impactTarget.y = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 500), -1, 1, origin.y - 300, origin.y + 300);

    
//    ofSetColor(255, 0, 0   );
    if(impact){
//              ofDrawLine(0, 0, 100, 100);
//        ofDrawCircle(impactTarget, 200);
        addRepulsionForce(impactTarget.x, impactTarget.y, 200, 3);
        drawLines = true;
        lineAlpha += 2;
    }
    else{
        drawLines = false;
        lineAlpha -= 2;
    }
//    ofPopStyle();
    if(lineAlpha > 255) lineAlpha = 255;
    if (lineAlpha < 0) lineAlpha = 0;
}



