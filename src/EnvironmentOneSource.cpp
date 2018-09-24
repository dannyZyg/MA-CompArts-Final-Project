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
    
    setScale = 0;
    setRotation = 1.1;
    blurOffset = 2.5;
    blurPasses = 1;
}

void EnvironmentOneSource::setup(){
    
    blur.setup(width,height, 4, .1, 6);
    
    s.width = 600;
    s.height = 600;
    s.internalformat = GL_RGBA;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 3;
    s.numColorbuffers = 6;
    s.useDepth = true;
    s.useStencil = true;
    
    
    gpuBlur.setup(s);
   
    
    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 4);
    
    blur1 = true;
    blur2 = true;
    startTime = 0;
}

void EnvironmentOneSource::update(){
    
    setRotation = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01), -1, 1, -PI, PI);
    
    blur.setScale(setScale);
    blur.setRotation(setRotation);
    
    gpuBlur.blurOffset = blurOffset;
    gpuBlur.blurPasses = blurPasses;
    gpuBlur.numBlurOverlays = 4;
    gpuBlur.blurOverlayGain = 165;
    
//    if(active){
//        enviro.impact = true;
//    }
//    
//    if(!active){
//        enviro.impact = false;
//    }
}

void EnvironmentOneSource::draw(){

    if(blur2) gpuBlur.beginDrawScene();
    if(blur1)blur.begin();
    
    ofBackground(0);    //refresh background circle colour every frame
    
    enviro.display();
    enviro.drawMembranes();
    
    if(blur1){
        blur.end();
        blur.draw();
    }
    
    if(blur2){
        gpuBlur.endDrawScene();
        //calc the fbo blurring, no drawing on screen yet
        gpuBlur.performBlur();
        
        //draw the "clean" scene on screen
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        gpuBlur.drawSceneFBO();
        
        //overlay the blurred fbo on top of the previously drawn clean scene
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        gpuBlur.drawBlurFbo();
        
    }
    
    
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
