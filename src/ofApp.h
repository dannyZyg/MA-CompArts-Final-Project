#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "StoneParticleSystem.h"

#include "StonesSmall_1_4.hpp"
#include "StonesSmall_5_8.hpp"
#include "StonesSmall_9_12.hpp"
#include "StonesSmall_13_16.hpp"
#include "StonesMed_1_4.hpp"
#include "StonesMed_5_8.hpp"
#include "StonesLarge_1_4.hpp"
#include "EnvironmentOneSource.hpp"
#include "EnvironmentTwoSource.hpp"
#include "EnvironmentThreeSource.hpp"
#include "Timer.hpp"
#include "Envelope.hpp"
#include "Scheduler.hpp"



#define NUM_BYTES 2

class ofApp : public ofBaseApp{
public:
//    ofApp(){}
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    
    void scheduler();

    ofxPiMapper piMapper;

    // By using a custom source that is derived from FboSource
    // you will be able to see the source listed in sources editor

    StonesSmall_1_4 small_stones_1_4;
    StonesSmall_5_8 small_stones_5_8;
    StonesSmall_9_12 small_stones_9_12;
    StonesSmall_13_16 small_stones_13_16;
    StonesMed_1_4 med_stones_1_4;
    StonesMed_5_8 med_stones_5_8;
    StonesLarge_1_4 large_stones;
    EnvironmentOneSource environmentOne;
    EnvironmentTwoSource environmentTwo;
    EnvironmentThreeSource environmentThree;
        
    bool drawTemplate;
    ofImage layout;
  
    bool displayCircleTemplate;
    
    bool debug;
    
    void debugDisplay();
    ofTrueTypeFont verdana30;
    
    float startTime;
    int timeSpacing;
    
    
    void E1_to_E2(int variation);
    void E1_to_E3(int variation);
    void E2_to_E1(int variation);
    void E2_to_E3(int variation);
    void E3_to_E1(int variation);
    void E3_to_E2(int variation);
    
    void sensorToSystem(int variation);

    void sequence2();

    
    // Serial conversion code from Joshua Noble's "Programming Interactivity:
    // A Designer's Guide to Proccessing, Arduino and openFrameworks" (p. 229-232, 2012)
    // available at https://books.google.co.uk/books?id=sAsHA1HM1WcC&pg=PA225&lpg=PA225&dq=serial+input+from+arduino+openframeworks&source=bl&ots=KHz3UccApe&sig=mI6JR-iB4WiLT9Q-opHa-OZiNZQ&hl=en&sa=X&ved=0ahUKEwjmvtnD_I7aAhWBWhQKHW5EAKw4ChDoAQg1MAI#v=snippet&q=ofSerial&f=false
    void serialSetup();
    void serialUpdate();
    // a flag for whether to send our 'handshake' to arduino
    bool                    bSendSerialMessage;
    int                     countCycles;
    unsigned char           bytesRead[NUM_BYTES];
    
    // average the values from the sensor with array, clamp values to remove large spikes in readings
    int                     currentSample = 0;
    int                     val;
    int                     vals[8];
    int                     clampVal;
    int                     avg;
    ofSerial                serial;

    
    void fillMapWithStones();
    // a map where the keys are integers and the values are strings
    std::map<int, std::string> stoneIndex;
    
//    std::vector<shared_ptr<StoneParticleSystem> > systems;
    
//    vector<StoneParticleSystem&> systems;
    
    void triggerStone(Timer& timer, StoneParticleSystem& stone, int timing);
    void sequenceComplete(Timer& timer_, int timing);
    int count;
    
    
    
    void triggerEnviro1(Timer timer_, int timing);
    void triggerEnviro2(int timing);
    void triggerEnviro3(int timing);

    
    bool sensorTrigger;
    
    bool sequenceActive;
    float testTimer;
    void resetTimer();
    
    
    void SensorSequenceComplete();
    
    bool sensorSequenceActive;
    
    
    int e1Count, e2Count, e3Count;
    
    float activeLength;
    
    
    float sequenceSpacingStart;
    
    float timerSequenceSpacing;
    
    bool envTest;
    float testVal;
    
//    Timer testTimer;
    Timer env1Timer, env2Timer, env3Timer, sensorTimer;
    Envelope testEnvelope;
    
    bool env;
    int randomPath;
    Scheduler s;
    
};
