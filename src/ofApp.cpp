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
    
    
    piMapper.registerFboSource(e1test);
    piMapper.registerFboSource(e2test);
    piMapper.registerFboSource(es);

//    piMapper.registerFboSource(stoneSourceTest);
//    piMapper.registerFboSource(testStones);

////Send large font to all FBO sources
    environmentOne.font = verdana30;
    environmentTwo.font = verdana30;
    environmentThree.font = verdana30;
//    small_stones_1_4.font = verdana30;
//    small_stones_5_8.font = verdana30;
//    small_stones_9_12.font = verdana30;
//    small_stones_13_16.font = verdana30;
//    med_stones_1_4.font = verdana30;
//    med_stones_5_8.font = verdana30;
//    large_stones.font = verdana30;
//
//    stoneSourceTest.font = verdana30;
    
    
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
    
    
    activeLength = 3000;
    
    testEnvelope = Envelope();
    
    env1Timer = Timer();
    env2Timer = Timer();
    env3Timer = Timer();
    sensorTimer = Timer();
    
    env1Timer.setup();
    env2Timer.setup();
    env3Timer.setup();
    sensorTimer.setup();
}

void ofApp::update(){
	piMapper.update();
    scheduler();
    serialUpdate();

    
    timerSequenceSpacing = ofGetElapsedTimeMillis() - sequenceSpacingStart;
    
    env1Timer.run();
    env2Timer.run();
    env3Timer.run();
    sensorTimer.run();
    
}

