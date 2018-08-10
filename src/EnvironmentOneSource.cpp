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
    
    
   
    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
    ofBackground(255);
    
    //refresh background circle colour every frame
    ofPushStyle();
    ofFill();
    ofSetLineWidth(5);
    ofSetColor(0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
    enviro.display();
    
    
    if(debug){
        
        // Draw white circle over environment and display fbo position label

        
        ofPushStyle();
        ofFill();
        ofSetLineWidth(5);
        ofSetColor(255);
        ofDrawCircle(origin, rad);
        ofSetColor(0);
        font.drawString("Enviro 1", origin.x - 200, origin.y);
        ofPopStyle();
        
        
    }
   
    
}


