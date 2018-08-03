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
    
    blur.setup(width,height, 10, .2, 4);

    
    
    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
    debug = false;

}

void EnvironmentThreeSource::update(){
    blur.setScale(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
    blur.setRotation(ofMap(ofGetMouseY(), 0, ofGetHeight(), -PI, PI));
    
}

void EnvironmentThreeSource::draw(){
   
    ofBackground(255);
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(255, 0, 0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
    
    blur.begin();
    
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
        ofDrawBitmapString("Enviro 3", origin.x, origin.y);
        ofPopStyle();
        
        
    }
    
    
}





