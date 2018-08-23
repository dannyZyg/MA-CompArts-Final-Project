#include "StoneParticleSystem.h"

StoneParticleSystem::StoneParticleSystem(){
        
}

void StoneParticleSystem::setup(int width, int height, int k) {
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
    
    showParticleSpacing = 5;
    
    numToDisplay = 0;
    kParticles = 50;
    for(int i = 0; i < kParticles; i++) {

        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;

        StoneParticle particle = StoneParticle();

        particles.push_back(particle);
    }
    
    for(int i = 0; i < particles.size(); i++){
        particles[i].origin = origin;
        particles[i].externalRad = externalRad;
    }
    
    setupColours(baseColour);
    
    padding = 128;
    timeStep = 100;
    isMousePressed = false;
    slowMotion = true;
    particleNeighborhood = 32;
    particleRepulsion = 0.7;
    centerAttraction = 0;
    drawBalls = true;
    
    active = false;
    timer = 0;
    timeIntervalPassed = 0;
    drawLines = true;
    
}


void StoneParticleSystem::setupColours(ofColor base_){
    
    
    team1Col.setBaseColor(base_);
    team1Col.generateAnalogous();
    
    for(int i = 0; i < particles.size(); i++){
        particles[i].col = ofColor(team1Col[ofRandom(team1Col.size())]);
    }
    
}

void StoneParticleSystem::updateColours(){
    
//    for(int i = 0; i < particles.size(); i++){
//        
//        
//        if(sensor){
//            particles[i].col = ofColor(0, 255, 0);
//            
//            
//        }
//        else{
//            particles[i].col = ofColor(team1Col[ofRandom(team1Col.size())]);
//        }
//
//    }
    
    
}

void StoneParticleSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

void StoneParticleSystem::addParticle() {
    
    float x = ofRandom(origin.x, origin.x);
    float y = ofRandom(origin.y, origin.y);;
    
    StoneParticle particle = StoneParticle();
    
    particles.push_back(particle);
}

unsigned StoneParticleSystem::size() const {
	return particles.size();
}

StoneParticle& StoneParticleSystem::operator[](unsigned i) {
	return particles[i];
}

vector<StoneParticle*> StoneParticleSystem::getNeighbors(StoneParticle& particle, float radius) {
	return getNeighbors(particle.x, particle.y, radius);
}

vector<StoneParticle*> StoneParticleSystem::getNeighbors(float x, float y, float radius) {
	vector<StoneParticle*> region = getRegion(
		(int) (x - radius),
		(int) (y - radius),
		(int) (x + radius),
		(int) (y + radius));
	vector<StoneParticle*> neighbors;
	int n = region.size();
	float xd, yd, rsq, maxrsq;
	maxrsq = radius * radius;
	for(int i = 0; i < n; i++) {
		StoneParticle& cur = *region[i];
		xd = cur.x - x;
		yd = cur.y - y;
		rsq = xd * xd + yd * yd;
		if(rsq < maxrsq)
			neighbors.push_back(region[i]);
	}
	return neighbors;
}

vector<StoneParticle*> StoneParticleSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
	vector<StoneParticle*> region;
	back_insert_iterator< vector<StoneParticle*> > back = back_inserter(region);
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
			vector<StoneParticle*>& cur = bins[y * xBins + x];
			copy(cur.begin(), cur.end(), back);
		}
	}
	return region;
}

void StoneParticleSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	n = particles.size();
	unsigned xBin, yBin, bin;
	for(int i = 0; i < n; i++) {
		StoneParticle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.x) >> k;
		yBin = ((unsigned) cur.y) >> k;
		bin = yBin * xBins + xBin;
		if(xBin < xBins && yBin < yBins)
			bins[bin].push_back(&cur);
	}
}

void StoneParticleSystem::addRepulsionForce(const StoneParticle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, radius, scale);
}

void StoneParticleSystem::addRepulsionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, scale);
}

void StoneParticleSystem::addAttractionForce(const StoneParticle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, radius, scale);
}

void StoneParticleSystem::addAttractionForce(float x, float y, float radius, float scale) {
	addForce(x, y, radius, -scale);
}

void StoneParticleSystem::addForce(const StoneParticle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, radius, -scale);
}

void StoneParticleSystem::addForce(float targetX, float targetY, float radius, float scale) {
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
			vector<StoneParticle*>& curBin = bins[y * xBins + x];
			int n = curBin.size();
			for(int i = 0; i < n; i++) {
				StoneParticle& curBinnedParticle = *(curBin[i]);
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

void StoneParticleSystem::update() {
	int n = particles.size();
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition();
	}
}

void StoneParticleSystem::draw() {
    if(active){
        int n = particles.size();
        glBegin(GL_POINTS);
        for(int i = 0; i < n; i++)
            particles[i].draw();
        glEnd();
    }
}

int StoneParticleSystem::getWidth() const {
	return width;
}

int StoneParticleSystem::getHeight() const {
	return height;
}


void StoneParticleSystem::display(){
    

//    fadeParticles();
    
    ofPushStyle();
    ofPushMatrix();
    
    
    // do this once per frame
    setupForces();
    
    if(sensor && active) drawLines = true;
    else drawLines = false;
    
    // apply per-particle forces
    if(drawLines) {
        ofSetColor(24, 124, 174);
        ofSetLineWidth(2);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    for(int i = 0; i < particles.size(); i++) {
        StoneParticle& cur = particles[i];
        // global force on other particles
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        cur.bounceOffWalls(false);
        cur.addDampingForce();
        
    }
    if(drawLines) {
        glEnd();
    }
    
    // single-pass global forces
    addAttractionForce(origin.x, origin.y, 200, centerAttraction);
    if(isMousePressed) {
    addRepulsionForce(ofGetMouseX(), ofGetMouseY(), 200, 1);
    }
    update();
    

    
    ofPopStyle();
    ofPopMatrix();
    

    
    
    

}



void StoneParticleSystem::reset(){
    
    particles.clear();
}


void StoneParticleSystem::fadeParticles(){
    
    for(int i = 0; i < numToDisplay; i++) {
        particles[i].displayParticle();
    }
    
    if(active){
        if(timer % showParticleSpacing == 0){
            numToDisplay ++;
            if(numToDisplay >= kParticles){
                numToDisplay = kParticles;
            }
        }
    }
    
    if(!active){
          if(timer % showParticleSpacing == 0){
              numToDisplay --;
              if(numToDisplay <= 0){
                  numToDisplay = 0;
              }
          }
    }
    
    timer ++;
    
    if(timer > 50000)timer = 0;
    
}

// Make changes based on which system sent the signal
void StoneParticleSystem::originSystem(string originSystem_){
    newColours(originSystem_);
    
}

void StoneParticleSystem::newColours(string originSystem_){
    
    if(originSystem_ == "Sensor"){
        ofColor c = ofColor(255, 0, 0);
        setupColours(c);
    }
    if(originSystem_ == "Environment One"){
        ofColor c = ofColor(0, 255, 0);
        setupColours(c);
    }
    if(originSystem_ == "Environment Two"){
        ofColor c = ofColor(255, 0, 255);
        setupColours(c);
    }
    if(originSystem_ == "Environment Three"){
        ofColor c = ofColor(255,211, 91);
        setupColours(c);
    }
    
}
