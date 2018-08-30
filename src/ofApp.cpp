#include "ofApp.h"

void ofApp::setup(){
    ofSetFullscreen(true);
    
    
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
        
    env1Timer = Timer();
    env2Timer = Timer();
    env3Timer = Timer();
    sensorTimer = Timer();
    
    stoneTimer = Timer();
    stoneTimer.setup();
    
    env1Timer.setup();
    env2Timer.setup();
    env3Timer.setup();
    sensorTimer.setup();
    
    
    sensorPath = ofRandom(6);
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
    stoneTimer.run();
    
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
    ofDrawBitmapString("Incoming Trigger = " + ofToString(incomingTrigger), 32, 40);

    
    
    
    ofSetColor(255);
    
}



void ofApp::keyPressed(int key){
	piMapper.keyPressed(key);
    
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
    
 
    if(key == 'B') environmentTwo.blur1 = !environmentTwo.blur1;
    if(key == 'N') environmentTwo.blur2 = !environmentTwo.blur2;
    if(key == 'A') environmentThree.active = !environmentThree.active;
    
    
    if(key == '!') environmentOne.enviro.randomVals = true;
    if(key == '@') environmentTwo.enviro.randomVals = true;
    if(key == '#') environmentThree.enviro.randomVals = true;
    
    if(key == 'P'){
        environmentOne.setScale = ofRandom(10);
        environmentOne.setRotation = ofRandom(-PI, PI);
        
    }

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
    environmentOne.enviro.randomVals = true;

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
    
//    if(incomingTrigger) sensorTrigger = true;
//    else(sensorTrigger = false);
//    cout << "se"
    if(incomingTrigger && !sensorSequenceActive){
        sensorTimer.reset();
        sensorSequenceActive = true;
        lastSensorPath = sensorPath;
        sensorPath = ofRandom(6);
        if(sensorPath == lastSensorPath){
            sensorPath = ofRandom(6);
        }
//        cout<< "sensorPath" << sensorPath<<endl;
    }

//    cout << "sens seq active " << sensorSequenceActive << endl;
    if(sensorSequenceActive){
        sensorToSystem(sensorTimer, sensorPath);
//        sensorToSystem(sensorTimer, 5);
        

    }

//    E1_to_E2(env2Timer, 2);
//    E1_to_E3(env2Timer, 2);
    E2_to_E1(env2Timer, 2);
//    E2_to_E3(env2Timer, 0);
//    E3_to_E1(env2Timer, 0);
//    E3_to_E2(env2Timer, 0);

    
//    cout << "e2 seq act " << environmentTwo.enviro.sequenceActive << endl;
    
    
//    cout<< "activate draw = " << small_stones_1_4.stones[0].activateDraw << endl;
//    cout << "release pct" << small_stones_1_4.stones[0].releaseTimer.pct << endl;
    
    
//    if(sensorTrigger && !environmentTwo.enviro.sequenceActive){
//        env2Timer.reset();
//        environmentTwo.enviro.sequenceActive = true;
//    }
//
//    if(environmentTwo.enviro.sequenceActive){
//        E2_to_E3(env2Timer, 1);
//    }
    
 
//    cout << "e2 timer" << env2Timer.timer << endl;
//    cout << "out trigger" << environmentTwo.enviro.trigger <<endl;
//    cout << "sys output" << environmentTwo.enviro.systemOutput << endl;
    
//    cout << "setNewRules = " << environmentTwo.enviro.setNewRules << endl;

    
//    cout << "e2 ran path" << environmentTwo.enviro.randomPath<< endl;
//    cout << "e2 ran destination " << environmentTwo.enviro.destination <<endl;
    
//
//    E1_to_E2(env1Timer, 0);
//
//    cout<< "sens trigger = " <<sensorTrigger << endl;
//
////////////ENVIRONMENT COMMUNICATION//
//
//// Environment One
    // Conditions under which to allow a sequence to start
//    if(environmentOne.enviro.trigger && !environmentOne.enviro.sequenceActive && !environmentOne.active){
//        // if these conditions are met, do the following once only!
//        environmentOne.enviro.randomPath = ofRandom(3); // choose random stone path
//        environmentOne.enviro.destination = ofRandom(2); // choose random destination enviro
//        environmentOne.enviro.sequenceActive = true; // condition to start the sequence
////        environmentOne.enviro.sequenceTrigger = false; // reset the trigger condition
//        env1Timer.reset(); // start a fresh timer
//        env1Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
//    }
//    // Then display the sequence while the bool is true
//    if(environmentOne.enviro.sequenceActive){
//        if(environmentOne.enviro.destination == 0){
//            E1_to_E2(env1Timer, environmentOne.enviro.randomPath);
//        }
//        if(environmentOne.enviro.destination == 1){
//            E1_to_E3(env1Timer, environmentOne.enviro.randomPath);
//        }
//    }
////
////// Environment Two
//    // Conditions under which to allow a sequence to start
//    if(environmentTwo.enviro.trigger && !environmentTwo.enviro.sequenceActive && !environmentTwo.active){
//
//        // if these conditions are met, do the following once only!
//        environmentTwo.enviro.sequenceActive = true; // condition to start the sequence
//        env2Timer.reset(); // start a fresh timer
//        env2Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
//        environmentTwo.enviro.randomPath = ofRandom(3); // choose random stone path
//        environmentTwo.enviro.destination = ofRandom(2);
//    }
//    // Then display the sequence while the bool is true
//    if(environmentTwo.enviro.sequenceActive){
//        if(environmentTwo.enviro.destination == 0){
//            E2_to_E1(env2Timer, environmentTwo.enviro.randomPath);
//        }
//        if(environmentTwo.enviro.destination == 1){
//            E2_to_E3(env2Timer, environmentTwo.enviro.randomPath);
//        }
//    }
//////
////// Environment Three
//    // Conditions under which to allow a sequence to start
//    if(environmentThree.enviro.trigger && !environmentThree.enviro.sequenceActive && !environmentThree.active){
//        // if these conditions are met, do the following once only!
//        environmentThree.enviro.randomPath = ofRandom(3); // choose random stone path
//        environmentThree.enviro.destination = ofRandom(2); // choose random destination enviro
//        environmentThree.enviro.sequenceActive = true; // condition to start the sequence
////        environmentThree.enviro.sequenceTrigger = false; // reset the trigger condition
//        env3Timer.reset(); // start a fresh timer
//        env3Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
//    }
//    // Then display the sequence while the bool is true
//    if(environmentThree.enviro.sequenceActive){
//        if(environmentThree.enviro.destination == 0){
//            E3_to_E1(env3Timer, environmentThree.enviro.randomPath);
//        }
//        if(environmentThree.enviro.destination == 1){
//            E3_to_E2(env3Timer, environmentThree.enviro.randomPath);
//        }
//    }
    
    
}


