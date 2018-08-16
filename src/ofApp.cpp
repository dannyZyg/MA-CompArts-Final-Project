#include "ofApp.h"

void ofApp::setup(){
    serialSetup();
    verdana30.load("verdana.ttf", 80, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    
    ofSetCircleResolution(100);
//    ofSetRectMode(OF_RECTMODE_CENTER);
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
    sensorTrigger = false;
    sensorSequenceActive = false;
    sequenceActive = false;
    
    e1Count = 0;
    e2Count = 0;
    e3Count = 0;
    
    activeLength = 3000;
}

void ofApp::update(){
	piMapper.update();
    scheduler();
    serialUpdate();
    
    
//    if(ofGetElapsedTimeMillis() > 2000){
    
        
//    }

    timer = ofGetElapsedTimeMillis() - startTime;

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
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 25);
    ofDrawBitmapString("Sensor Val = " + ofToString(val), 32, 10);
    
    
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
    
    
}

void ofApp::mouseReleased(int x, int y, int button){
	piMapper.mouseReleased(x, y, button);
    
    count++;
    if(count > 2) count =0;
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

//    E3_to_E2(1);
//    E2_to_E3(0);
    

    if(ofGetElapsedTimeMillis() > 0 && val < 250 && !sensorTrigger && !sequenceActive){
        randomPath = ofRandom(3);
        sensorTrigger = true;
        sequenceActive = true;
        resetTimer();
        }
    
    if(sensorTrigger && sequenceActive){
//        sensorToSystem(1);
//        E1_to_E2(2);
//        E1_to_E3(0);
//        E2_to_E3(2);
        E3_to_E1(2);
        
    }
    
    cout<< "SeqAct = " << sequenceActive << endl;
}


void ofApp::sequenceComplete(int timing){
    if (ofGetElapsedTimeMillis() - startTime > (timeSpacing * timing)){
        sequenceActive = false;
        sensorTrigger = false;
    }
}



void ofApp::triggerStone(StoneParticleSystem& stone, int timing){
    //turn on the stone
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * timing){
        stone.active = true;
    }
    //turn off the stone
    if( ofGetElapsedTimeMillis() - startTime > timeSpacing * timing + activeLength){
        stone.active = false;
    }
}

void ofApp::triggerEnviro1(int timing){
    //turn on the stone
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * timing){
        environmentOne.active = true;
    }
    //turn off the stone
    if( ofGetElapsedTimeMillis() - startTime > timeSpacing * timing + activeLength){
        environmentOne.active = false;
        sensorTrigger = false;
    }
}

void ofApp::triggerEnviro2(int timing){
    //turn on the stone
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * timing){
        environmentTwo.active = true;
    }
    //turn off the stone
    if( ofGetElapsedTimeMillis() - startTime > timeSpacing * timing + activeLength){
        environmentTwo.active = false;
        sensorTrigger = false;

    }
}

void ofApp::triggerEnviro3(int timing){
    //turn on the stone
    if (ofGetElapsedTimeMillis() - startTime > timeSpacing * timing){
        environmentThree.active = true;
    }
    //turn off the stone
    if( ofGetElapsedTimeMillis() - startTime > timeSpacing * timing + activeLength){
        environmentThree.active = false;
        
        
        e3Count ++;
        if(e3Count > 2) e3Count = 0;
    }
}


// instructions for the illumination of particular stepping stones in order and in a timed sequence

void ofApp::E1_to_E2(int variation){
    
    if(variation == 0){
        triggerStone(med_stones_5_8.stones[2], 0);
        triggerStone(large_stones.stones[2], 1);
        triggerStone(med_stones_5_8.stones[3], 2);
        triggerStone(large_stones.stones[3], 3);
        triggerStone(small_stones_13_16.stones[2], 4);
        triggerEnviro2(5);
        
    }
    if(variation == 1){
        triggerStone(small_stones_9_12.stones[1], 0);
        triggerStone(small_stones_9_12.stones[2], 1);
        triggerStone(small_stones_13_16.stones[0], 2);
        triggerStone(large_stones.stones[3], 3);
        triggerStone(small_stones_13_16.stones[2], 4);
        triggerEnviro2(5);
    }
    if(variation == 2){
        triggerStone(small_stones_5_8.stones[3], 0);
        triggerStone(small_stones_9_12.stones[0], 1);
        triggerStone(med_stones_5_8.stones[0], 2);
        triggerStone(med_stones_5_8.stones[1], 3);
        triggerStone(large_stones.stones[2], 4);
        triggerStone(small_stones_5_8.stones[2], 5);
        triggerEnviro2(6);
    }
}
void ofApp::E1_to_E3(int variation){
    
    if(variation == 0){
        triggerStone(small_stones_5_8.stones[3], 0);
        triggerStone(small_stones_9_12.stones[0], 1);
        triggerStone(med_stones_5_8.stones[1], 2);
        triggerStone(med_stones_5_8.stones[0], 3);
        triggerStone(large_stones.stones[1], 4);
        triggerStone(med_stones_1_4.stones[3], 5);
        triggerStone(med_stones_1_4.stones[2], 6);
        triggerStone(small_stones_5_8.stones[1], 7);
        triggerEnviro3(8);
//        sequenceComplete(9);
    }
    if(variation == 1){
        triggerStone(med_stones_5_8.stones[2], 0);
        triggerStone(large_stones.stones[2], 1);
        triggerStone(small_stones_5_8.stones[2], 2);
        triggerStone(med_stones_5_8.stones[1], 3);
        triggerStone(med_stones_5_8.stones[0], 4);
        triggerStone(large_stones.stones[1], 5);
        triggerStone(med_stones_1_4.stones[3], 6);
        triggerStone(small_stones_5_8.stones[1], 7);
        triggerEnviro3(8);
    }
    if(variation == 2){
        triggerStone(small_stones_9_12.stones[0], 0);
        triggerStone(med_stones_5_8.stones[1], 1);
        triggerStone(med_stones_5_8.stones[0], 2);
        triggerStone(large_stones.stones[1], 3);
        triggerStone(med_stones_1_4.stones[3], 4);
        triggerStone(small_stones_5_8.stones[1], 5);
        triggerEnviro3(6);
    }
    
}

