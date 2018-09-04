//
//  Timer.cpp
//  FinalProjectMA
//
//  Created by Danny on 18/8/18.
//

#include "Timer.hpp"

Timer::Timer(){
    activeStoneTime = 3000;
    endTime = 0;
    reached = true;
}

void Timer::setup(){
    startTime = ofGetElapsedTimeMillis();
    
}

void Timer::run(){
    timer = ofGetElapsedTimeMillis() - startTime;
    
    if(timer >= endTime && !reached) {
        reached = true;
        ofMessage msg("Timer Reached");
        ofSendMessage(msg);
    }
    
    // get the percantage of the timer
    pct = ofMap(timer, 0.0, endTime, 0.0, 1.0, true);

}

void Timer::reset(){
    startTime = ofGetElapsedTimeMillis();
    reached = false;
}

