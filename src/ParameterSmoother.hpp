//
//  ParameterSmoother.hpp
//  magnetsKinect
//
//  Created by Danny on 23/4/18.
//

// Parameter Smoother class - original code from https://www.youtube.com/watch?v=BdJRSqgEqPQ

#pragma once

#ifndef ParameterSmoother_hpp
#define ParameterSmoother_hpp

#include <stdio.h>
#include "ofMain.h"

#endif /* ParameterSmoother_hpp */


class ParameterSmoother{
    
public:
    ParameterSmoother(float smoothingTimeMS, float frameRate);

    inline float process(float input){
            z1 = (input * b ) + (z1 * a);
            float result(z1);
            return result;
    }
    
private:
    float a, b, z1;

};

struct smoothValue {
    float targetValue;
    float currentValue;
    ParameterSmoother* smoother;
};
