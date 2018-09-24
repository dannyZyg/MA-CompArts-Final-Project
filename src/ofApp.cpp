//////////////////////////////////////////////////////////////////////////

    ////     //     ////////    //       //     //      //     ///////
    // //    //     //           //     //      //      //   ///
    //  //   //     ////////       // //        //      //   //
    //   //  //     //               //         //      //    ///////
    //    // //     //            //   //        //    //          ///
    //     ////     ////////    //       //       //////     ////////

//////////////////////////////////////////////////////////////////////////
// Danny Keig 2018
// MA Computational Arts Final Project
// Goldsmiths, University of London

// Nexus aims to explore emergence, ecologies and networks. It is a projection mapping work utilising ofxPiMapper to project onto real world geometry. It consists of three unique environments which have their own rule systems for their agents (particles). When certain conditions are met within an environment, they communicate with another environment and can impact the destination environment's rules.

// The conditions for communication are largely based on an environment's unique qualities, and are described in their relevant classes.

// When the systems communicate, they send a signal of smaller particle clusters in a timed sequence along the physical shapes, which I will call stepping stones, or in the case of the code, between the smaller 'stone sources'. Each stone source FBO contains four separate 'stone systems', handling four physical stones on the structure when mapped.

// The work is interactive, and connects with an arduino over serial. A proximity sensor detects human presence, and a hand motion can trigger a new sequence from the sensor location to a random envrionment. The regular communications between the three environments result in the destination receiving new rules in the form of a preset, however the interactive sensor sequences seed the environments with randomised values. While an interactive sequence is active, the receiving environment will have a blur applied to it which is controlled by the sensor value comming from the arduino. All other sequences will end and remain inactive until the interaction has ceased.

// The code is built around a collection of FBO sources which are mapped to the physical shapes. Every physical shape has a unique instance of a particle system. There is a base particle class, a base particle system class, and derived particle system classes.

// The flow of data is as such: Particle --> Particle System (or derived class) --> FBO source --> ofApp
// Each of the three environments exists as its own derived particle system class. Each of the stones is a derived 'stone system' class.

// Thanks to Krisjanis Rijnieks for developing ofxPiMapper, and to Kyle McDonald for sharing his binned particle system with the openFrameworks community.

// https://ofxpimapper.com/
// https://github.com/kylemcdonald/openFrameworksDemos/tree/master/BinnedParticleSystem/src

#include "ofApp.h"

void ofApp::setup(){
//    ofSetFullscreen(true);
    serialSetup();
    
    // Font used for environment/geometry labeling.
    verdana30.load("verdana.ttf", 80, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    ofSetCircleResolution(100);
	ofBackground(0);
    debug = false;
    printInfo = false;

	// Register our piMapper sources.
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
    
    
    // dummy stone system to fill the position 0 in the vector. This means the real stones are numbered from 1. I've done this so that the number 0 in the paths array can be ignored and treated like a blank slot.
    StoneSystem* s;
    stoneSystems.push_back(s);
    
    // attain the address of all of the stone systems from each of the sources and load into one vector
    loadStonesFromSource(small_stones_1_4);
    loadStonesFromSource(small_stones_5_8);
    loadStonesFromSource(small_stones_9_12);
    loadStonesFromSource(small_stones_13_16);
    loadStonesFromSource(med_stones_1_4);
    loadStonesFromSource(med_stones_5_8);
    loadStonesFromSource(large_stones);
    
}


// function which collects all of the stone systems from each of the sources and loads them into a single vector housing pointers to all of them
void ofApp::loadStonesFromSource(StoneSource& source){
    for(int i = 0; i < source.stones.size(); i ++){
        StoneSystem* s = &source.stones[i];
        stoneSystems.push_back(s);
    }
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


// This function takes the mapped sensor value and creates a smooth onset and offset when controlling each environment's Blur. When the destination system changes or the hand is removed from the sensor, the blur effect will fade out.
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
    }

    //run the sequence
    if(sensorSequenceActive){
        lastDestination = sensorDestination;
        sensorSequence("Sensor", sensorDestination, sensorPath, sensorTimer);
    }
    
    environmentSequenceActivation("Environment One", environmentOne.enviro, env1Timer);
    environmentSequenceActivation("Environment Two", environmentTwo.enviro, env2Timer);
    environmentSequenceActivation("Environment Three", environmentThree.enviro, env3Timer);

}

