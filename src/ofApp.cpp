#include "ofApp.h"

void ofApp::setup(){
    
    
    
    
    ofSetCircleResolution(100);
//    ofSetFullscreen(true);
    displayCircleTemplate = false;
	ofBackground(0);
    ofSetCircleResolution(100);
    debug = false;

	// Register our sources.
	// This should be done before mapper.setup().
    piMapper.registerFboSource(stones_1_4);
    piMapper.registerFboSource(large_stones);
    piMapper.registerFboSource(med_stones);

    piMapper.registerFboSource(environmentOne);
    piMapper.registerFboSource(environmentTwo);
    piMapper.registerFboSource(environmentThree);
//    piMapper.registerFboSource(stones_5_8);
//    piMapper.registerFboSource(type1);

	piMapper.setup();

	// This will set the app fullscreen if compiled on Raspberry Pi.
	#ifdef TARGET_RASPBERRY_PI
		ofSetFullscreen(true);
	#endif
    
    layout.load("layout.png");
   
    drawTemplate = false;
    
}

void ofApp::update(){
	piMapper.update();
    scheduler();
    
}

void ofApp::draw(){
 
    
    ofBackground(0);
    
    if(drawTemplate) layout.draw(0, 0);
    
    // a template circle for making a perfect 'circle' and not an ellipse
    if(displayCircleTemplate){
        ofPushStyle();
        ofSetColor(255);
        ofDrawRectangle(150, 150, 300, 300);
        ofPopStyle();
    }
    
    // draw the piMapper interface and surfaces
	piMapper.draw();
    
    // continuation of the circle template
    if(displayCircleTemplate){
        ofPushStyle();
        ofSetColor(0, 255, 0);
        ofDrawCircle(300, 300, 150);
        ofPopStyle();
    }
 
    ofSetColor(255);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
    
}

void ofApp::keyPressed(int key){
	piMapper.keyPressed(key);
    
//    if(key == 'b') stones_1_4.sequence1();
//    if(key == 'n') stones_1_4.sequence2();
//    if(key == 'm') stones_1_4.reset();
    
    //display the circle
    if(key == 'w') displayCircleTemplate = !displayCircleTemplate;
//    if(key == )\
    
    if(key == '/') drawTemplate = !drawTemplate;
    
    if(key == 'd') {
        debug = !debug;
        
        environmentOne.debug = !debug;
        environmentTwo.debug = !debug;
        environmentThree.debug = !debug;
        stones_1_4.debug = ! debug;
        
        cout << "DEBUG = " << debug << endl;
        
        
        
        
        
    }
    
}

void ofApp::keyReleased(int key){
	piMapper.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int button){
	piMapper.mousePressed(x, y, button);
}

void ofApp::mouseReleased(int x, int y, int button){
	piMapper.mouseReleased(x, y, button);
}

void ofApp::mouseDragged(int x, int y, int button){
	piMapper.mouseDragged(x, y, button);
}



void ofApp::scheduler(){
    
    
    
}


