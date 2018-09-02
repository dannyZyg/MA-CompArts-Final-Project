#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "StoneSystem.hpp"
#include "EnvironmentOneSource.hpp"
#include "EnvironmentTwoSource.hpp"
#include "EnvironmentThreeSource.hpp"
#include "Timer.hpp"
#include "ParticleSystem.h"
#include "StoneSource.hpp"
#include "ParameterSmoother.hpp"

#define NUM_BYTES 2

class ofApp : public ofBaseApp{
public:
    
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

    //FBO sources: Three main environments
    EnvironmentOneSource environmentOne;
    EnvironmentTwoSource environmentTwo;
    EnvironmentThreeSource environmentThree;
    
    //FBO sources: stepping stone sources. Each FBO source takes care of four stones by dividing the canvas.
    SmallStones_1_4 small_stones_1_4;
    SmallStones_5_8 small_stones_5_8;
    SmallStones_9_12 small_stones_9_12;
    SmallStones_13_16 small_stones_13_16;
    MediumStones_1_4 med_stones_1_4;
    MediumStones_5_8 med_stones_5_8;
    LargeStones_1_4 large_stones;
    
    bool debug, printInfo;
    ofTrueTypeFont verdana30; //font for geometry labels
    
    int timeSpacing; //time interval between each sequential stone trigger
    
    void E1_to_E2(Timer& t, int variation);
    void E1_to_E3(Timer& t, int variation);
    void E2_to_E1(Timer& t, int variation);
    void E2_to_E3(Timer& t, int variation);
    void E3_to_E1(Timer& t, int variation);
    void E3_to_E2(Timer& t, int variation);
    void sensorToSystem(Timer& t, int destination, int variation);
    void triggerStone(string sender, Timer& t, StoneSystem& stone, int timing);
    void triggerEnviro1(string sender, Timer& t, int timing);
    void triggerEnviro2(string sender, Timer& t, int timing);
    void triggerEnviro3(string sender, Timer& t, int timing);
    void sequenceComplete(string sender, Timer& t, int timing);

    bool sensorTrigger, verifiedTrigger;
    void proximitySensorToBlur();
    bool sensorSequenceActive;
    int sensorPath, sensorDestination, lastPath;
    
    Timer env1Timer, env2Timer, env3Timer, sensorTimer; // timers for the communication sequences
    smoothValue env1SmoothedSensor, env2SmoothedSensor, env3SmoothedSensor; //smoothing of the sensor value
    
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
    
    
};