void ofApp::environmentSequenceActivation(string enviroIdentifier, ParticleSystem& enviro, Timer& t){
    
    ///// Conditions under which to allow a sequence to start
    if(enviro.trigger && !enviro.sequenceActive && !sensorSequenceActive && !verifiedTrigger){
        // prepare the sequence.
        // if these conditions are met, do the following once only!
        enviro.randomPath = ofRandom(3); // choose random stone path
        enviro.destination = ofRandom(2); // choose random destination enviro
        
        enviro.sequenceActive = true; // condition to start the sequence
        t.reset(); // start a fresh timer
        t.activeStoneTime = ofRandom(1000, 5000); // chose how long the stones are active
    }
    // Then display the sequence while the bool is true
    if(enviro.sequenceActive){
        
        if(enviroIdentifier == "Environment One"){
            if(enviro.destination == 0) sequence(enviroIdentifier, environmentTwo.enviro, enviro.destination, enviro.randomPath, env1Timer);
            if(enviro.destination == 1) sequence(enviroIdentifier, environmentThree.enviro, enviro.destination, enviro.randomPath, env1Timer);
        }
        
        if(enviroIdentifier == "Environment Two"){
            if(enviro.destination == 0) sequence(enviroIdentifier, environmentOne.enviro, enviro.destination, enviro.randomPath, env2Timer);
            if(enviro.destination == 1) sequence(enviroIdentifier, environmentThree.enviro, enviro.destination, enviro.randomPath, env2Timer);
        }
        
        if(enviroIdentifier == "Environment Three"){
            if(enviro.destination == 0) sequence(enviroIdentifier, environmentOne.enviro, enviro.destination, enviro.randomPath, env3Timer);
            if(enviro.destination == 1) sequence(enviroIdentifier, environmentTwo.enviro, enviro.destination, enviro.randomPath, env3Timer);
        }
    }
}


// runs any sequence for communication between two environments
void ofApp::sequence(string sender, ParticleSystem& enviro, int destination, int sequenceNum, Timer& t){
    
    // convert origin name to int
    int originEnviro;
    if(sender == "Environment One") originEnviro = 0;
    if(sender == "Environment Two") originEnviro = 1;
    if(sender == "Environment Three") originEnviro = 2;
    
    // loop through the sequence array and trigger stones in the path at their particular time interval
    for(int i = 0; i < 15; i ++){
        triggerStoneSystem(sender, t, stoneSystems[enviroPaths[originEnviro][destination][sequenceNum][i]], i);
    }
    
    // find the last position in the sequence array that is not a 0
    int lastUsedPos = 0;
    
    for(int i = 0; i < 15; i++){
        int valAtIndex = enviroPaths[originEnviro][destination][sequenceNum][i];
        if(valAtIndex != 0) lastUsedPos ++;
        if(valAtIndex == 0) break;
    }
    
    triggerEnviro(sender, enviro, t, lastUsedPos);
    sequenceComplete(sender, t, lastUsedPos + 1);
}

// runs any sequence for communication between the sensor and an environment

void ofApp::sensorSequence(string sender, int destination, int sequenceNum, Timer& t){
    
    // loop through the sequence array and trigger stones in the path at their particular time interval
    for(int i = 0; i < 15; i ++){
        triggerStoneSystem(sender, t, stoneSystems[sensorPaths[destination][sequenceNum][i]], i);
    }
    
    // find the last position in the sequence array that is not a 0
    int lastUsedPos = 0;
    
    for(int i = 0; i < 15; i++){
        int valAtIndex = sensorPaths[destination][sequenceNum][i];
        if(valAtIndex != 0) lastUsedPos ++;
        if(valAtIndex == 0) break;
    }
    
    if(destination == 0) triggerEnviro(sender, environmentOne.enviro, t, lastUsedPos);
    if(destination == 1) triggerEnviro(sender, environmentTwo.enviro, t, lastUsedPos);
    if(destination == 2) triggerEnviro(sender, environmentThree.enviro, t, lastUsedPos);
    
    sequenceComplete(sender, t, lastUsedPos + 1);
}

// function which completes the stone sequence, setting the sequence state to inactive at the end, ready for another trigger.
void ofApp::sequenceComplete(string sender, Timer& t, int timing){
    if (t.timer > (timeSpacing * (timing + 6))){

        //check which timer/environment is being addressed and turn off the relative sequenceActive state
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
void ofApp::triggerStoneSystem(string sender, Timer& t, StoneSystem* stone, int timing){
    // the four bools correspond to which particles will fade in and out.
    
    //turn on the stone
    if(t.timer > timeSpacing * timing){
        if(sender == "Environment One") stone -> env1 = true;
        if(sender == "Environment Two") stone -> env2 = true;
        if(sender == "Environment Three") stone -> env3 = true;
        if(sender == "Sensor") stone -> sens = true;
    }
    //turn off the stone
    if(t.timer  > timeSpacing * timing + 3000){
        stone -> releaseTimer.reset();
        if(sender == "Environment One") stone -> env1 = false;
        if(sender == "Environment Two") stone -> env2 = false;
        if(sender == "Environment Three") stone -> env3 = false;
        if(sender == "Sensor") stone -> sens = false;
    }
}


// The following three functions turn on and off an environment's impact state to complete the visual sequence

void ofApp::triggerEnviro(string sender, ParticleSystem& enviro, Timer& t, int timing){
    
    //turn on the environment impact
    if (t.timer > timeSpacing * timing){
        enviro.impact = true;
    }
    //turn off the environment impact
    if  (t.timer > (timeSpacing * timing) + t.activeStoneTime){
        enviro.impact = false;
    }
    // trigger the environment's new rules once impacted
    if (t.timer > timeSpacing * timing && t.timer < timeSpacing * timing + 100){
        if(sender == "Sensor") enviro.triggeredBySensor = true;
        enviro.setNewRules = true;
    }
    
}


/////////SERIAL COMMUNICATION WITH ARDUINO///////////

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


