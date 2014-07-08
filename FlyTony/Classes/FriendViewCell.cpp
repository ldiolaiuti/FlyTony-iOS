//
//  CustomViewCell.cpp
//  FlyTony
//
//  Created by Luca Diolaiuti on 26/03/14.
//
//

#include "FriendViewCell.h"



USING_NS_CC;

FriendViewCell::FriendViewCell(int width, int height){

    
    _canvasLayer = CCLayerColor::create(ccc4(0, 0, 0, 0));
    _canvasLayer->setContentSize(CCSizeMake(width,height));
    _canvasLayer->setPosition(CCPointZero);
    
    _background = CCLayerColor::create(ccc4(33,43,112, 255));
    _background->setContentSize(CCSizeMake(width,height-10));
    _background->setPosition(ccp(0,5));
    _background->setAnchorPoint(CCPointZero);
    _canvasLayer->addChild(_background);
    
    
    _radioUnselected = CCSprite::create("buttons/radio.png");
    _radioUnselected->setAnchorPoint(ccp(0, 0.5));
    _radioUnselected->setPosition(ccp(10, _background->getContentSize().height/2));
    _background->addChild(_radioUnselected);
    
    _radioSelected = CCSprite::create("buttons/radio_select.png");
    _radioSelected->setAnchorPoint(ccp(0, 0.5));
    _radioSelected->setPosition(ccp(10, _background->getContentSize().height/2));
    _radioSelected->setVisible(false);
    _background->addChild(_radioSelected);

    
    
    this->addChild(_canvasLayer);
    
    _selected=false;
}

FriendViewCell::~FriendViewCell(){
    
}

void FriendViewCell::onEnter(){
    
}


void FriendViewCell::touched(){
    if (_selected==false){
        _background->setColor(ccc3(41,56,155));
        _radioUnselected->setVisible(false);
        _radioSelected->setVisible(true);
        _selected=true;
    }else{
        _background->setColor(ccc3(33,43,112));
        _radioSelected->setVisible(false);
        _radioUnselected->setVisible(true);
        _selected=false;
    }
}

#pragma mark Selection

bool FriendViewCell::isSelected(){
    return _selected;
}


void FriendViewCell::setSelected(bool selected){
    
}