void ofApp::draw(){
 
    float x = testEnvelope.output(env, 50, 100, 100);
    if(testEnvelope.complete) env = false;
    
    
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
    
    
    ofSetColor(255);
    
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
    
    if(key == 'D') {
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
    
    if(key == ' ') environmentThree.enviro.sequenceActive = !environmentThree.enviro.sequenceActive;
 
    if(key == 'B') environmentTwo.blur1 = !environmentTwo.blur1;
    if(key == 'N') environmentTwo.blur2 = !environmentTwo.blur2;
    if(key == 'A') environmentThree.active = !environmentThree.active;

    if(key == 'H') large_stones.stones[1].sens = !large_stones.stones[1].sens;
    if(key == 'J') large_stones.stones[1].env1 = !large_stones.stones[1].env1;
    if(key == 'K') large_stones.stones[1].env2 = !large_stones.stones[1].env2;
    if(key == 'L') large_stones.stones[1].env3 = !large_stones.stones[1].env3;
    
//    if(key == 'R') testStones.stones[0].env1 = !testStones.stones[0].env1;
    
}

void ofApp::keyReleased(int key){
	piMapper.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int button){
	piMapper.mousePressed(x, y, button);
    
    envTest = !envTest;
    env = !env;
    cout<< "env = " << envTest << endl;
    environmentTwo.enviro.randomVals = true;

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
    
//    ofPushStyle();
//    ofFill();
//    ofSetColor(255);
//    ofDrawCircle(environmentOne.origin.x, environmentOne.origin.y, environmentOne.rad);
//    verdana30.drawString("Env 1", environmentOne.origin.x, environmentOne.origin.y);
//    ofPopStyle();
    
    
}

void ofApp::scheduler(){

//SENSOR COMMUNICATION//
    float tempVal = ofMap(mouseX, 0, ofGetWidth(), 0, 1000);
    
    if(ofGetElapsedTimeMillis() > 2000 && tempVal < 100) sensorTrigger = true;
  
    
    else(sensorTrigger = false);
    if(sensorTrigger && !sensorSequenceActive){
        sensorSequenceActive = true;
        sensorTimer.reset();
        
    }

    if(sensorSequenceActive){
        sensorToSystem(sensorTimer, 1);
//        E3_to_E1(randomPath);
    }
    


//ENVIRONMENT COMMUNICATION//
    
// Environment One
// Conditions under which to allow a sequence to start
    if(environmentOne.enviro.sequenceTrigger && !environmentOne.enviro.sequenceActive){
        // if these conditions are met, do the following once only!
        environmentOne.enviro.randomPath = ofRandom(3); // choose random stone path
        environmentOne.enviro.destination = ofRandom(2); // choose random destination enviro
        environmentOne.enviro.sequenceActive = true; // condition to start the sequence
        environmentOne.enviro.sequenceTrigger = false; // reset the trigger condition
        env1Timer.reset(); // start a fresh timer
        env1Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
    }
    // Then display the sequence while the bool is true
    if(environmentOne.enviro.sequenceActive){
        if(environmentOne.enviro.destination == 0){
            E1_to_E2(env1Timer, environmentOne.enviro.randomPath);
        }
        if(environmentThree.enviro.destination == 1){
            E1_to_E3(env1Timer, environmentOne.enviro.randomPath);
        }
    }

// Environment Three
// Conditions under which to allow a sequence to start
    if(environmentThree.enviro.sequenceTrigger && !environmentThree.enviro.sequenceActive){
        // if these conditions are met, do the following once only!
        environmentThree.enviro.randomPath = ofRandom(3); // choose random stone path
        environmentThree.enviro.destination = ofRandom(2); // choose random destination enviro
        environmentThree.enviro.sequenceActive = true; // condition to start the sequence
        environmentThree.enviro.sequenceTrigger = false; // reset the trigger condition
        env3Timer.reset(); // start a fresh timer
        env3Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
    }
// Then display the sequence while the bool is true
    if(environmentThree.enviro.sequenceActive){
        if(environmentThree.enviro.destination == 0){
            E3_to_E1(env3Timer, environmentThree.enviro.randomPath);
        }
        if(environmentThree.enviro.destination == 1){
            E3_to_E2(env3Timer, environmentThree.enviro.randomPath);
        }
    }
}


// function which completes the stone sequence, setting the sequence state to inactive at the end.
void ofApp::sequenceComplete(Timer& t, int timing){
    if (t.timer > (timeSpacing * (timing + 8))){

        //check which timer/environment is being addressed and turn of the relative sequenceActive state
        if(t.timer == env1Timer.timer){
            environmentOne.enviro.sequenceActive = false;
            environmentOne.enviro.systemOutput = false;
        }
        if(t.timer == env2Timer.timer){
            environmentTwo.enviro.sequenceActive = false;
            environmentTwo.enviro.systemOutput = false;
        }
        if(t.timer == env3Timer.timer){
            environmentThree.enviro.systemOutput = false;
            environmentThree.enviro.sequenceActive = false;
        }
        if(t.timer == sensorTimer.timer){
            sensorSequenceActive = false;
        }
        
        

    }
    
}


// function which triggers a stone on and off at the right time in a given sequence
void ofApp::triggerStone(Timer& t, StoneParticleSystem& stone, int timing){
    
    // check which environment/sensor sent the signal and inform the stone
    string sender;
    if(t.timer == env1Timer.timer) sender = "Environment One";
    if(t.timer == env2Timer.timer) sender = "Environment Two";
    if(t.timer == env3Timer.timer) sender = "Environment Three";
    if(t.timer == sensorTimer.timer) sender = "Sensor";
//    newColours(sender, stone);
    stone.originSystem(sender);
    
    //turn on the stone
    if(t.timer > timeSpacing * timing){
        if(sender == "Environment One") stone.env1 = true;
        if(sender == "Environment Two") stone.env2 = true;
        if(sender == "Environment Three") stone.env3 = true;
        if(sender == "Sensor") stone.sens = true;
//        stone.active = true;
//        if(t.timer == sensorTimer.timer) stone.sensor = true;
//        else stone.sensor = false;
    }
    //turn off the stone
    if(t.timer > timeSpacing * timing + t.activeStoneTime){
        stone.releaseTimer.reset();
        stone.env1 = false;
        stone.env2 = false;
        stone.env3 = false;
        stone.sens = false;

    }
}

// The following three functions turns on and off an environment's impact state to complete the visual sequence
void ofApp::triggerEnviro1(Timer& t, int timing){
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        environmentOne.active = true;
    }
    //turn off the environment impact
    if  (t.timer > (timeSpacing * timing) + t.activeStoneTime){
        environmentOne.active = false;
    }
    
}

