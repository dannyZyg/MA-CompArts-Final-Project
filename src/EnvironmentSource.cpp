//
//  EnvironmentSource.cpp
//  FinalProjectMA
//
//  Created by Danny on 28/7/18.
//

#include "EnvironmentSource.hpp"

EnvironmentSource::EnvironmentSource(){
    
    
    allocate(600, 600);
    width = fbo-> getWidth();
    height = fbo -> getHeight();
    
    setScale = 1.5;
    setRotation = 1.1;
    blurOffset = 3.5;
    blurPasses = 1;
    
  
}

void EnvironmentSource::setup(){
    name = "Environment Source";
    blur.setup(width,height, 8, .1, 4);
    
    s.width = 600;
    s.height = 600;
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
    
    blur1 = true;
    blur2 = true;
    startTime = 0;
    //    active = true;
}

void EnvironmentTestSource::setup(){
    name = "Environment test";
    blur.setup(width,height, 8, .1, 4);
    
    s.width = 600;
    s.height = 600;
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
    
    blur1 = true;
    blur2 = true;
    startTime = 0;
    //    active = true;
}
void EnvironmentTest2Source::setup(){
    name = "Environment test 2";
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
}
//void EnvironmentThreeSource::setup(){
//    name = "Environment Three";
//}


void EnvironmentSource::update(){
    blur.setScale(setScale);
    blur.setRotation(setRotation);
    
    
    gpuBlur.blurOffset = blurOffset;
    gpuBlur.blurPasses = blurPasses;
    gpuBlur.numBlurOverlays = 5;
    gpuBlur.blurOverlayGain = 150;
    
    //    enviro.addRepulsionForce(100, 300, 200, 1);
    
    
    
    
    if(active){
        //        startTime = 0;
        enviro.impact = true;
    }
    
    if(!active){
        enviro.impact = false;
        
    }
    
    
    
    newRules();
    
    
    
}

void EnvironmentSource::draw(){
    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
    if(blur2) gpuBlur.beginDrawScene();
    if(blur1)blur.begin();
    
    ofBackground(0);    //refresh background circle colour every frame
    ofPushStyle();
    ofFill();
    ofSetLineWidth(5);
    ofSetColor(0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
    ofSetColor(0, 255, 0);
    enviro.display();
    
    
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

void EnvironmentSource::newRules(){
    
    if(active){
        
        //        enviro.centerAttraction = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01), -1, 1, -1.5, 1.5);
        
    }
    
}

