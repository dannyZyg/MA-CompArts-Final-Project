//
//  EnvironmentTwoSource.cpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#include "EnvironmentTwoSource.hpp"



EnvironmentTwoSource::EnvironmentTwoSource(){
    
    name = "Environment Two";
    allocate(600, 600);
    
    width = fbo-> getWidth();
    height = fbo -> getHeight();
    
}

void EnvironmentTwoSource::setup(){
    
    blur.setup(width,height, 10, .1, 2);

    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
    
    debug = false;

}

void EnvironmentTwoSource::update(){
    
    blur.setScale(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
    blur.setRotation(ofMap(ofGetMouseY(), 0, ofGetHeight(), -PI, PI));
}

void EnvironmentTwoSource::draw(){
   
    
    ofBackground(255);
    blur.begin();

    
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(255, 0, 0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
    
   
    enviro.display();
    
    
    blur.end();
    blur.draw();
    
    if(debug){
        ofPushStyle();
        ofFill();
        ofSetLineWidth(5);
        ofSetColor(255);
        ofDrawCircle(origin, rad);
        ofSetColor(0);
        ofDrawBitmapString("Enviro 2", origin.x, origin.y);
        ofPopStyle();
        
        
    }
    
}


