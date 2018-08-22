//
//  StonesLarge_1_4.cpp
//  sketchClassTest
//
//  Created by Danny on 17/7/18.
//

#include "StonesLarge_1_4.hpp"



StonesLarge_1_4::StonesLarge_1_4(){

    name = "Large Stones 1-4";
    width = 800;
    height = 800;
    allocate(width, height);

}


void StonesLarge_1_4::setup(){
    debug = false;
    ofBackground(255);
    gs = width/2;
    numCells = fbo->getHeight()/gs;
    rad = gs/2;

    binPower = 5;

    for(int i = 0; i < numCells; i++){
        for(int k = 0; k < numCells; k++){
            ofVec2f tempOrigin;
            tempOrigin.set (k*gs + gs/2, i*gs + gs/2);
            origins.push_back(tempOrigin);

        }
    }

    for(int i = 0; i < origins.size(); i ++){
        StoneParticleSystem s;
        stones.push_back(s);


    }

    for(int i = 0; i < stones.size(); i ++){
        stones[i].baseColour = ofColor (216, 41, 32);
        stones[i].origin = origins[i];
        stones[i].externalRad = rad;
        stones[i].setup(width, height, binPower);
//        stones[i].setupColours();
    }
    
    
    blur.setup(width,height, 3, 0.1, 4);
    s.width = width;
    s.height = height;
    s.internalformat = GL_RGB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 8;
    s.numColorbuffers = 6;
    s.useDepth = true;
    s.useStencil = true;

    gpuBlur.setup(s);
}

void StonesLarge_1_4::update(){



    for(int i = 0; i < stones.size(); i ++){
//        stones[i].update();
        stones[i].updateColours();
    }

//    cout<<stones[1].timeIntervalPassed<<endl;
//    cout<<stones[1].timer<<endl;

//    blur.setScale(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
//    blur.setRotation(ofMap(ofGetMouseY(), 0, ofGetHeight(), -PI, PI));
//
//    gpuBlur.blurOffset = 5 * ofMap(ofGetMouseX(), 0, ofGetHeight(), 1, 0, true);
//    gpuBlur.blurPasses = 10. * ofMap(ofGetMouseY(), 0, ofGetWidth(), 0, 1, true);
//    gpuBlur.numBlurOverlays = 4;
//    gpuBlur.blurOverlayGain = 255;
    
    
}


void StonesLarge_1_4::draw(){


    //colour of background rectangle (behind circular canvas), used for trimming fbo scene precicesly to circle
    
    
    
    
//    gpuBlur.beginDrawScene();
    ofBackground(0);
//    blur.begin();

    
    //refresh background circle colour every frame, for all stepping stones in the fbo source
    
//    for(int i = 0; i < origins.size(); i ++){
//        ofPushStyle();
//        ofSetColor(0);
//        ofNoFill();
//        ofDrawRectangle(origins[i], gs, gs);
//        ofFill();
//        ofDrawCircle(origins[i], rad);
//        ofPopStyle();
//    }
    
    
    
    
// ofClear(255);


    for(int i = 0; i < stones.size(); i ++){
        stones[i].fadeParticles();
        stones[i].display();
    }

    if(debug) debugMode();


    
//        gpuBlur.endDrawScene();
//        //calc the fbo blurring, no drawing on screen yet
//        gpuBlur.performBlur();
//
//        //draw the "clean" scene on screen
//        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//        gpuBlur.drawSceneFBO();
//
//        //overlay the blurred fbo on top of the previously drawn clean scene
//        ofEnableBlendMode(OF_BLENDMODE_ADD);
//        gpuBlur.drawBlurFbo();
    
    
//        blur.end();
//        blur.draw();
    
    
}


void StonesLarge_1_4::reset(){

//    for(int i = 0; i < stones.size(); i ++){
//
//        startTime = ofGetElapsedTimeMillis();
////        stones[i].reset();
//    }

}


void StonesLarge_1_4::debugMode(){

    // Draw white circles over all stepping stones and display their position numbers
    
    for(int i = 0; i < stones.size(); i ++){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        ofDrawCircle(origins[i], rad);
        ofSetColor(0);
        font.drawString("L" + ofToString(i + 1),origins[i].x - 100, origins[i].y);
        ofPopStyle();
        
    }
}


