// Timer class is based on the timer example from openFrameworks.
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
    
    float startTime;
    float endTime; // how long the timer should run for
    float activeStoneTime; //used for the stepping stones. The time that each stone uses to fade in/out

    float timer; // time since startTime
    float pct; // percentage of the timer completed
    bool reached; // used as a trigger when we hit the timer

    
//    bool complete;
//    bool active;
//    bool begin;

    
    
    
};
