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
    
    blur.setup(width,height, 3, 0.1, 4);

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
    s.numSamples = 8;
    s.numColorbuffers = 6;
    s.useDepth = true;
    s.useStencil = true;
    
    
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
    gpuBlur.numBlurOverlays = 4;
    gpuBlur.blurOverlayGain = 255;
    
    
    // for all cell boundaries, contract and expand with perlin noise
    for(int i = 0; i < cells.size(); i ++){
        float noise = ofSignedNoise(ofGetFrameNum() * 0.01 + noiseSeed[i]) * 1.2;
        cells[i] += noise;
    }
    
    enviro.receiveCells(cells);
//    enviro.update(100);

    
}

void EnvironmentTwoSource::draw(){
    
    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
    ofBackground(255);
    
    //refresh background circle colour every frame
    ofPushStyle();
    ofFill();
    ofSetLineWidth(5);
    ofSetColor(0);
    ofDrawCircle(origin, rad);
    ofPopStyle();
    
//     gpuBlur.beginDrawScene();
//    ofClear(255);
//    blur.begin();

   
    enviro.display();
    


//    gpuBlur.endDrawScene();
//    //calc the fbo blurring, no drawing on screen yet
//    gpuBlur.performBlur();
//
//    //draw the "clean" scene on screen
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    gpuBlur.drawSceneFBO();
//
//    //overlay the blurred fbo on top of the previously drawn clean scene
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    gpuBlur.drawBlurFbo();
    
    
//    blur.end();
//    blur.draw();
    
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
    
    ofSetColor(255);
    ofNoFill();
    ofDrawCircle(origin, rad);
    
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


