#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
	timeStep(100) {
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
    brightness = 200;
    saturation = 200;
    
    kParticles = 300;
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        Particle particle = Particle();
        particles.push_back(particle);
    }
    
    
    
    padding = 128;
    timeStep = 100;
    isMousePressed = false;
    slowMotion = true;
    particleNeighborhood = 32;
    particleRepulsion = 0.1;
    centerAttraction = 0.3;
    drawBalls = true;
}

void ParticleSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

void ParticleSystem::add(Particle particle) {
	particles.push_back(particle);
}

unsigned ParticleSystem::size() const {
	return particles.size();
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

void ParticleSystem::update(float lastTimeStep) {
    int n = particles.size();
    float curTimeStep = lastTimeStep * timeStep;
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

int ParticleSystem::getWidth() const {
	return width;
}

int ParticleSystem::getHeight() const {
	return height;
}


void ParticleSystem::display(){

    
    setTimeStep(timeStep);
    // do this once per frame
    setupForces();
    
    ofVec2f target;
    target.x = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, origin.x - externalRad, origin.x + externalRad);
     target.y = ofMap(sin(ofGetFrameNum() * 0.01 + 654), -1, 1, origin.x - externalRad, origin.x + externalRad);
    
    if(impact){
        addRepulsionForce(target.x, target.y, 100, 0.5);
    }
    
    ofPushMatrix();
    
    // apply per-particle forces
    if(!drawBalls) {
        ofSetColor(24, 124, 174);
        ofSetLineWidth(0.1);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    for(int i = 0; i < particles.size(); i++) {
        Particle& cur = particles[i];
        // global force on other particles
        
        
        
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        cur.bounceOffWalls();
        
        cur.addDampingForce();
    
//        if(ofGetElapsedTimeMillis() % 1000 == 0) state = !state;
//        cout<<state<<endl;

        
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

    
    
    
//    b.updatePosition();
//    b.displayParticle();
//    b.test();
    
}


void E2System::receiveCells(vector <float> cells_){
    cells = cells_;
}

void E2System::u(){
//    updateColours();
//    particles[i].receiveCells(cells);
    
}

void E2System::behaviourState(){
    
    for(int i = 0; i < particles.size(); i++) {
        E2Particle& cur = particles[i];

        if(state){
            allocateCellState(cur);
            cellWallRebound(cur);
        }
        if(!state){
            cur.bounceOffWalls();
            ofSetColor(255, 0, 0);
            ofDrawRectangle(255,255, 100, 100);
        }
    }
    cout<< "state = " <<state << endl;
}


void E2System::cellWallRebound(E2Particle& particle){
    
    // if statements telling the particles to stay within their allocated cell walls
    
    if(particle.cellState == 0){
        particle.bounceOffOuterCell(cells[1]);
    }
    
    else if(particle.cellState == 1){
        particle.bounceOffInnerCell(cells[1]);
        particle.bounceOffOuterCell(cells[2]);
    }
    else if(particle.cellState == 2){
        particle.bounceOffInnerCell(cells[2]);
        particle.bounceOffWalls();
    }
    
}

void E2System::allocateCellState(E2Particle& particle){
    
    float d = ofDist(particle.x, particle.y, origin.x, origin.y);
    if(d > 0 && d < cells[1]){
        particle.cellState = 0;
    }
    else if(d > cells[1] && d < cells[2]){
        particle.cellState = 1;

    }
    else if(d > cells[2] && d < externalRad){
        particle.cellState = 2;

        
    }
}

void E2System::setupColours(){
    ofColor cell1Base = ofColor(145,49, 191);
    cell1Col.setBaseColor(cell1Base);
    cell1Col.generateAnalogous();
    
    ofColor cell2Base = ofColor(0,49, 220);
    cell2Col.setBaseColor(cell2Base);
    cell2Col.generateAnalogous();
    
    ofColor cell3Base = ofColor(255,49, 191);
    cell3Col.setBaseColor(cell3Base);
    cell3Col.generateAnalogous();
    
    
    for(int i = 0; i < particles.size(); i++){
        
        particles[i].colIndex = ofRandom(cell1Col.size());
        
        particles[i].col = ofColor(cell1Col[ofRandom(cell1Col.size())]);
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
}

void E2System::updateColours(){
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


