#include "EnvironmentTwoSystem.h"

EnvironmentTwoSystem::EnvironmentTwoSystem(){
    kParticles = 300;
    particleNeighborhood = 32;
    particleRepulsion = 0.5;
    centerAttraction = 0;
    particleAttraction = 0;
    drawLines = true;
    cellWallsActive = true;
    lineAlpha = 0;
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
//    brightness = 200;
//    saturation = 200;
    
    for(int i = 0; i < kParticles; i++) {
        
        float x = ofRandom(origin.x - 100, origin.x + 100);
        float y = ofRandom(origin.y - 100, origin.y + 100);;
        
        E2Particle particle = E2Particle();
        
        particles.push_back(particle);
        setupColours();
    }
    
    glowTimer.setup();
    glow = false;
    
}


void EnvironmentTwoSystem::setupColours(){
    cell1Base = ofColor(145,49, 191);
    cell1Col.setBaseColor(cell1Base);
    cell1Col.generateAnalogous();
    
    cell2Base = ofColor(0,49, 220);
    cell2Col.setBaseColor(cell2Base);
    cell2Col.generateAnalogous();
    
    cell3Base = ofColor(255,49, 191);
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
    }
}

E2Particle& EnvironmentTwoSystem::operator[](unsigned i) {
    return particles[i];
}

vector<E2Particle*> EnvironmentTwoSystem::getNeighbors(E2Particle& particle, float radius) {
    return getNeighbors(particle.x, particle.y, radius);
}

vector<E2Particle*> EnvironmentTwoSystem::getNeighbors(float x, float y, float radius) {
    vector<E2Particle*> region = getRegion(
                                                       (int) (x - radius),
                                                       (int) (y - radius),
                                                       (int) (x + radius),
                                                       (int) (y + radius));
    vector<E2Particle*> neighbors;
    int n = region.size();
    float xd, yd, rsq, maxrsq;
    maxrsq = radius * radius;
    for(int i = 0; i < n; i++) {
        E2Particle& cur = *region[i];
        xd = cur.x - x;
        yd = cur.y - y;
        rsq = xd * xd + yd * yd;
        if(rsq < maxrsq)
            neighbors.push_back(region[i]);
    }
    return neighbors;
}

vector<E2Particle*> EnvironmentTwoSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
    vector<E2Particle*> region;
    back_insert_iterator< vector<E2Particle*> > back = back_inserter(region);
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
            vector<E2Particle*>& cur = bins[y * xBins + x];
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
        E2Particle& cur = particles[i];
        cur.resetForce();
        xBin = ((unsigned) cur.x) >> k;
        yBin = ((unsigned) cur.y) >> k;
        bin = yBin * xBins + xBin;
        if(xBin < xBins && yBin < yBins)
            bins[bin].push_back(&cur);
    }
}


void EnvironmentTwoSystem::add(E2Particle particle) {
    particles.push_back(particle);
}

void EnvironmentTwoSystem::addRepulsionForce(const E2Particle& particle, float radius, float scale) {
    addRepulsionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentTwoSystem::addRepulsionForce(float x, float y, float radius, float scale) {
    addForce(x, y, radius, scale);
}

void EnvironmentTwoSystem::addAttractionForce(const E2Particle& particle, float radius, float scale) {
    addAttractionForce(particle.x, particle.y, radius, scale);
}

void EnvironmentTwoSystem::addAttractionForce(float x, float y, float radius, float scale) {
    addForce(x, y, radius, -scale);
}

void EnvironmentTwoSystem::addForce(const E2Particle& particle, float radius, float scale) {
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
            vector<E2Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                E2Particle& curBinnedParticle = *(curBin[i]);
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

void EnvironmentTwoSystem::update() {
    int n = particles.size();
    for(int i = 0; i < n; i++) {
        particles[i].updatePosition();
        particles[i].receiveCells(cells);
        if(pingFromWalls) particles[i].returnFromWall();
    }
}

void EnvironmentTwoSystem::draw() {
    int n = particles.size();
    glBegin(GL_POINTS);
    for(int i = 0; i < n; i++)
        particles[i].draw();
    glEnd();
}

void EnvironmentTwoSystem::display(){
    float test = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, particles.size());

//    presetSelector("p3");
    
    
    // do this once per frame
    setupForces();
    impactEffect();
    
    if(randomVals){
        //change rules
        
        particleRepulsion = ofRandom(0.1, 1.);
        particleAttraction = ofRandom(0.1, 0.5);
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = ofRandom(3);
            particles[i].vel.x = ofRandom(-2, 2);
            particles[i].vel.y = ofRandom(-2, 2);

        }
        int r1 = ofRandom(2);
        int r2 = ofRandom(2);
        if(r1 == 1) pingFromWalls = true;
        if(r1 == 0) pingFromWalls = false;
        if(r2 == 1) cellWallsActive = true;
        if(r2 == 0) cellWallsActive = false;

        cout<< particleRepulsion <<endl;
        cout<< "ping " << pingFromWalls << endl;
        cout <<"cellWallsActive " << cellWallsActive <<endl;
        
        randomVals = false;
    }
    
    
    
    
    ofPushMatrix();
    
    // apply per-particle forces
    if(drawLines) {
        float lineLerp = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 255), -1, 1, 0, 1);
        ofColor lineCol = cell2Base.getLerped(cell1Base, lineLerp);
        
        
        ofSetColor(lineCol, lineAlpha);
        ofSetLineWidth(2);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    
    
    particleInteractions();
    
    for(int i = 0; i < particles.size(); i++) {
        E2Particle& cur = particles[i];
        // global force on other particles
        
        addAttractionForce(cur, particleNeighborhood, particleAttraction);
        addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        //        cur.bounceOffWalls();
        
        cur.addDampingForce();
        
        //        if(ofGetElapsedTimeMillis() % 1000 == 0) state = !state;
        //        cout<<state<<endl;
        if(cellWallsActive){
            allocateCellState(cur);
            cellWallRebound(cur);
        }
        if(!cellWallsActive){
            cur.bounceOffWalls(true);
        }
    }
    
    if(drawLines) {
        glEnd();
    }
    
    // single-pass global forces
    if(isMousePressed) {
        addRepulsionForce(ofGetMouseX(), ofGetMouseY(), 200, 1);
    }
    update();
    
    // draw all the particles
    
        for(int i = 0; i < particles.size(); i++) {
            particles[i].displayParticle();
        }
    
    outputConditions();
    ofPopMatrix();

}


