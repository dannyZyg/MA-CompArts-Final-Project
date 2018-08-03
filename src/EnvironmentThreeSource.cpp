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
    
    blur.setup(width,height, 8, .1, 4);
    
    
    s.width = width;
    s.height = height;
    s.internalformat = GL_RGBA;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 6;
    s.numColorbuffers = 3;
    s.useDepth = true;
    s.useStencil = true;
    
    
    gpuBlur.setup(s);

    
    
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
    
    
    gpuBlur.blurOffset = 5 * ofMap(ofGetMouseX(), 0, ofGetHeight(), 1, 0, true);
    gpuBlur.blurPasses = 10. * ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 1, true);
    gpuBlur.numBlurOverlays = 3;
    gpuBlur.blurOverlayGain = 150;
    
    
    
    
}

void EnvironmentThreeSource::draw(){
   
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(255, 0, 0);
    ofDrawCircle(origin, rad);
    ofPopStyle();

    
    
    gpuBlur.beginDrawScene();
    
    
    ofBackground(0);
    
//    blur.begin();
    

    
    enviro.display();
//    blur.end();
//    blur.draw();
    
    gpuBlur.endDrawScene();
    //calc the fbo blurring, no drawing on screen yet
    gpuBlur.performBlur();

    //draw the "clean" scene on screen
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur.drawSceneFBO();

    //overlay the blurred fbo on top of the previously drawn clean scene
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    gpuBlur.drawBlurFbo();
    
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





