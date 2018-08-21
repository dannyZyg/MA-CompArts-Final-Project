//
//  Envelope.hpp
//  FinalProjectMA
//
//  Created by Danny on 18/8/18.
//
#pragma once

#ifndef Envelope_hpp
#define Envelope_hpp

#include <stdio.h>
#include "ofMain.h"
#endif /* Envelope_hpp */


class Envelope{
    
public:
    Envelope();
    
    void run();
    void status();
    float output(bool trigger, float min, float max, float release);
    bool completion();
    int counter;
    float newVal;
    bool active;
    bool complete;
};