void ofApp::E2_to_E1(int variation){
    
    if(variation == 0){
        triggerStone(small_stones_13_16.stones[2], 0);
        triggerStone(large_stones.stones[3], 1);
        triggerStone(med_stones_5_8.stones[3], 2);
        triggerStone(small_stones_13_16.stones[0], 3);
        triggerStone(small_stones_9_12.stones[2], 4);
        triggerStone(small_stones_9_12.stones[1], 5);
        triggerEnviro1(6);
        
    }
    if(variation == 1){
        
        triggerStone(small_stones_5_8.stones[2], 0);
        triggerStone(med_stones_5_8.stones[1], 1);
        triggerStone(med_stones_5_8.stones[0], 2);
        triggerStone(small_stones_5_8.stones[3], 3);
        triggerStone(small_stones_9_12.stones[0], 4);
        triggerStone(med_stones_5_8.stones[2], 5);
        triggerEnviro1(6);
    }
    if(variation == 2){
        triggerStone(small_stones_5_8.stones[2], 0);
        triggerStone(med_stones_5_8.stones[1], 1);
        triggerStone(med_stones_5_8.stones[0], 2);
        triggerStone(small_stones_5_8.stones[3], 3);
        triggerStone(small_stones_9_12.stones[0], 4);
        triggerStone(med_stones_5_8.stones[2], 5);
        triggerEnviro1(6);
    }
}
void ofApp::E2_to_E3(int variation){
    
    if(variation == 0){
        triggerStone(small_stones_1_4.stones[3], 0);
        triggerStone(small_stones_5_8.stones[0], 1);
        triggerStone(small_stones_1_4.stones[2], 2);
        triggerStone(med_stones_1_4.stones[0], 3);
        triggerStone(large_stones.stones[0], 4);
        triggerStone(small_stones_1_4.stones[1], 5);
        triggerStone(small_stones_1_4.stones[0], 6);
    }
    if(variation == 1){
        triggerStone(small_stones_5_8.stones[2], 0);
        triggerStone(med_stones_5_8.stones[1], 1);
        triggerStone(med_stones_5_8.stones[0], 2);
        triggerStone(large_stones.stones[1], 3);
        triggerStone(med_stones_1_4.stones[3], 4);
        triggerStone(med_stones_1_4.stones[2], 5);
        triggerStone(small_stones_5_8.stones[1], 6);
        triggerEnviro3(7);
        
    }
    if(variation == 2){
        
        triggerStone(small_stones_1_4.stones[3], 0);
        triggerStone(small_stones_5_8.stones[0], 1);
        triggerStone(med_stones_1_4.stones[0], 2);
        triggerStone(small_stones_1_4.stones[2], 3);
        triggerStone(med_stones_1_4.stones[1], 4);
        triggerEnviro3(5);
    }
}