// function which completes the stone sequence, setting the sequence state to inactive at the end.
void ofApp::sequenceComplete(string sender, Timer& t, int timing){
    if (t.timer > (timeSpacing * (timing + 8))){

        //check which timer/environment is being addressed and turn of the relative sequenceActive state
        if(sender == "Environment One"){
            environmentOne.enviro.sequenceActive = false;
        }
        if(sender == "Environment Two"){
            environmentTwo.enviro.sequenceActive = false;
        }
        if(sender == "Environment Three"){
            environmentThree.enviro.sequenceActive = false;
        }
        if(sender == "Sensor"){
            sensorSequenceActive = false;
        }
    }
}


// function which triggers a stone on and off at the right time in a given sequence
void ofApp::triggerStone(string sender, Timer& t, StoneSystem& stone, int timing){
    
    //turn on the stone
    if(t.timer > timeSpacing * timing){
        if(sender == "Environment One") stone.env1 = true;
        if(sender == "Environment Two") stone.env2 = true;
        if(sender == "Environment Three") stone.env3 = true;
        if(sender == "Sensor") stone.sens = true;
    }
    //turn off the stone
    if(t.timer  > timeSpacing * timing + 3000){
        stone.releaseTimer.reset();
        if(sender == "Environment One") stone.env1 = false;
        if(sender == "Environment Two") stone.env2 = false;
        if(sender == "Environment Three") stone.env3 = false;
        if(sender == "Sensor") stone.sens = false;
    }
}