void ofApp::triggerEnviro2(Timer& t, int timing){
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        environmentTwo.active = true;
    }
    //turn off the environment impact
    if (t.timer > timeSpacing * timing + t.activeStoneTime){
        environmentTwo.active = false;
    }
    if (t.timer == timeSpacing * timing){
        environmentTwo.enviro.randomVals = true;
    }
}

void ofApp::triggerEnviro3(Timer& t, int timing){
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        environmentThree.active = true;
    }
    //turn off the environment impact
    if (t.timer > timeSpacing * timing + t.activeStoneTime){
        environmentThree.active = false;
    }
}


// instructions for the illumination of particular stepping stones in order and in a timed sequence

void ofApp::E1_to_E2(Timer& t, int variation){

    if(variation == 0){
        triggerStone(t, med_stones_5_8.stones[2], 0);
        triggerStone(t,large_stones.stones[2], 1);
        triggerStone(t, med_stones_5_8.stones[3], 2);
        triggerStone(t, large_stones.stones[3], 3);
        triggerStone(t, small_stones_13_16.stones[2], 4);
        triggerEnviro2(t, 5);
        sequenceComplete(t, 6);

    }
    if(variation == 1){
        triggerStone(t, small_stones_9_12.stones[1], 0);
        triggerStone(t, small_stones_9_12.stones[2], 1);
        triggerStone(t, small_stones_13_16.stones[0], 2);
        triggerStone(t, large_stones.stones[3], 3);
        triggerStone(t, small_stones_13_16.stones[2], 4);
        triggerEnviro2(t, 5);
        sequenceComplete(t, 6);
    }
    if(variation == 2){
        triggerStone(t, small_stones_5_8.stones[3], 0);
        triggerStone(t, small_stones_9_12.stones[0], 1);
        triggerStone(t, med_stones_5_8.stones[0], 2);
        triggerStone(t, med_stones_5_8.stones[1], 3);
        triggerStone(t, large_stones.stones[2], 4);
        triggerStone(t, small_stones_5_8.stones[2], 5);
        triggerEnviro2(t, 6);
        sequenceComplete(t, 7);
    }
}

void ofApp::E1_to_E3(Timer& t, int variation){

    if(variation == 0){
        triggerStone(t, small_stones_5_8.stones[3], 0);
        triggerStone(t, small_stones_9_12.stones[0], 1);
        triggerStone(t, med_stones_5_8.stones[1], 2);
        triggerStone(t, med_stones_5_8.stones[0], 3);
        triggerStone(t, large_stones.stones[1], 4);
        triggerStone(t, med_stones_1_4.stones[3], 5);
        triggerStone(t, med_stones_1_4.stones[2], 6);
        triggerStone(t, small_stones_5_8.stones[1], 7);
        triggerEnviro3(t, 8);
        sequenceComplete(t, 9);
    }
    if(variation == 1){
        triggerStone(t, med_stones_5_8.stones[2], 0);
        triggerStone(t, large_stones.stones[2], 1);
        triggerStone(t, small_stones_5_8.stones[2], 2);
        triggerStone(t, med_stones_5_8.stones[1], 3);
        triggerStone(t, med_stones_5_8.stones[0], 4);
        triggerStone(t, large_stones.stones[1], 5);
        triggerStone(t, med_stones_1_4.stones[3], 6);
        triggerStone(t, small_stones_5_8.stones[1], 7);
        triggerEnviro3(t, 8);
        sequenceComplete(t, 9);
    }
    if(variation == 2){
        triggerStone(t, small_stones_9_12.stones[0], 0);
        triggerStone(t, med_stones_5_8.stones[1], 1);
        triggerStone(t, med_stones_5_8.stones[0], 2);
        triggerStone(t, large_stones.stones[1], 3);
        triggerStone(t, med_stones_1_4.stones[3], 4);
        triggerStone(t, small_stones_5_8.stones[1], 5);
        triggerEnviro3(t, 6);
        sequenceComplete(t, 7);
    }

}

