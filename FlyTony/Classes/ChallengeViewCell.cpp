//
//  ChallengeViewCell.cpp
//  FlyTony
//
//  Created by Luca Diolaiuti on 22/04/14.
//
//

#include "ChallengeViewCell.h"

static const char s_FontScore[] = "fonts/score.fnt";



ChallengeViewCell::ChallengeViewCell(int width, int height, char* name, char* message){
    
    
    _canvasLayer = CCLayer::create();
    _canvasLayer->setContentSize(CCSizeMake(width,height));
    _canvasLayer->setPosition(CCPointZero);
    
    _background = CCLayerColor::create(ccc4(33, 43, 112, 255));
    _background->setContentSize(CCSizeMake(width,113));
    _background->setPosition(ccp(0,25));
    _background->setAnchorPoint(CCPointZero);
    _canvasLayer->addChild(_background);
    
    _messageBox = CCSprite::create("baloon.png");
    _messageBox->setAnchorPoint(ccp(0.5,0.5));
    _messageBox->setPosition(ccp(_background->getContentSize().width/2, 75));
    _background->addChild(_messageBox);
    
    _acceptedLayer = CCLayerColor::create(ccc4(86,51,113,255));
    _acceptedLayer->setContentSize(CCSizeMake(200, 60));
    _acceptedLayer->setAnchorPoint(ccp(0,0));
    _acceptedLayer->setPosition(ccp(220, 5));
    _accept = CCLabelBMFont::create("Accept", s_FontScore);
    _accept->setAnchorPoint(CCPointZero);
    _accept->setPosition(42, 6);
    _acceptedLayer->addChild(_accept);
    _canvasLayer->addChild(_acceptedLayer);
    
    this->addChild(_canvasLayer);
    
    _selected=false;
}

ChallengeViewCell::~ChallengeViewCell(){}

void ChallengeViewCell::onEnter(){
    
}



#pragma mark Selection


void ChallengeViewCell::touched(){
    if (_selected==false){
        _background->setColor(ccGREEN);
        _selected=true;
    }else{
        _background->setColor(ccc3(0, 0, 128));
        _selected=false;
    }
}

bool ChallengeViewCell::isSelected(){
    return _selected;
}


void ChallengeViewCell::setSelected(bool selected){
    
}