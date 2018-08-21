//
//  Scheduler.hpp
//  FinalProjectMA
//
//  Created by Danny on 19/8/18.
//

#ifndef Scheduler_hpp
#define Scheduler_hpp

#include <stdio.h>
#include "ofMain.h"
#include "StoneParticleSystem.h"


#endif /* Scheduler_hpp */

class Scheduler{
public:
    Scheduler();
    
    void recieveStones(StoneParticleSystem& s);
    void run();
    

    
};