void ofApp::E2_to_E1(Timer& t, int variation){

    if(variation == 0){
        triggerStone(t, small_stones_13_16.stones[2], 0);
        triggerStone(t, large_stones.stones[3], 1);
        triggerStone(t, med_stones_5_8.stones[3], 2);
        triggerStone(t, small_stones_13_16.stones[0], 3);
        triggerStone(t, small_stones_9_12.stones[2], 4);
        triggerStone(t, small_stones_9_12.stones[1], 5);
        triggerEnviro1(t, 6);
        sequenceComplete(t, 7);

    }
    if(variation == 1){

        triggerStone(t, small_stones_5_8.stones[2], 0);
        triggerStone(t, med_stones_5_8.stones[1], 1);
        triggerStone(t, med_stones_5_8.stones[0], 2);
        triggerStone(t, small_stones_5_8.stones[3], 3);
        triggerStone(t, small_stones_9_12.stones[0], 4);
        triggerStone(t, med_stones_5_8.stones[2], 5);
        triggerEnviro1(t, 6);
        sequenceComplete(t, 7);
    }
    if(variation == 2){
        triggerStone(t, small_stones_5_8.stones[2], 0);
        triggerStone(t, med_stones_5_8.stones[1], 1);
        triggerStone(t, med_stones_5_8.stones[0], 2);
        triggerStone(t, small_stones_5_8.stones[3], 3);
        triggerStone(t, small_stones_9_12.stones[0], 4);
        triggerStone(t, med_stones_5_8.stones[2], 5);
        triggerEnviro1(t, 6);
        sequenceComplete(t, 7);
    }
}
void ofApp::E2_to_E3(Timer& t, int variation){

    if(variation == 0){
        triggerStone(t, small_stones_1_4.stones[3], 0);
        triggerStone(t, small_stones_5_8.stones[0], 1);
        triggerStone(t, small_stones_1_4.stones[2], 2);
        triggerStone(t, med_stones_1_4.stones[0], 3);
        triggerStone(t, large_stones.stones[0], 4);
        triggerStone(t, small_stones_1_4.stones[1], 5);
        triggerStone(t, small_stones_1_4.stones[0], 6);
        triggerEnviro3(t, 7);
        sequenceComplete(t, 8);
    }
    if(variation == 1){
        triggerStone(t, small_stones_5_8.stones[2], 0);
        triggerStone(t, med_stones_5_8.stones[1], 1);
        triggerStone(t, med_stones_5_8.stones[0], 2);
        triggerStone(t, large_stones.stones[1], 3);
        triggerStone(t, med_stones_1_4.stones[3], 4);
        triggerStone(t, med_stones_1_4.stones[2], 5);
        triggerStone(t, small_stones_5_8.stones[1], 6);
        triggerEnviro3(t, 7);
        sequenceComplete(t, 8);

    }
    if(variation == 2){

        triggerStone(t, small_stones_1_4.stones[3], 0);
        triggerStone(t, small_stones_5_8.stones[0], 1);
        triggerStone(t, med_stones_1_4.stones[0], 2);
        triggerStone(t, small_stones_1_4.stones[2], 3);
        triggerStone(t, med_stones_1_4.stones[1], 4);
        triggerEnviro3(t, 5);
        sequenceComplete(t, 6);
    }
}

