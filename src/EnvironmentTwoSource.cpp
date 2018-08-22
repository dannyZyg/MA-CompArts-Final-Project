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
    enviro.setupColours();
    
    debug = false;
    
    blur1 = false;
    blur2 = false;
    
    int num = 3;
    for(int i = 0; i < num; i ++){
        float r = rad / num * i;
        cells.push_back(r);
        
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
    

    
    
    
}

void EnvironmentTwoSource::update(){
    

    blur.setScale(3);//ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
    blur.setRotation(-PI);//ofMap(ofGetMouseY(), 0, ofGetHeight(), -PI, PI));
    
    
    gpuBlur.blurOffset = 3;//5 * ofMap(ofGetMouseX(), 0, ofGetHeight(), 1, 0);
    gpuBlur.blurPasses = 0.7;//10. * ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 1);
    gpuBlur.numBlurOverlays = 3;
    gpuBlur.blurOverlayGain = 150;
    
    
    // for all cell boundaries, contract and expand with perlin noise
    for(int i = 0; i < cells.size(); i ++){
        float noise = ofSignedNoise(ofGetFrameNum() * 0.01 + noiseSeed[i]) * 1.2;
        cells[i] += noise;
    }
    
    enviro.receiveCells(cells);
//    enviro.update(100);
    enviro.updateColours();

    
    if(active){
        //        startTime = 0;
        enviro.impact = true;
    }
    
    if(!active){
        enviro.impact = false;
        
    }
    
}

void EnvironmentTwoSource::draw(){

    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
    if(blur2) gpuBlur.beginDrawScene();
    if(blur1) blur.begin();
    ofBackground(0);
    

    
//    //refresh background circle colour every frame
//    ofPushStyle();
//    ofFill();
//    ofSetLineWidth(5);
//    ofSetColor(0);
//    ofDrawCircle(origin, rad);
//    ofPopStyle();
    
//    ofClear(255);
//    blur.begin();

//   enviro.behaviourState();
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
        font.drawString("Enviro 2", origin.x - 200, origin.y);
        ofPopStyle();
    }
    
//    ofSetColor(255);
//    ofNoFill();
//    ofDrawCircle(origin, rad);
    
//    drawCellDebug();
    
    
    
}

void EnvironmentTwoSource::drawCellDebug(){
  //  draw the noisy circle guide
    
        for(int i = 0; i < cells.size(); i ++){
            ofSetColor(255);
            ofNoFill();
            ofDrawCircle(origin, cells[i]);
            ofDrawBitmapString(ofToString(i), origin.x + cells[i], origin.y);
            }
    
}