// The following three functions turns on and off an environment's impact state to complete the visual sequence
void ofApp::triggerEnviro1(string sender, Timer& t, int timing){
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        environmentOne.active = true;
    }
    //turn off the environment impact
    if  (t.timer > (timeSpacing * timing) + t.activeStoneTime){
        environmentOne.active = false;
    }
    if (t.timer > timeSpacing * timing && t.timer < timeSpacing * timing + 100){
        if(sender == "Sensor") environmentOne.enviro.triggeredBySensor = true;
        environmentOne.enviro.setNewRules = true;
    }
    
}

void ofApp::triggerEnviro2(string sender, Timer& t, int timing){
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        environmentTwo.active = true;
    }
    //turn off the environment impact
    if (t.timer > timeSpacing * timing + 3000){
        environmentTwo.active = false;
    }
    if (t.timer > timeSpacing * timing && t.timer < timeSpacing * timing + 100){
        if(sender == "Sensor") environmentTwo.enviro.triggeredBySensor = true;
        environmentTwo.enviro.setNewRules = true;
    }
}

void ofApp::triggerEnviro3(string sender, Timer& t, int timing){
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        environmentThree.active = true;
    }
    //turn off the environment impact
    if (t.timer > timeSpacing * timing + t.activeStoneTime){
        environmentThree.active = false;
    }
    if (t.timer > timeSpacing * timing && t.timer < timeSpacing * timing + 100){
        if(sender == "Sensor") environmentThree.enviro.triggeredBySensor = true;
        environmentThree.enviro.setNewRules = true;
    }
}


// instructions for the illumination of particular stepping stones in order and in a timed sequence

void ofApp::E1_to_E2(Timer& t, int variation){
    string s = "Environment One";
    
    if(variation == 0){
        triggerStone(s, t, small_stones_9_12.stones[0], 0);
        triggerStone(s, t, med_stones_5_8.stones[2], 1);
        triggerStone(s, t, large_stones.stones[2], 2);
        triggerStone(s, t,  med_stones_5_8.stones[3], 3);
        triggerStone(s, t, small_stones_13_16.stones[2], 4);
        triggerStone(s, t, large_stones.stones[3], 5);
        triggerEnviro2(s, t, 6);
        sequenceComplete(s, t, 7);

    }
    if(variation == 1){
        triggerStone(s, t, small_stones_9_12.stones[1], 0);
        triggerStone(s, t, small_stones_9_12.stones[2], 1);
        triggerStone(s, t, small_stones_13_16.stones[0], 2);
        triggerStone(s, t, large_stones.stones[3], 3);
        triggerStone(s, t, small_stones_13_16.stones[2], 4);
        triggerEnviro2(s, t, 5);
        sequenceComplete(s, t, 6);
    }
    if(variation == 2){
       
        triggerStone(s, t, small_stones_5_8.stones[3], 0);
        triggerStone(s, t, med_stones_5_8.stones[0], 1);
        triggerStone(s, t, small_stones_9_12.stones[0], 2);
        triggerStone(s, t, med_stones_5_8.stones[1], 3);
        triggerStone(s, t, med_stones_5_8.stones[2], 4);
        triggerStone(s, t, small_stones_5_8.stones[2], 5);
        triggerEnviro2(s, t, 6);
        sequenceComplete(s, t, 7);
    }
}

void ofApp::E1_to_E3(Timer& t, int variation){

    string s = "Environment One";
    
    if(variation == 0){
        triggerStone(s, t, small_stones_5_8.stones[3], 0);
        triggerStone(s, t, small_stones_9_12.stones[0], 1);
        triggerStone(s, t, med_stones_5_8.stones[1], 2);
        triggerStone(s, t, large_stones.stones[1], 3);
        triggerStone(s, t, med_stones_1_4.stones[2], 4);
        triggerStone(s, t, small_stones_5_8.stones[1], 5);
        triggerStone(s, t, med_stones_1_4.stones[3], 6);
        triggerStone(s, t, med_stones_1_4.stones[1], 7);
        triggerEnviro3(s, t, 8);
        sequenceComplete(s, t, 9);
    }
    if(variation == 1){
        triggerStone(s, t, large_stones.stones[2], 0);
        triggerStone(s, t, med_stones_5_8.stones[2], 1);
        triggerStone(s, t, small_stones_5_8.stones[2], 2);
        triggerStone(s, t, med_stones_5_8.stones[1], 3);
        triggerStone(s, t, small_stones_9_12.stones[0], 4);
        triggerStone(s, t, med_stones_5_8.stones[0], 5);
        triggerStone(s, t, large_stones.stones[1], 6);
        triggerStone(s, t, med_stones_1_4.stones[2], 7);
        triggerStone(s, t, small_stones_5_8.stones[1], 8);
        triggerEnviro3(s, t, 9);
        sequenceComplete(s, t, 10);
    }
    if(variation == 2){
        triggerStone(s, t, small_stones_5_8.stones[3], 0);
        triggerStone(s, t, med_stones_5_8.stones[0], 1);
        triggerStone(s, t, large_stones.stones[1], 2);
        triggerStone(s, t, med_stones_1_4.stones[2], 3);
        triggerStone(s, t, small_stones_5_8.stones[1], 4);
        triggerEnviro3(s, t, 5);
        sequenceComplete(s, t, 6);
    }

}

