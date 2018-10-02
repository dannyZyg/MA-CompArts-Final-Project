# Nexus

Nexus aims to explore emergence, ecologies and networks. It is a projection mapping work utilising ofxPiMapper to project onto real world geometry. It consists of three unique environments which have their own rule systems for their agents (particles). When certain conditions are met within an environment, they communicate with another environment and can impact the destination environment's rules.

The conditions for communication are largely based on an environment's unique qualities, and are described in their relevant classes.

When the systems communicate, they send a signal of smaller particle clusters in a timed sequence along the physical shapes, which I will call stepping stones, or in the case of the code, between the smaller 'stone sources'. Each stone source FBO contains four separate 'stone systems', handling four physical stones on the structure when mapped.

The work is interactive, and connects with an arduino over serial. A proximity sensor detects human presence, and a hand motion can trigger a new sequence from the sensor location to a random envrionment. The regular communications between the three environments result in the destination receiving new rules in the form of a preset, however the interactive sensor sequences seed the environments with randomised values. While an interactive sequence is active, the receiving environment will have a blur applied to it which is controlled by the sensor value comming from the arduino. All other sequences will end and remain inactive until the interaction has ceased.

The code is built around a collection of FBO sources which are mapped to the physical shapes. Every physical shape has a unique instance of a particle system. There is a base particle class, a base particle system class, and derived particle system classes.

The flow of data is as such: 

Particle --> Particle System (or derived class) --> FBO source --> ofApp

Each of the three environments exists as its own derived particle system class. Each of the stones is a derived 'stone system' class.

The various sequence paths are contained in multidimensional arrays which are found in the ofApp.h



# Getting Started

The following addons are needed to compile the code:

ofxBlur
ofxColorPalette
ofxFboBlur
ofxGui
ofxPiMapper
ofxXmlSettings


# Acknowledgments

Thanks to Krisjanis Rijnieks for developing ofxPiMapper
https://ofxpimapper.com/

Thanks to Kyle McDonald for sharing his binned particle system with the openFrameworks community.
https://github.com/kylemcdonald/openFrameworksDemos/tree/master/BinnedParticleSystem/src
