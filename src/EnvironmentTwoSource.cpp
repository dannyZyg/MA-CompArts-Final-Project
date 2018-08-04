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
    
    blur.setup(width,height, 10, .01, 8);

    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
    
    debug = false;
    
    s.width = width;
    s.height = height;
    s.internalformat = GL_RGB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    
    
    gpuBlur.setup(s);
    
    int num = 3;
    for(int i = 0; i < num; i ++){
        float r = rad / num * i;
        cells.push_back(r);
        
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
    
    

}

void EnvironmentTwoSource::update(){
    
    blur.setScale(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
    blur.setRotation(ofMap(ofGetMouseY(), 0, ofGetHeight(), -PI, PI));
    
    gpuBlur.blurOffset = 5 * ofMap(ofGetMouseX(), 0, ofGetHeight(), 1, 0, true);
    gpuBlur.blurPasses = 10. * ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 1, true);
    gpuBlur.numBlurOverlays = 8;
    gpuBlur.blurOverlayGain = 255;
    
    
    
    for(int i = 0; i < cells.size(); i ++){
        
        float noise = ofSignedNoise(ofGetFrameNum() * 0.01 + noiseSeed[i]);
        
        cells[i] += noise;

    }
    
    enviro.receiveCells(cells);
    enviro.update(100);

    
}

void EnvironmentTwoSource::draw(){
   
    

    

    
//    blur.begin();
    
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
   
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(255, 0, 0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
//     gpuBlur.beginDrawScene();
    
    ofBackground(255);
//    ofClear(255);
    enviro.display();
    
    
//    gpuBlur.endDrawScene();
//
//    //blur the fbo
//    //blending will be disabled at this stage
//    gpuBlur.performBlur();

//    //draw the "clean" scene
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    gpuBlur.drawSceneFBO();
//
//    //overlay the blur on top
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
//    gpuBlur.drawBlurFbo();
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
//    blur.end();
//    blur.draw();
    
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
    
    
    //draw the noisy circle guide
    
    for(int i = 0; i < cells.size(); i ++){
        ofSetColor(255, 0, 0);
        ofNoFill();
        ofDrawCircle(origin, cells[i]);
        ofDrawBitmapString(ofToString(i), origin.x + cells[i], origin.y);
    }
    
}


