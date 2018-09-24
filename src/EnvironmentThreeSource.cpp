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
    
    width =  600;
    height = 600;
    
    setScale = 0;
    setRotation = -2.5;
    blurOffset = 4.2;
    blurPasses = 1;
    
}

void EnvironmentThreeSource::setup(){
    
    
    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 4);
    debug = false;
    
    blur.setup(width,height, 4, .1, 6);
    
    
    s.width = width;
    s.height = height;
    s.internalformat = GL_RGBA;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 6;
    s.numColorbuffers = 3;
    s.useDepth = true;
    s.useStencil = true;
    
    
    gpuBlur.setup(s);
    
    blur1 = true;
    blur2 = true;
    
    
}

void EnvironmentThreeSource::update(){
    
    setRotation = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 500), -1, 1, -PI, PI);

    
    blur.setScale(setScale);
    blur.setRotation(setRotation);
    
    gpuBlur.blurOffset = blurOffset;
    gpuBlur.blurPasses = blurPasses;
    gpuBlur.numBlurOverlays = 3;
    gpuBlur.blurOverlayGain = 250;
    
//    if(active){
//        enviro.impact = true;
//    }
//    
//    if(!active){
//        enviro.impact = false;
//    }
}

void EnvironmentThreeSource::draw(){

    if(blur2) gpuBlur.beginDrawScene();
    if(blur1)blur.begin();

    ofBackground(0);
    enviro.drawTeamLines();
    enviro.display();
    
    if(blur1)blur.end();
    if(blur1)blur.draw();

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
        ofPushMatrix();
        ofPushStyle();
        ofFill();
        ofSetLineWidth(5);
        ofSetColor(255, 0, 0);
        ofDrawRectangle(0, 0, width, height);
        ofSetColor(255);
        ofDrawCircle(origin, rad);
        ofSetColor(0);
        font.drawString("Enviro 3", origin.x - 200, origin.y);
        ofPopStyle();
        ofPopMatrix();

    }

}
