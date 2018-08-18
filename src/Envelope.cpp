//
//  Envelope.cpp
//  FinalProjectMA
//
//  Created by Danny on 18/8/18.
//

#include "Envelope.hpp"


Envelope::Envelope(){
    
}

float Envelope::output(bool trigger, float min, float max, float release){
    
    if(trigger){
        active = true;
    }
    if(active){
        counter ++;
        complete = false;
        active = true;
    }
    if(!trigger){
        active = false;
    }
    if(counter > release){
        counter = 0;
        complete = true;
    }
    newVal = ofMap(counter, 0, release, min, max);
    return newVal;
}
