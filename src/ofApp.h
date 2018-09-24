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
    
    
    vector <StoneSystem * > stoneSystems;
    
    void loadStonesFromSource(StoneSource& source);
    void triggerStoneSystem(string sender, Timer& t, StoneSystem* stone, int timing);
    void sequence(string sender, ParticleSystem& enviro,int destination, int sequenceNum, Timer& t);
    void sensorSequence(string sender, int destination, int sequenceNum, Timer& t);
    void environmentSequenceActivation(string enviroIdentifier, ParticleSystem& enviro, Timer& t);
    void triggerEnviro(string sender, ParticleSystem& enviro, Timer& t, int timing);
    
// Four dimensional array holding all of the sequence pahts from environment to environment
    // First layer contains three environments
        // Second layer holds two possible destinations
            // Third layer holds three possible sequences
                // Fourth layer holds the address for the specific stone in the sequence
    
int enviroPaths [3][2][3][15] = {
                            { // Environment One
                                { // E1 - > E2
                                    {9, 23, 27, 24, 15, 28},
                                    {10, 11, 13, 28, 15},
                                    {8, 21, 9, 22, 23, 7}
                                },
                                { // E1 -> E3
                                    {8, 9, 22, 26, 19, 6, 20, 18},
                                    {27, 23, 7, 22, 9, 21, 26, 19, 6},
                                    {9, 21, 26, 19, 6}
                                }
                            },
                            { // Environment Two
                                { // E2 -> E1
                                    {15, 28, 14, 13, 11, 10},
                                    {7, 22, 26, 21, 8, 9, 23},
                                    {7, 22, 21, 8, 9, 23}
                                },
                                { // E2 -> E3
                                    {4, 5, 3, 17, 25, 2, 1},
                                    {7, 22, 9, 21, 26, 19, 6},
                                    {4, 5, 17, 3, 18}
                                }
                            },
                            { // Environment Three
                                { // E3 -> E1
                                    {6, 19, 26, 21, 8},
                                    {6, 19, 26, 22, 9},
                                    {6, 20, 22, 23, 27, 15, 24, 10}
                                },
                                { // E3 -> E2
                                    {1, 2, 25, 17, 5, 3, 4},
                                    {6, 19, 26, 22, 7},
                                    {18, 3, 17, 5, 4}
                                }
                            }
                        };
    
    
// Three dimensional array for all of the sensor sequencce paths
    // Three dimensional array holding all of the sequence pahts from environment to environment
        // First layer holds three possible destinations
            // Second layer holds three possible sequences
                // Third layer holds the address for the specific stone in the sequence
    
int sensorPaths[3][3][15] =    {
                        { // Paths to Environment One
                            {14, 13, 11, 24, 10 },
                            {14, 28, 15, 24, 27, 23, 9, 8},
                            {14, 28, 15, 27, 23, 7, 22, 26, 21, 8}
                        },
                        { // Paths to Environment Two
                            {14, 13, 24, 27, 23, 7},
                            {12, 11, 13, 14, 28, 15},
                            {14, 28, 15, 24, 27, 23, 7, 20, 18, 3, 4}
                        },
                        { // Paths to Environment Three
                            {12, 11, 24, 27, 23, 9, 21, 26, 19},
                            {14, 13, 28, 15, 24, 27, 23, 7, 22, 20, 6},
                            {13, 28, 15, 24, 27, 23, 22, 20, 18, 3, 4, 5, 25, 2, 1  }
                        }
                    };
    
    bool debug, printInfo;
    ofTrueTypeFont verdana30; //font for geometry labels
    
    int timeSpacing; //time interval between each sequential stone trigger
    void sensorToSystem(Timer& t, int destination, int variation);
    void sequenceComplete(string sender, Timer& t, int timing);

    bool verifiedTrigger;
    void proximitySensorToBlur();
    bool sensorSequenceActive;
    int sensorPath, sensorDestination, lastDestination;
    
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
