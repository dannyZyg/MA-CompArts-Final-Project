//
//  Timer.cpp
//  FinalProjectMA
//
//  Created by Danny on 18/8/18.
//

#include "Timer.hpp"


Timer::Timer(){
//    intervalLength = intervalLength_;
    
}

void Timer::setup(int interval){
    startTime = ofGetElapsedTimeMillis();
    intervalLength = interval;
    
    
}


void Timer::run(){
    timer = ofGetElapsedTimeMillis() - startTime;
    
    if(timer > intervalLength){
        complete = true;
    }
    else{
        complete = false;
    }
}

void Timer::reset(){
    startTime = ofGetElapsedTimeMillis();
}