void ofApp::E3_to_E1(int variation){
    
    if(variation == 0){
       triggerStone(small_stones_5_8.stones[1], 0);
       triggerStone(med_stones_1_4.stones[2], 1);
       triggerStone(large_stones.stones[1], 2);
       triggerStone(med_stones_5_8.stones[0], 3);
       triggerStone(small_stones_5_8.stones[3], 4);
       triggerEnviro1(5);
        
    }
    if(variation == 1){
        triggerStone(small_stones_5_8.stones[1], 0);
        triggerStone(med_stones_1_4.stones[2], 1);
        triggerStone(large_stones.stones[1], 2);
        triggerStone(med_stones_5_8.stones[0], 3);
        triggerStone(med_stones_5_8.stones[1], 4);
        triggerStone(small_stones_9_12.stones[0], 5);
        triggerEnviro1(6);
    }
    if(variation == 2){
        triggerStone(small_stones_5_8.stones[1], 0);
        triggerStone(med_stones_1_4.stones[3], 1);
        triggerStone(large_stones.stones[1], 2);
        triggerStone(med_stones_5_8.stones[0], 3);
        triggerStone(med_stones_5_8.stones[1], 4);
        triggerStone(large_stones.stones[2], 5);
        triggerStone(med_stones_5_8.stones[2], 6);
        triggerEnviro1(7);
        
        
        
        
        
 

    }
}

void ofApp::E3_to_E2(int variation){
    
    if(variation == 0){
        triggerStone(small_stones_1_4.stones[0], 0);
        triggerStone(small_stones_1_4.stones[1], 1);
        triggerStone(large_stones.stones[0], 2);
        triggerStone(med_stones_1_4.stones[0], 3);
        triggerStone(small_stones_5_8.stones[0], 4);
        triggerStone(small_stones_1_4.stones[2], 4);
        triggerStone(small_stones_1_4.stones[3], 5);
        triggerEnviro1(6);
    }
    
    if(variation == 1){
        triggerStone(small_stones_5_8.stones[1], 0);
        triggerStone(med_stones_1_4.stones[2], 1);
        triggerStone(med_stones_1_4.stones[3], 2);
        triggerStone(large_stones.stones[1], 3);
        triggerStone(med_stones_5_8.stones[0], 4);
        triggerStone(med_stones_5_8.stones[1], 5);
        triggerStone(small_stones_5_8.stones[2], 6);
        triggerEnviro1(7);

    }
    if(variation == 2){
        triggerStone(med_stones_1_4.stones[1], 0);
        triggerStone(small_stones_1_4.stones[2], 1);
        triggerStone(med_stones_1_4.stones[0], 2);
        triggerStone(small_stones_5_8.stones[0], 3);
        triggerStone(small_stones_1_4.stones[3], 4);
        triggerEnviro1(5);
    }
}


void ofApp::sensorToSystem(int variation){
    
    sensorSequenceActive = true;
    if(variation == 0){
        
        triggerStone(small_stones_13_16.stones[1], 0);
        triggerStone(small_stones_13_16.stones[0], 1);
        triggerStone(med_stones_5_8.stones[3], 2);
        triggerStone(large_stones.stones[3], 3);
        triggerStone(small_stones_13_16.stones[2], 4);
        triggerEnviro2(5);
        
    }
    if(variation == 1){
        triggerStone(small_stones_13_16.stones[1], 0);
        triggerStone(small_stones_13_16.stones[0], 1);
        triggerStone(small_stones_9_12.stones[2], 2);
        triggerStone(small_stones_9_12.stones[3], 3);
        triggerStone(small_stones_9_12.stones[1], 4);
        triggerEnviro1(5);
    }
    
}

void ofApp::resetTimer(){
    startTime = ofGetElapsedTimeMillis();
}




///////SERIAL///////////



//--------------------------------------------------------------
void ofApp::serialSetup(){
    
    // code from Joshua Noble's "Programming Interactivity: A Designer's Guide to Processing, Arduino & openFrameworks"
    
    countCycles = 0;                                                 // reset the cycle counter
    bSendSerialMessage = true;                                       // send a message to arduino to say oF is ready to receive serial information
    int baud = 115200;                                               // this is the rate the proximity sensor operates at
    serial.setup(0, baud);                                           // open the first device and talk to it at rate of int baud
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList(); // list the devices/ports available for serial communication
    
}

//--------------------------------------------------------------
void ofApp::serialUpdate(){
    
    // code from Joshua Noble's "Programming Interactivity: A Designer's Guide to Processing, Arduino & openFrameworks"
    
    // serial information is sent as bytes, which need to be converted back into usable values
    
    if(bSendSerialMessage){
        serial.writeByte('x');                                  //send a handshake to the arduino
        
        // make sure there's something to write all the data to
        unsigned char bytesReturned[NUM_BYTES];
        memset(bytesReturned, 0, NUM_BYTES);
        
        // keep reading until there are no bytes left to read
        while(serial.readBytes(bytesReturned, NUM_BYTES) > 0){
            val = bytesReturned[0];
            val <<= 8;                                          // shift values into correct range
            val += bytesReturned[1];
            
            bSendSerialMessage = false;                         // get ready to wait a few frames before asking again
        }
    }
    countCycles++;
    if(countCycles == 1){
        bSendSerialMessage = true;                              // send a message to the arduino to tell it oF is ready for more readings
        countCycles = 0;
    }
}