void ofApp::E2_to_E1(Timer& t, int variation){
    string s = "Environment Two";

    if(variation == 0){
        triggerStone(s, t, small_stones_13_16.stones[2], 0);
        triggerStone(s, t, large_stones.stones[3], 1);
        triggerStone(s, t, small_stones_13_16.stones[1], 2);
        triggerStone(s, t, small_stones_13_16.stones[0], 3);
        triggerStone(s, t, small_stones_9_12.stones[2], 4);
        triggerStone(s, t, small_stones_9_12.stones[1], 5);
        triggerEnviro1(s, t, 6);
        sequenceComplete(s, t, 7);

    }
    if(variation == 1){

        triggerStone(s, t, small_stones_5_8.stones[2], 0);
        triggerStone(s, t, med_stones_5_8.stones[1], 1);
        triggerStone(s, t, large_stones.stones[1], 2);
        triggerStone(s, t, med_stones_5_8.stones[0], 3);
        triggerStone(s, t, small_stones_5_8.stones[3], 4);
        triggerStone(s, t, small_stones_9_12.stones[0], 5);
        triggerStone(s, t, med_stones_5_8.stones[2], 6);
        triggerEnviro1(s, t, 7);
        sequenceComplete(s, t, 8);
    }
    if(variation == 2){
        triggerStone(s, t, small_stones_5_8.stones[2], 0);
        triggerStone(s, t, med_stones_5_8.stones[1], 1);
        triggerStone(s, t, med_stones_5_8.stones[0], 2);
        triggerStone(s, t, small_stones_5_8.stones[3], 3);
        triggerStone(s, t, small_stones_9_12.stones[0], 4);
        triggerStone(s, t, med_stones_5_8.stones[2], 5);
        triggerEnviro1(s, t, 6);
        sequenceComplete(s, t, 7);
    }
}
void ofApp::E2_to_E3(Timer& t, int variation){
    string s = "Environment Two";

    if(variation == 0){
        triggerStone(s, t, small_stones_1_4.stones[3], 0);
        triggerStone(s, t, small_stones_5_8.stones[0], 1);
        triggerStone(s, t, small_stones_1_4.stones[2], 2);
        triggerStone(s, t, med_stones_1_4.stones[0], 3);
        triggerStone(s, t, large_stones.stones[0], 4);
        triggerStone(s, t, small_stones_1_4.stones[1], 5);
        triggerStone(s, t, small_stones_1_4.stones[0], 6);
        triggerEnviro3(s, t, 7);
        sequenceComplete(s, t, 8);
    }
    if(variation == 1){
        triggerStone(s, t, small_stones_5_8.stones[2], 0);
        triggerStone(s, t, med_stones_5_8.stones[1], 1);
        triggerStone(s, t, med_stones_5_8.stones[0], 2);
        triggerStone(s, t, large_stones.stones[1], 3);
        triggerStone(s, t, med_stones_1_4.stones[3], 4);
        triggerStone(s, t, med_stones_1_4.stones[2], 5);
        triggerStone(s, t, small_stones_5_8.stones[1], 6);
        triggerEnviro3(s, t, 7);
        sequenceComplete(s, t, 8);

    }
    if(variation == 2){

        triggerStone(s, t, small_stones_1_4.stones[3], 0);
        triggerStone(s, t, small_stones_5_8.stones[0], 1);
        triggerStone(s, t, med_stones_1_4.stones[0], 2);
        triggerStone(s, t, small_stones_1_4.stones[2], 3);
        triggerStone(s, t, med_stones_1_4.stones[1], 4);
        triggerEnviro3(s, t, 5);
        sequenceComplete(s, t, 6);
    }
}

