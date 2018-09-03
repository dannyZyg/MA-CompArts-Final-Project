#include "ofApp.h"

void ofApp::setup(){
    ofSetFullscreen(true);
    serialSetup();
    
    // Font used for environment/geometry labeling.
    verdana30.load("verdana.ttf", 80, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    ofSetCircleResolution(100);
	ofBackground(0);
    debug = false;
    printInfo = false;

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
    
    // setup piMapper
	piMapper.setup();

    timeSpacing = 1000;
    sensorTrigger = false;
    sensorSequenceActive = false;
    verifiedTrigger = false;
    
    // Setup the timers that will be used for communication sequences.
    env1Timer = Timer();
    env2Timer = Timer();
    env3Timer = Timer();
    sensorTimer = Timer();
    env1Timer.setup();
    env2Timer.setup();
    env3Timer.setup();
    sensorTimer.setup();
    
    sensorDestination = 2;
    
    // setup the three smoother objects. One to handle each of the environment's sensor onset.
    env1SmoothedSensor.smoother = new ParameterSmoother(5., ofGetFrameRate());
    env2SmoothedSensor.smoother = new ParameterSmoother(5., ofGetFrameRate());
    env3SmoothedSensor.smoother = new ParameterSmoother(5., ofGetFrameRate());
    
}

void ofApp::update(){
	piMapper.update();
    scheduler(); // call the function which handles all of the communication sequences
    serialUpdate(); // update serial with new values from the sensor
    
    // keep the timers running
    env1Timer.run();
    env2Timer.run();
    env3Timer.run();
    sensorTimer.run();
    
    proximitySensorToBlur(); //this function adds a slow onset/offset to the sensor's control of the blur effect
    
}

void ofApp::draw(){
 
    ofBackground(0);
    
    // draw the piMapper interface and surfaces
	piMapper.draw();
    
    if(printInfo){
        ofSetColor(255);
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 16, 16);
        ofDrawBitmapString("Sensor Val = " + ofToString(val), 16, 32);
    }
}

