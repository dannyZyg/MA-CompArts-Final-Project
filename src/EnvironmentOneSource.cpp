//
//  EnvironmentOneSource.cpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#include "EnvironmentOneSource.hpp"



EnvironmentOneSource::EnvironmentOneSource(){
    


    
    name = "Environment One";
    allocate(600, 600);
    
    width = fbo-> getWidth();
    height = fbo -> getHeight();
    
}

void EnvironmentOneSource::setup(){
    
    
    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
    

}

void EnvironmentOneSource::update(){
    
    
}

void EnvironmentOneSource::draw(){
   
    ofBackground(255);
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(255, 0, 0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
    enviro.display();
    
}


