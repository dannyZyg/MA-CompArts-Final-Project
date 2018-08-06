#include "ofApp.h"

void ofApp::setup(){
    
    verdana30.load("verdana.ttf", 80, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    
    ofSetCircleResolution(100);
    ofSetRectMode(OF_RECTMODE_CENTER);
//    ofSetFullscreen(true);
    displayCircleTemplate = false;
	ofBackground(0);
    ofSetCircleResolution(100);
    debug = false;

	// Register our sources.
	// This should be done before mapper.setup().
    piMapper.registerFboSource(environmentOne);
    piMapper.registerFboSource(environmentTwo);
    piMapper.registerFboSource(environmentThree);
    piMapper.registerFboSource(small_stones_1_4);
    piMapper.registerFboSource(small_stones_5_8);
    piMapper.registerFboSource(small_stones_9_12);
    piMapper.registerFboSource(small_stones_13_16);
    piMapper.registerFboSource(med_stones_1_4);
    piMapper.registerFboSource(med_stones_5_8);
    piMapper.registerFboSource(large_stones);

//Send large font to all FBO sources
    environmentOne.font = verdana30;
    environmentTwo.font = verdana30;
    environmentThree.font = verdana30;
    small_stones_1_4.font = verdana30;
    small_stones_5_8.font = verdana30;
    small_stones_9_12.font = verdana30;
    small_stones_13_16.font = verdana30;
    med_stones_1_4.font = verdana30;
    med_stones_5_8.font = verdana30;
    large_stones.font = verdana30;
    
// setup piMapper
	piMapper.setup();

	// This will set the app fullscreen if compiled on Raspberry Pi.
	#ifdef TARGET_RASPBERRY_PI
		ofSetFullscreen(true);
	#endif
    
    layout.load("layout.png");
   
    drawTemplate = false;
    
    timeSpacing = 1000;
    
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
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 20);
    
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
        small_stones_1_4.debug = ! debug;
        small_stones_5_8.debug = ! debug;
        small_stones_9_12.debug = ! debug;
        small_stones_13_16.debug = ! debug;
        med_stones_1_4.debug = ! debug;
        med_stones_5_8.debug = ! debug;
        large_stones.debug = ! debug;
        
        cout << "DEBUG = " << debug << endl;
    }
    
}

void ofApp::keyReleased(int key){
	piMapper.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int button){
	piMapper.mousePressed(x, y, button);
    
//    small_stones_1_4.stones[2].numToDisplay ++;
//    cout<< small_stones_1_4.stones[2].numToDisplay <<endl;
    
    
}

void ofApp::mouseReleased(int x, int y, int button){
	piMapper.mouseReleased(x, y, button);
}

void ofApp::mouseDragged(int x, int y, int button){
	piMapper.mouseDragged(x, y, button);
}


void ofApp::debugDisplay(){
    
    ofPushStyle();
    ofFill();
    ofSetColor(255);
    ofDrawCircle(environmentOne.origin.x, environmentOne.origin.y, environmentOne.rad);
    verdana30.drawString("Env 1", environmentOne.origin.x, environmentOne.origin.y);
    ofPopStyle();
    
    
}


void ofApp::scheduler(){

//    for(int i = 0; i < small_stones_1_4.stones.size(); i ++){
//        small_stones_1_4.stones[i].active = false;        
//    }
    
    
//    large_stones.stones[0].active = true;

// instructions for the illumination of particular stepping stones in order and in a timed sequence
    large_stones.stones[0].active = true;
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing){
        med_stones_1_4.stones[0].active = true;
    }
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * 2 ){
        small_stones_1_4.stones[2].active = true;
        small_stones_5_8.stones[0].active = true;
    }
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * 3){
        small_stones_1_4.stones[3].active = true;
    }


    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * 5){
        large_stones.stones[0].active = false;
    }

    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * 6){
        med_stones_1_4.stones[0].active = false;
    }
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * 7 ){
        small_stones_1_4.stones[2].active = false;
        small_stones_5_8.stones[0].active = false;
    }
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * 8){
        small_stones_1_4.stones[3].active = false;
    }

    
}

