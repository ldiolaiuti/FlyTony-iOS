//
//  GTODelegate.cpp
//  FlyTony
//
//  Created by Luca Diolaiuti on 04/04/14.
//
//

#include "GTODelegate.h"

GTODelegate::GTODelegate(){
    _debug=true;
}

GTODelegate::~GTODelegate(){}

void GTODelegate::onGameTrackingObtained(GameTrackingObjCpp obj){
    if(_debug){
        std::cout<<"GTODelegate onGameTrackingObtained";
    }
}


void GTODelegate::onGameTrackingFailed(){
    if (_debug) {
        std::cout<<"GTODelegate onGameTrackingFailed";
    }
}