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
        
        EnvironmentThreeParticle particle = EnvironmentThreeParticle(x, y, 0, 0);
        
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

void EnvironmentThreeSystem::add(EnvironmentThreeParticle particle) {
	particles.push_back(particle);
}

unsigned EnvironmentThreeSystem::size() const {
	return particles.size();
}

EnvironmentThreeParticle& EnvironmentThreeSystem::operator[](unsigned i) {
	return particles[i];
}

vector<EnvironmentThreeParticle*> EnvironmentThreeSystem::getNeighbors(EnvironmentThreeParticle& particle, float radius) {
	return getNeighbors(particle.x, particle.y, radius);
}

vector<EnvironmentThreeParticle*> EnvironmentThreeSystem::getNeighbors(float x, float y, float radius) {
	vector<EnvironmentThreeParticle*> region = getRegion(
		(int) (x - radius),
		(int) (y - radius),
		(int) (x + radius),
		(int) (y + radius));
	vector<EnvironmentThreeParticle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		EnvironmentThreeParticle& cur = *region[i];
		xd = cur.x - x;
		yd = cur.y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

vector<EnvironmentThreeParticle*> EnvironmentThreeSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<EnvironmentThreeParticle*> region;
	back_insert_iterator< vector<EnvironmentThreeParticle*> > back = back_inserter(region);
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
			vector<EnvironmentThreeParticle*>& cur = bins[y * xBins + x];
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
		EnvironmentThreeParticle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.x) >> k;
		yBin = ((unsigned) cur.y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(&cur);
	}
}

void EnvironmentThreeSystem::addRepulsionForce(const EnvironmentThreeParticle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentThreeSystem::addRepulsionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, scale);
}

void EnvironmentThreeSystem::addAttractionForce(const EnvironmentThreeParticle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentThreeSystem::addAttractionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, -scale);
}

void EnvironmentThreeSystem::addForce(const EnvironmentThreeParticle& particle, float radius, float scale) {
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
			vector<EnvironmentThreeParticle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				EnvironmentThreeParticle& curBinnedParticle = *(curBin[i]);
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
	float curTimeStep = lastTimeStep * timeStep;
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition(curTimeStep);
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
    
    // apply per-particle forces
    if(!drawBalls) {
        ofSetColor(24, 124, 174);
        ofSetLineWidth(0.1);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    
    float tempRad = 50;
    
    for(int i = 0; i < particles.size(); i++) {
        EnvironmentThreeParticle& cur = particles[i];
        // global force on other particles
        
        
        
//        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        
        
        vector<EnvironmentThreeParticle*> nei = getNeighbors(cur.x, cur.y, tempRad);
        
        
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
        
        
            
        
        
        // forces on this particle
        cur.bounceOffWalls();
        cur.addDampingForce();
        
        
        
        
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

