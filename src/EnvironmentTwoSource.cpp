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
    
    setScale = 0;
    setRotation = -3.5;
    blurOffset = 2.5;
    blurPasses = 2;
}

void EnvironmentTwoSource::setup(){
    
    blur.setup(width,height, 4, .1, 8);

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
    enviro.setup(width, height, 4);
    
    debug = false;
    blur1 = true;
    blur2 = true;
    
    
    // divide the main circle into a number of 'cells'
    int num = 3;
    for(int i = 0; i < num; i ++){
        float r = rad / num * i;
        cells.push_back(r);
        
        float seed = ofRandom(1000);
        noiseSeed.push_back(seed);
    }
    

    
    
    
}

void EnvironmentTwoSource::update(){

    setRotation = ofMap(ofSignedNoise(ofGetFrameNum() * 0.01 + 1000), -1, 1, -PI, PI);

    blur.setScale(setScale);
    blur.setRotation(setRotation);
    
    gpuBlur.blurOffset = blurOffset;
    gpuBlur.blurPasses = blurPasses;
    gpuBlur.numBlurOverlays = 6;
    gpuBlur.blurOverlayGain = 250;
    
    
//     for all cell boundaries, contract and expand with perlin noise
    for(int i = 0; i < cells.size(); i ++){
        float noise = ofSignedNoise(ofGetFrameNum() * 0.01 + noiseSeed[i]) * 1.2;
        cells[i] += noise;
    }
    
    enviro.receiveCells(cells);
    enviro.updateColours();

    
//    if(active){
//        enviro.impact = true;
//    }
//    
//    if(!active){
//        enviro.impact = false;
//    }
}

void EnvironmentTwoSource::draw(){

    if(blur2) gpuBlur.beginDrawScene();
    if(blur1) blur.begin();
    ofBackground(0);
    
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
        
        // Draw white circle over environment and display fbo label

        ofPushStyle();
        ofFill();
        ofSetLineWidth(5);
        ofSetColor(255);
        ofDrawCircle(origin, rad);
        ofSetColor(0);
        font.drawString("Enviro 2", origin.x - 200, origin.y);
        ofPopStyle();
    }
    
    // a debug mode for displaying the moving cell walls
    bool drawCellWalls = false;
    if(drawCellWalls) drawCellDebug();
    
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