void ofApp::E3_to_E1(Timer& t, int variation){
    string s = "Environment Three";

    if(variation == 0){
       triggerStone(s, t, small_stones_5_8.stones[1], 0);
       triggerStone(s, t, med_stones_1_4.stones[2], 1);
       triggerStone(s, t, large_stones.stones[1], 2);
       triggerStone(s, t, med_stones_5_8.stones[0], 3);
       triggerStone(s, t, small_stones_5_8.stones[3], 4);
       triggerEnviro1(s, t, 5);
       sequenceComplete(s, t, 6);


    }
    if(variation == 1){
        triggerStone(s, t, small_stones_5_8.stones[1], 0);
        triggerStone(s, t, med_stones_1_4.stones[2], 1);
        triggerStone(s, t, large_stones.stones[1], 2);
        triggerStone(s, t, med_stones_5_8.stones[0], 3);
        triggerStone(s, t, med_stones_5_8.stones[1], 4);
        triggerStone(s, t, small_stones_9_12.stones[0], 5);
        triggerEnviro1(s, t, 6);
        sequenceComplete(s, t, 7);
    }
    if(variation == 2){

        triggerStone(s, t, small_stones_5_8.stones[1], 0);
        triggerStone(s, t, med_stones_1_4.stones[3], 1);
        triggerStone(s, t, large_stones.stones[1], 2);
        triggerStone(s, t, med_stones_5_8.stones[0], 3);
        triggerStone(s, t, med_stones_5_8.stones[1], 4);
        triggerStone(s, t, large_stones.stones[2], 5);
        triggerStone(s, t, med_stones_5_8.stones[2], 6);
        triggerEnviro1(s, t, 7);
        sequenceComplete(s, t, 8);
    }
}

void ofApp::E3_to_E2(Timer& t, int variation){
    string s = "Environment Three";

    if(variation == 0){
        triggerStone(s, t, small_stones_1_4.stones[0], 0);
        triggerStone(s, t, small_stones_1_4.stones[1], 1);
        triggerStone(s, t, large_stones.stones[0], 2);
        triggerStone(s, t, med_stones_1_4.stones[0], 3);
        triggerStone(s, t, small_stones_5_8.stones[0], 4);
        triggerStone(s, t, small_stones_1_4.stones[2], 4);
        triggerStone(s, t, small_stones_1_4.stones[3], 5);
        triggerEnviro2(s, t, 6);
        sequenceComplete(s, t, 7);
    }

    if(variation == 1){
        triggerStone(s, t, small_stones_5_8.stones[1], 0);
        triggerStone(s, t, med_stones_1_4.stones[2], 1);
        triggerStone(s, t, med_stones_1_4.stones[3], 2);
        triggerStone(s, t, large_stones.stones[1], 3);
        triggerStone(s, t, med_stones_5_8.stones[0], 4);
        triggerStone(s, t, med_stones_5_8.stones[1], 5);
        triggerStone(s, t, small_stones_5_8.stones[2], 6);
        triggerEnviro2(s, t, 7);
        sequenceComplete(s, t, 8);

    }
    if(variation == 2){
        triggerStone(s, t, med_stones_1_4.stones[1], 0);
        triggerStone(s, t, small_stones_1_4.stones[2], 1);
        triggerStone(s, t, med_stones_1_4.stones[0], 2);
        triggerStone(s, t, small_stones_5_8.stones[0], 3);
        triggerStone(s, t, small_stones_1_4.stones[3], 4);
        triggerEnviro2(s, t, 5);
        sequenceComplete(s, t, 6);
    }
}