void EnvironmentTwoSystem::particleInteractions(){

    
}

void EnvironmentTwoSystem::outputConditions(){
    // TRIGGER FOR OUTPUT
    

    
}


void EnvironmentTwoSystem::impactEffect(){
    
    ofVec2f target;
    target.x = ofMap(sin(ofGetFrameNum() * 0.01), -1, 1, origin.x - externalRad, origin.x + externalRad);
    target.y = ofMap(sin(ofGetFrameNum() * 0.01 + 654), -1, 1, origin.x - externalRad, origin.x + externalRad);
    
    if(impact){
        addRepulsionForce(target.x, target.y, 100, 0.5);
//        drawLines = true;
        lineAlpha += 2;
        if(lineAlpha > 255) lineAlpha = 255;
    }
    else{
        lineAlpha -=2;
        if (lineAlpha < 0) lineAlpha = 0;
//        drawLines = false;
    }
}


void EnvironmentTwoSystem::receiveCells(vector <float> cells_){
    cells = cells_;
}


void EnvironmentTwoSystem::cellWallRebound(E2Particle& particle){
    
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
        particle.bounceOffWalls(true);
    }
    
}

void EnvironmentTwoSystem::allocateCellState(E2Particle& particle){
    
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

void EnvironmentTwoSystem::newRules(int option){
    
    // predefined behaviours
    if(option == 0) presetSelector("p1");
    if(option == 1) presetSelector("p2");
    if(option == 2) presetSelector("p3");
    
    //random behaviours
    if(option == 3) presetSelector("r1");
    if(option == 4) presetSelector("r2");
    if(option == 5) presetSelector("r3");
}

void EnvironmentTwoSystem::presetSelector(string preset){
    
    // cell walls active. Outside cells retreat from outer wall.
    if(preset == "p1"){
        particleRepulsion = 0.5;
        centerAttraction = 0;
        cellWallsActive = true;
        pingFromWalls = true;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 0.4;
            
            
        }
        


    }
    
    if(preset == "p2"){
        pingFromWalls = true;
        cellWallsActive = false;
        particleRepulsion = 0.5;// 0.5;
        centerAttraction = 0;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 5;
        }
    }
    
    if(preset == "p3"){
        
        cellWallsActive = false;
        particleRepulsion = 0.4;
        particleAttraction = 0.3;
        pingFromWalls = false;
        

//        cout<<particles[0].xv<<endl;
        
        for(int i = 0; i < particles.size(); i ++){
            particles[i].maxSpeed = 10;
//            particles[i].vel.x = 3;
//            particles[i].vel.y = 3;
//            particles[i].vel.x = ofMap(ofSignedNoise(ofGetFrameNum() + i * 25), -1, 1, -2, 2);
//            particles[i].vel.y = ofMap(ofSignedNoise(ofGetFrameNum() + i * 25 + 500), -1, 1, -2, 2);
//            particles[i].xv = ofRandom(-2, 2);
//            particles[i].yv = ofRandom(-2, 2);
        }
        //        numActive = 200;
        //        maxParticles = 150;
    }
    
    if(preset == "r1"){
        
    }
    
    if(preset == "r2"){
        
    }
    
    if(preset == "r3"){
        
    }
    
}


