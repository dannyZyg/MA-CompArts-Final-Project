//
//  Timer.hpp
//  FinalProjectMA
//
//  Created by Danny on 18/8/18.
//
#pragma once

#ifndef Timer_hpp
#define Timer_hpp

#include <stdio.h>
#include "ofMain.h"

#endif /* Timer_hpp */


class Timer{
    
public:
    Timer();
    
    
    void run();
    void reset();
    void setup();
    void setup(int interval);
    
    float startTime;
    float endTime; // when do want to stop the timer

    float timer;
    float pct;
    
    
    float intervalLength;
    bool complete;
    bool active;
    bool begin;

    
    float activeStoneTime;
    
    bool reached; // used as a trigger when we hit the timer
    
};