void ofApp::E3_to_E1(Timer& t, int variation){
    
    if(variation == 0){
       triggerStone(t, small_stones_5_8.stones[1], 0);
       triggerStone(t, med_stones_1_4.stones[2], 1);
       triggerStone(t, large_stones.stones[1], 2);
       triggerStone(t, med_stones_5_8.stones[0], 3);
       triggerStone(t, small_stones_5_8.stones[3], 4);
       triggerEnviro1(t, 5);
       sequenceComplete(t, 6);


    }
    if(variation == 1){
        triggerStone(t, small_stones_5_8.stones[1], 0);
        triggerStone(t, med_stones_1_4.stones[2], 1);
        triggerStone(t, large_stones.stones[1], 2);
        triggerStone(t, med_stones_5_8.stones[0], 3);
        triggerStone(t, med_stones_5_8.stones[1], 4);
        triggerStone(t, small_stones_9_12.stones[0], 5);
        triggerEnviro1(t, 6);
        sequenceComplete(t, 7);
    }
    if(variation == 2){
        
        triggerStone(t, small_stones_5_8.stones[1], 0);
        triggerStone(t, med_stones_1_4.stones[3], 1);
        triggerStone(t, large_stones.stones[1], 2);
        triggerStone(t, med_stones_5_8.stones[0], 3);
        triggerStone(t, med_stones_5_8.stones[1], 4);
        triggerStone(t, large_stones.stones[2], 5);
        triggerStone(t, med_stones_5_8.stones[2], 6);
        triggerEnviro1(t, 7);
        sequenceComplete(t, 8);
    }
}

void ofApp::E3_to_E2(Timer& t, int variation){

    if(variation == 0){
        triggerStone(t, small_stones_1_4.stones[0], 0);
        triggerStone(t, small_stones_1_4.stones[1], 1);
        triggerStone(t, large_stones.stones[0], 2);
        triggerStone(t, med_stones_1_4.stones[0], 3);
        triggerStone(t, small_stones_5_8.stones[0], 4);
        triggerStone(t, small_stones_1_4.stones[2], 4);
        triggerStone(t, small_stones_1_4.stones[3], 5);
        triggerEnviro2(t, 6);
        sequenceComplete(t, 7);
    }

    if(variation == 1){
        triggerStone(t, small_stones_5_8.stones[1], 0);
        triggerStone(t, med_stones_1_4.stones[2], 1);
        triggerStone(t, med_stones_1_4.stones[3], 2);
        triggerStone(t, large_stones.stones[1], 3);
        triggerStone(t, med_stones_5_8.stones[0], 4);
        triggerStone(t, med_stones_5_8.stones[1], 5);
        triggerStone(t, small_stones_5_8.stones[2], 6);
        triggerEnviro2(t, 7);
        sequenceComplete(t, 8);

    }
    if(variation == 2){
        triggerStone(t, med_stones_1_4.stones[1], 0);
        triggerStone(t, small_stones_1_4.stones[2], 1);
        triggerStone(t, med_stones_1_4.stones[0], 2);
        triggerStone(t, small_stones_5_8.stones[0], 3);
        triggerStone(t, small_stones_1_4.stones[3], 4);
        triggerEnviro2(t, 5);
        sequenceComplete(t, 6);
    }
}


void ofApp::sensorToSystem(Timer& t, int variation){
    if(variation == 0){
        triggerStone(t, small_stones_13_16.stones[1], 0);
        triggerStone(t, small_stones_13_16.stones[0], 1);
        triggerStone(t, med_stones_5_8.stones[3], 2);
        triggerStone(t, large_stones.stones[3], 3);
        triggerStone(t, small_stones_13_16.stones[2], 4);
        triggerEnviro2(t, 5);
        sequenceComplete(t, 6);

    }
    if(variation == 1){
        triggerStone(t, small_stones_13_16.stones[1], 0);
        triggerStone(t, small_stones_13_16.stones[0], 1);
        triggerStone(t, small_stones_9_12.stones[2], 2);
        triggerStone(t, small_stones_9_12.stones[3], 3);
        triggerStone(t, small_stones_9_12.stones[1], 4);
        triggerEnviro1(t, 5);
        sequenceComplete(t, 6);
    }
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


