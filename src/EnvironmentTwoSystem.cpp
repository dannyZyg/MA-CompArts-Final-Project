#include "EnvironmentTwoSystem.h"

EnvironmentTwoSystem::EnvironmentTwoSystem() :
	timeStep(100) {

        
}

void EnvironmentTwoSystem::setup(int width, int height, int k) {
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
    
    
    kParticles = 1000;
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        EnvironmentTwoParticle particle = EnvironmentTwoParticle(x, y, 0, 0);
        
        particles.push_back(particle);
        
        

        
//        particleSystem.add(particle);
        setupColours();
    }
    
    padding = 128;
    timeStep = 100;
    isMousePressed = false;
    slowMotion = true;
    particleNeighborhood = 32;
    particleRepulsion = 0.3;
    centerAttraction = 0;
    drawBalls = true;
    
}


void EnvironmentTwoSystem::setupColours(){
    ofColor cell1Base = ofColor(145,49, 191);
    cell1Col.setBaseColor(cell1Base);
    cell1Col.generateAnalogous();
    
    ofColor cell2Base = ofColor(255,49, 191);
    cell2Col.setBaseColor(cell2Base);
    cell2Col.generateAnalogous();
    
    ofColor cell3Base = ofColor(0,49, 220);
    cell3Col.setBaseColor(cell3Base);
    cell3Col.generateAnalogous();

    
    for(int i = 0; i < particles.size(); i++){
        
        particles[i].colIndex = ofRandom(cell1Col.size());
        
        particles[i].col = ofColor(cell1Col[ofRandom(cell1Col.size())]);
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
}

void EnvironmentTwoSystem::updateColours(){
    
    
    for(int i = 0; i < particles.size(); i ++){
        
        float d = ofDist(particles[i].x, particles[i].y, origin.x, origin.y);
        
        if(d > 0 && d < cells[1]){
            particles[i].col = ofColor(cell1Col[particles[i].colIndex]);
            }
        else if(d > cells[1] && d < cells[2]){
            particles[i].col = ofColor(cell2Col[particles[i].colIndex]);
        }
        else if(d > cells[2] && d < externalRad){
            particles[i].col = ofColor(cell3Col[particles[i].colIndex]);
        }
        
        else{
//            particles[i].col = ofColor(cell1Col[particles[i].colIndex]);
        }
        
        
    }
    
    
}


void EnvironmentTwoSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

void EnvironmentTwoSystem::add(EnvironmentTwoParticle particle) {
	particles.push_back(particle);
}

unsigned EnvironmentTwoSystem::size() const {
	return particles.size();
}

EnvironmentTwoParticle& EnvironmentTwoSystem::operator[](unsigned i) {
	return particles[i];
}

vector<EnvironmentTwoParticle*> EnvironmentTwoSystem::getNeighbors(EnvironmentTwoParticle& particle, float radius) {
	return getNeighbors(particle.x, particle.y, radius);
}

vector<EnvironmentTwoParticle*> EnvironmentTwoSystem::getNeighbors(float x, float y, float radius) {
	vector<EnvironmentTwoParticle*> region = getRegion(
		(int) (x - radius),
		(int) (y - radius),
		(int) (x + radius),
		(int) (y + radius));
	vector<EnvironmentTwoParticle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		EnvironmentTwoParticle& cur = *region[i];
		xd = cur.x - x;
		yd = cur.y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

vector<EnvironmentTwoParticle*> EnvironmentTwoSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<EnvironmentTwoParticle*> region;
	back_insert_iterator< vector<EnvironmentTwoParticle*> > back = back_inserter(region);
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
			vector<EnvironmentTwoParticle*>& cur = bins[y * xBins + x];
			copy(cur.begin(), cur.end(), back);
		}
	}
	return region;
}

void EnvironmentTwoSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	n = particles.size();
	unsigned xBin, yBin, bin;
	for(int i = 0; i < n; i++) {
		EnvironmentTwoParticle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.x) >> k;
		yBin = ((unsigned) cur.y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(&cur);
	}
}

void EnvironmentTwoSystem::addRepulsionForce(const EnvironmentTwoParticle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentTwoSystem::addRepulsionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, scale);
}

void EnvironmentTwoSystem::addAttractionForce(const EnvironmentTwoParticle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentTwoSystem::addAttractionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, -scale);
}

void EnvironmentTwoSystem::addForce(const EnvironmentTwoParticle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, radius, -scale);
}

void EnvironmentTwoSystem::addForce(float targetX, float targetY, float radius, float scale) {
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
			vector<EnvironmentTwoParticle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				EnvironmentTwoParticle& curBinnedParticle = *(curBin[i]);
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

void EnvironmentTwoSystem::update(float lastTimeStep) {
	int n = particles.size();
	float curTimeStep = lastTimeStep * timeStep;
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition(curTimeStep);
	}
    
//    particleRepulsion = ofMap(sin(ofGetFrameNum() * 0.02 + 500), -1, 1, 0.2, 1);
//    centerAttraction = ofMap(sin(ofGetFrameNum() * 0.02), -1, 1, 0.2, 1);

    updateColours();

    
}

void EnvironmentTwoSystem::draw() {
	int n = particles.size();
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
        particles[i].draw();
	glEnd();
    
    
}

int EnvironmentTwoSystem::getWidth() const {
	return width;
}

int EnvironmentTwoSystem::getHeight() const {
	return height;
}


void EnvironmentTwoSystem::display(){

    
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
    for(int i = 0; i < particles.size(); i++) {
        EnvironmentTwoParticle& cur = particles[i];
        // global force on other particles
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
//        cur.bounceOffWalls();
        cur.bounceOffCells();
        cur.addDampingForce();
        
        
        
        if(i > 0 && i < 700){
            particles[i].bounceOffCells(0.3, cells[1], 0);
            
            
        }
        
        
        
        
        
        
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
        }
    }
    
    
//    particleSystem.display();
    ofPopMatrix();

    
}


//void EnvironmentTwoSystem::receiveGeometry(ofVec2f origin_, float externalRad_){
//    
//    origin = origin_;
//    externalRad = externalRad_;
//    
//}

void EnvironmentTwoSystem::receiveCells(vector <float> cells_){
    cells = cells_;
}

//void EnvironmentTwoSystem::bounceOffCells(){
//    if(
//
//
//}