void ofApp::keyPressed(int key){
	piMapper.keyPressed(key);
    
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
    if(key == 'I') printInfo = !printInfo;
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


// This function takes the mapped sensor value and creates a smooht onset and offset when controlling each environment's Blur. When the destination system changes or the hand is removed from the sensor, the blur effect will fade out.
void ofApp::proximitySensorToBlur(){
    
    float mappedSensorVal = ofMap(val, 450, 0, 0, 10, true);
    
    env1SmoothedSensor.currentValue = env1SmoothedSensor.smoother -> process(env1SmoothedSensor.targetValue);
    environmentOne.setScale = env1SmoothedSensor.currentValue;
    
    env2SmoothedSensor.currentValue = env2SmoothedSensor.smoother -> process(env2SmoothedSensor.targetValue);
    environmentTwo.setScale = env2SmoothedSensor.currentValue;
    
    env3SmoothedSensor.currentValue = env3SmoothedSensor.smoother -> process(env3SmoothedSensor.targetValue);
    environmentThree.setScale = env3SmoothedSensor.currentValue;

    
    if(sensorSequenceActive){
        if(sensorDestination == 0){ // smoothing for Env1
            if(val < 800 && val > 0) {
                env1SmoothedSensor.targetValue = mappedSensorVal;
            }
            else{
                env1SmoothedSensor.targetValue = 0;
            }
        }
        if(sensorDestination == 1){
            if(val < 800 && val > 0) { // smoothing for Env2
                env2SmoothedSensor.targetValue = mappedSensorVal;
            }
            else{
                env2SmoothedSensor.targetValue = 0;
            }
        }
        if(sensorDestination == 2){
            if(val < 800 && val > 0) { // smoothing for Env3
                env3SmoothedSensor.targetValue = mappedSensorVal;
            }
            else{
                env3SmoothedSensor.targetValue = 0;
            }
        }
    }
    else{ // otherwise, fade to 0
        env1SmoothedSensor.targetValue = 0;
        env2SmoothedSensor.targetValue = 0;
        env3SmoothedSensor.targetValue = 0;
    }
}


// This function listens to signs for sequence activation and then runs the right sequence.
void ofApp::scheduler(){

//////////////SENSOR SEQUENCE ACTIVATION///////////////////////

    //'31' is a troublesome number with the sensor, and can sometimes send false triggers.
    if(val > 15 && val < 450 && val != 31) verifiedTrigger = true; // verified trigger with no unintended changes
    else verifiedTrigger = false;
    
    // prepare the sequence. Do once only.
    if(verifiedTrigger && !sensorSequenceActive){
        
        sensorDestination = ofRandom(3);
        if(sensorDestination == lastDestination){
            sensorDestination = ofRandom(3);
        }
        
        sensorSequenceActive = true;
        sensorTimer.reset();
     
        sensorPath = ofRandom(3);
        cout<<sensorDestination<<endl;
    }

    //run the sequence
    if(sensorSequenceActive){
        lastDestination = sensorDestination;
        sensorToSystem(sensorTimer, sensorDestination, sensorPath);
    }

    
    

////////////ENVIRONMENT SEQUENCE ACTIVATION//////////////////////
//
// Environment One
///// Conditions under which to allow a sequence to start
    if(environmentOne.enviro.trigger && !environmentOne.enviro.sequenceActive && !environmentOne.active && !sensorSequenceActive && !verifiedTrigger){
        // prepare the sequence.
        // if these conditions are met, do the following once only!
        environmentOne.enviro.randomPath = ofRandom(3); // choose random stone path
        environmentOne.enviro.destination = ofRandom(2); // choose random destination enviro
        environmentOne.enviro.sequenceActive = true; // condition to start the sequence
        env1Timer.reset(); // start a fresh timer
        env1Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
    }
    // Then display the sequence while the bool is true
    if(environmentOne.enviro.sequenceActive){
        if(environmentOne.enviro.destination == 0){
            E1_to_E2(env1Timer, environmentOne.enviro.randomPath);
        }
        if(environmentOne.enviro.destination == 1){
            E1_to_E3(env1Timer, environmentOne.enviro.randomPath);
        }
    }

//// Environment Two
    // Conditions under which to allow a sequence to start
    if(environmentTwo.enviro.trigger && !environmentTwo.enviro.sequenceActive && !environmentTwo.active && !sensorSequenceActive && !verifiedTrigger){
        // prepare the sequence.
        // if these conditions are met, do the following once only!
        environmentTwo.enviro.sequenceActive = true; // condition to start the sequence
        env2Timer.reset(); // start a fresh timer
        env2Timer.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
        environmentTwo.enviro.randomPath = ofRandom(3); // choose random stone path
        environmentTwo.enviro.destination = ofRandom(2);
    }
    // Then display the sequence while the bool is true
    if(environmentTwo.enviro.sequenceActive){
        if(environmentTwo.enviro.destination == 0){
            E2_to_E1(env2Timer, environmentTwo.enviro.randomPath);
        }
        if(environmentTwo.enviro.destination == 1){
            E2_to_E3(env2Timer, environmentTwo.enviro.randomPath);
        }
    }

//// Environment Three
    // Conditions under which to allow a sequence to start
    if(environmentThree.enviro.trigger && !environmentThree.enviro.sequenceActive && !environmentThree.active && !sensorSequenceActive && !verifiedTrigger){
        // prepare the sequence.
        // if these conditions are met, do the following once only!
        environmentThree.enviro.randomPath = ofRandom(3); // choose random stone path
        environmentThree.enviro.destination = ofRandom(2); // choose random destination enviro
        environmentThree.enviro.sequenceActive = true; // condition to start the sequence
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


// function which completes the stone sequence, setting the sequence state to inactive at the end, ready for another trigger.
void ofApp::sequenceComplete(string sender, Timer& t, int timing){
    if (t.timer > (timeSpacing * (timing + 6))){

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
    // the four bools correspond to which particles will fade in and out.
    
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
    // trigger the environment's new rules once impacted
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
    // trigger the environment's new rules once impacted
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
    // trigger the environment's new rules once impacted
    if (t.timer > timeSpacing * timing && t.timer < timeSpacing * timing + 100){
        if(sender == "Sensor") environmentThree.enviro.triggeredBySensor = true;
        environmentThree.enviro.setNewRules = true;
    }
}



//////////COMMUNICATION SEQUENCES///////////////////////
// instructions for the illumination of particular stepping stones in order and in a timed sequence. These are curated paths along the stepping stones.

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
        triggerStone(s, t, small_stones_9_12.stones[0], 2);
        triggerStone(s, t, med_stones_5_8.stones[0], 3);
        triggerStone(s, t, large_stones.stones[1], 4);
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
        triggerStone(s, t, med_stones_5_8.stones[1], 3);
        triggerStone(s, t, small_stones_9_12.stones[0], 4);
        triggerEnviro1(s, t, 5);
        sequenceComplete(s, t, 6);
    }
    if(variation == 2){

        triggerStone(s, t, small_stones_5_8.stones[1], 0);
        triggerStone(s, t, med_stones_1_4.stones[3], 1);
        triggerStone(s, t, med_stones_5_8.stones[1], 2);
        triggerStone(s, t, small_stones_9_12.stones[0], 3);
        triggerStone(s, t, med_stones_5_8.stones[2], 4);
        triggerStone(s, t, large_stones.stones[2], 5);
        triggerStone(s, t, med_stones_5_8.stones[3], 6);
        triggerStone(s, t, small_stones_9_12.stones[1], 7);
        triggerEnviro1(s, t, 8);
        sequenceComplete(s, t, 9);
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
        triggerStone(s, t, large_stones.stones[1], 2);
        triggerStone(s, t, med_stones_5_8.stones[1], 3);
        triggerStone(s, t, small_stones_5_8.stones[2], 4);
        triggerEnviro2(s, t, 5);
        sequenceComplete(s, t, 6);

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


void ofApp::sensorToSystem(Timer& t, int destination, int variation){
    string s = "Sensor";
    
// Communication with environment one
    if(destination == 0){
        if(variation == 0){
            triggerStone(s, t, small_stones_13_16.stones[1], 0);
            triggerStone(s, t, small_stones_13_16.stones[0], 1);
            triggerStone(s, t, small_stones_9_12.stones[2], 2);
            triggerStone(s, t, med_stones_5_8.stones[3], 3);
            triggerStone(s, t, small_stones_9_12.stones[1], 4);
            triggerEnviro1(s, t, 5);
            sequenceComplete(s, t, 6);
        }
        if(variation == 1){
            triggerStone(s, t, small_stones_13_16.stones[1], 0);
            triggerStone(s, t, large_stones.stones[3], 1);
            triggerStone(s, t, small_stones_13_16.stones[2], 2);
            triggerStone(s, t, med_stones_5_8.stones[3], 3);
            triggerStone(s, t, large_stones.stones[2], 4);
            triggerStone(s, t, med_stones_5_8.stones[2], 5);
            triggerStone(s, t, small_stones_9_12.stones[0], 6);
            triggerStone(s, t, small_stones_5_8.stones[3], 7);
            triggerEnviro1(s, t, 8);
            sequenceComplete(s, t, 9);
        }
        if(variation == 2){
            triggerStone(s, t, small_stones_13_16.stones[1], 0);
            triggerStone(s, t, large_stones.stones[3], 1);
            triggerStone(s, t, small_stones_13_16.stones[2], 2);
            triggerStone(s, t, large_stones.stones[2], 3);
            triggerStone(s, t, med_stones_5_8.stones[2], 4);
            triggerStone(s, t, small_stones_5_8.stones[2], 5);
            triggerStone(s, t, med_stones_5_8.stones[1], 6);
            triggerStone(s, t, large_stones.stones[1], 7);
            triggerStone(s, t, med_stones_5_8.stones[0], 8);
            triggerStone(s, t, small_stones_5_8.stones[3], 9);
            triggerEnviro1(s, t, 10);
            sequenceComplete(s, t, 11);
        }
    }
// Communication with environment two

    if(destination == 1){
        if(variation == 0){
            triggerStone(s, t, small_stones_13_16.stones[1], 0);
            triggerStone(s, t, small_stones_13_16.stones[0], 1);
            triggerStone(s, t, med_stones_5_8.stones[3], 2);
            triggerStone(s, t, large_stones.stones[2], 3);
            triggerStone(s, t, med_stones_5_8.stones[2], 4);
            triggerStone(s, t, small_stones_5_8.stones[2], 5);
            triggerEnviro2(s, t, 6);
            sequenceComplete(s, t, 7);
        }
        if(variation == 1){
            triggerStone(s, t, small_stones_9_12.stones[3], 0);
            triggerStone(s, t, small_stones_9_12.stones[2], 1);
            triggerStone(s, t, small_stones_13_16.stones[0], 2);
            triggerStone(s, t, small_stones_13_16.stones[1], 3);
            triggerStone(s, t, small_stones_13_16.stones[0], 4);
            triggerStone(s, t, large_stones.stones[3], 5);
            triggerStone(s, t, small_stones_13_16.stones[2], 6);
            triggerEnviro2(s, t, 7);
            sequenceComplete(s, t, 8);
        }
        
        if(variation == 2){
            triggerStone(s, t, small_stones_13_16.stones[1], 0);
            triggerStone(s, t, large_stones.stones[3], 1);
            triggerStone(s, t, small_stones_13_16.stones[2], 2);
            triggerStone(s, t, med_stones_5_8.stones[3], 3);
            triggerStone(s, t, large_stones.stones[2], 4);
            triggerStone(s, t, med_stones_5_8.stones[2], 5);
            triggerStone(s, t, small_stones_5_8.stones[2], 6);
            triggerStone(s, t, med_stones_1_4.stones[3], 7);
            triggerStone(s, t, med_stones_1_4.stones[1], 8);
            triggerStone(s, t, small_stones_1_4.stones[2], 9);
            triggerStone(s, t, small_stones_1_4.stones[3], 10);
            triggerEnviro2(s, t, 11);
            sequenceComplete(s, t, 12);
        }
    }
    

// Communication with environment three
    
    if(destination == 2){
        if(variation == 0){
            triggerStone(s, t, small_stones_9_12.stones[3], 0);
            triggerStone(s, t, small_stones_9_12.stones[2], 1);
            triggerStone(s, t, med_stones_5_8.stones[3], 2);
            triggerStone(s, t, large_stones.stones[2], 3);
            triggerStone(s, t, med_stones_5_8.stones[2], 4);
            triggerStone(s, t, small_stones_9_12.stones[0], 5);
            triggerStone(s, t, med_stones_5_8.stones[0], 6);
            triggerStone(s, t, large_stones.stones[1], 7);
            triggerStone(s, t, med_stones_1_4.stones[2], 8);
            triggerEnviro3(s, t, 9);
            sequenceComplete(s, t, 10);
            
        }
        if(variation == 1){
            triggerStone(s, t, small_stones_13_16.stones[1], 0);
            triggerStone(s, t, small_stones_13_16.stones[0], 1);
            triggerStone(s, t, large_stones.stones[3], 2);
            triggerStone(s, t, small_stones_13_16.stones[2], 3);
            triggerStone(s, t, med_stones_5_8.stones[3], 4);
            triggerStone(s, t, large_stones.stones[2], 5);
            triggerStone(s, t, med_stones_5_8.stones[2], 6);
            triggerStone(s, t, small_stones_5_8.stones[2], 7);
            triggerStone(s, t, med_stones_5_8.stones[1], 8);
            triggerStone(s, t, med_stones_1_4.stones[3], 9);
            triggerStone(s, t, small_stones_5_8.stones[1], 10);
            triggerEnviro3(s, t, 11);
            sequenceComplete(s, t, 12);
        }
        if(variation == 2){
            triggerStone(s, t, small_stones_13_16.stones[0], 0);
            triggerStone(s, t, large_stones.stones[3], 1);
            triggerStone(s, t, small_stones_13_16.stones[2], 2);
            triggerStone(s, t, med_stones_5_8.stones[3], 3);
            triggerStone(s, t, large_stones.stones[2], 4);
            triggerStone(s, t, med_stones_5_8.stones[2], 5);
            triggerStone(s, t, med_stones_5_8.stones[1], 6);
            triggerStone(s, t, med_stones_1_4.stones[3], 7);
            triggerStone(s, t, med_stones_1_4.stones[1], 8);
            triggerStone(s, t, small_stones_1_4.stones[2], 9);
            triggerStone(s, t, small_stones_1_4.stones[3], 10);
            triggerStone(s, t, small_stones_5_8.stones[0], 11);
            triggerStone(s, t, large_stones.stones[0], 12);
            triggerStone(s, t, small_stones_1_4.stones[1], 13);
            triggerStone(s, t, small_stones_1_4.stones[0], 14);
            triggerEnviro3(s, t, 15);
            sequenceComplete(s, t, 16);
        }
    }
}

///////SERIAL COMMUNICATION WITH ARDUINO///////////

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
//            incomingTrigger = bytesReturned[2];
            
            bSendSerialMessage = false;                         // get ready to wait a few frames before asking again
        }
    }
    countCycles++;
    if(countCycles == 1){
        bSendSerialMessage = true;                              // send a message to the arduino to tell it oF is ready for more readings
        countCycles = 0;
    }
}