void ofApp::sensorToSystem(Timer& t, int variation){
    string s = "Sensor";
    
// Communication with environment one
    if(variation == 0){
        triggerStone(s, t, small_stones_13_16.stones[1], 0);
        triggerStone(s, t, small_stones_13_16.stones[0], 1);
        triggerStone(s, t, small_stones_9_12.stones[2], 2);
        triggerStone(s, t, small_stones_9_12.stones[3], 3);
        triggerStone(s, t, small_stones_9_12.stones[1], 4);
        triggerEnviro1(s, t, 5);
        sequenceComplete(s, t, 6);
    }

    if(variation == 1){
        triggerStone(s, t, small_stones_13_16.stones[1], 0);
        triggerStone(s, t, small_stones_13_16.stones[0], 1);
        triggerStone(s, t, med_stones_5_8.stones[3], 2);
        triggerStone(s, t, large_stones.stones[2], 3);
        triggerStone(s, t, med_stones_5_8.stones[1], 4);
        triggerStone(s, t, small_stones_5_8.stones[2], 5);
        triggerEnviro1(s, t, 6);
        sequenceComplete(s, t, 7);
    }

// Communication with environment two
    if(variation == 2){
        triggerStone(s, t, small_stones_13_16.stones[1], 0);
        triggerStone(s, t, small_stones_13_16.stones[0], 1);
        triggerStone(s, t, med_stones_5_8.stones[3], 2);
        triggerStone(s, t, large_stones.stones[3], 3);
        triggerStone(s, t, small_stones_13_16.stones[2], 4);
        triggerEnviro2(s, t, 5);
        sequenceComplete(s, t, 6);
    }

    if(variation == 3){
        triggerStone(s, t, small_stones_9_12.stones[3], 0);
        triggerStone(s, t, small_stones_9_12.stones[2], 1);
        triggerStone(s, t, small_stones_13_16.stones[0], 2);
        triggerStone(s, t, med_stones_5_8.stones[3], 3);
        triggerStone(s, t, large_stones.stones[2], 4);
        triggerStone(s, t, med_stones_5_8.stones[1], 5);
        triggerStone(s, t, small_stones_5_8.stones[2], 6);
        triggerEnviro2(s, t, 7);
        sequenceComplete(s, t, 8);
    }

// Communication with environment three
    if(variation == 4){
        triggerStone(s, t, small_stones_9_12.stones[3], 0);
        triggerStone(s, t, small_stones_9_12.stones[2], 1);
        triggerStone(s, t, small_stones_13_16.stones[0], 2);
        triggerStone(s, t, med_stones_5_8.stones[3], 3);
        triggerStone(s, t, large_stones.stones[2], 4);
        triggerStone(s, t, med_stones_5_8.stones[1], 6);
        triggerStone(s, t, med_stones_5_8.stones[0], 7);
        triggerStone(s, t, large_stones.stones[1], 8);
        triggerStone(s, t, med_stones_1_4.stones[3], 9);
        triggerStone(s, t, med_stones_1_4.stones[2], 10);
        triggerStone(s, t, small_stones_5_8.stones[1], 11);
        triggerEnviro3(s, t, 12);
        sequenceComplete(s, t, 13);

    }

    if(variation == 5){
        triggerStone(s, t, small_stones_13_16.stones[1], 0);
        triggerStone(s, t, small_stones_13_16.stones[0], 1);
        triggerStone(s, t, med_stones_5_8.stones[3], 2);
        triggerStone(s, t, large_stones.stones[2], 3);
        triggerStone(s, t, med_stones_5_8.stones[2], 4);
        triggerStone(s, t, small_stones_9_12.stones[0], 5);
        triggerStone(s, t, small_stones_5_8.stones[3], 6);
        triggerStone(s, t, med_stones_5_8.stones[0], 7);
        triggerStone(s, t, large_stones.stones[1], 8);
        triggerStone(s, t, med_stones_1_4.stones[3], 9);
        triggerStone(s, t, med_stones_1_4.stones[2], 10);
        triggerStone(s, t, small_stones_5_8.stones[1], 11);
        triggerEnviro3(s, t, 12);
        sequenceComplete(s, t, 13);

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
            incomingTrigger = bytesReturned[2];
            
            bSendSerialMessage = false;                         // get ready to wait a few frames before asking again
        }
    }
    countCycles++;
    if(countCycles == 1){
        bSendSerialMessage = true;                              // send a message to the arduino to tell it oF is ready for more readings
        countCycles = 0;
    }
}


