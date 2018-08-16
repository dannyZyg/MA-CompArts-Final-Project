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
    
    width =  600;//fbo-> getWidth();
    height = 600;//fbo -> getHeight();
    
}

void EnvironmentThreeSource::setup(){
    
    
    origin = ofVec2f(width/2, height/2);
    rad = width/2;
    
    enviro.origin = origin;
    enviro.externalRad = rad;
    enviro.setup(width, height, 5);
    debug = false;
    
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
    
    blur1 = true;
    blur2 = true;
    
    
}

void EnvironmentThreeSource::update(){
    blur.setScale(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
    blur.setRotation(ofMap(ofGetMouseY(), 0, ofGetHeight(), -PI, PI));
    
    
    gpuBlur.blurOffset = 5 * ofMap(ofGetMouseX(), 0, ofGetHeight(), 1, 0, true);
    gpuBlur.blurPasses = 10. * ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 1, true);
    gpuBlur.numBlurOverlays = 3;
    gpuBlur.blurOverlayGain = 150;
    
    
    
//    cout<< gpuBlur.blurPasses<<endl;
    if(active){
        //        startTime = 0;
        enviro.impact = true;
    }
    
    if(!active){
        enviro.impact = false;
        
    }
    
    

    
    
}

void EnvironmentThreeSource::draw(){
   
    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
  

    if(blur2) gpuBlur.beginDrawScene();
    if(blur1)blur.begin();

    ofBackground(0);
//    //refresh background circle colour every frame
//    ofPushStyle();
//    ofFill();
//    ofSetLineWidth(5);
//    ofSetColor(0);
//    ofDrawCircle(origin, rad);
//    ofPopStyle();
//
//    ofClear(0);

   

   
 
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

        ofPushStyle();
        ofFill();
        ofSetLineWidth(5);
        ofSetColor(255);
        ofDrawCircle(origin, rad);
        ofSetColor(0);
        font.drawString("Enviro 3", origin.x - 200, origin.y);
        ofPopStyle();

    }

}





