//
//  EnvironmentThreeSource.cpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#include "EnvironmentThreeSource.hpp"



EnvironmentThreeSource::EnvironmentThreeSource(){
    


    
    name = "Environment Three";
    allocate(600, 600);
    
    width = fbo-> getWidth();
    height = fbo -> getHeight();
    
}

void EnvironmentThreeSource::setup(){
    
    
    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
    

}

void EnvironmentThreeSource::update(){
    
    
}

void EnvironmentThreeSource::draw(){
   
    ofBackground(255);
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(255, 0, 0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
    enviro.display();
    
}


