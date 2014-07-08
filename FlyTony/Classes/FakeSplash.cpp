//
//  FakeSplash.cpp
//  Kroms
//
//  Created by Giuseppe Lapenta on 23/11/13.
//
//

#include "FakeSplash.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

FakeSplash::FakeSplash()
{
    
}

FakeSplash::~FakeSplash()
{
    
}

CCScene* FakeSplash::scene()
{
    CCScene* scene = CCScene::create();
    
    FakeSplash* splash = FakeSplash::create();
    
    scene->addChild(splash);
    
    return scene;
}


bool FakeSplash::init()
{
    
    if (!CCLayer::init())
    {
        return false;
    }

    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg = CCSprite::create("splashScreen.png");
    bg->setPosition(ccp(screenSize.width/2, screenSize.height/2));
    bg->setOpacity(0);
    addChild(bg);
    
    CCSequence* seq = CCSequence::create(
                                         CCDelayTime::create(0.1),
                                         CCFadeIn::create(0.2f),
                                         CCDelayTime::create(0.5f),
                                         CCCallFuncN::create(this, callfuncN_selector(FakeSplash::showInitialMenu)),
                                         NULL
                                         );
    
    bg->runAction(seq);


    return true;
}


void FakeSplash::showInitialMenu(CCNode *node)
{
    node->removeFromParentAndCleanup(true);
    GameTrackingObjCpp vuoto;
    
    vuoto.tapIteration=NULL;
    vuoto.tapIteration=NULL;
    CCScene* scene = GameScene::scene(vuoto);
    CCDirector::sharedDirector()->replaceScene(scene);

}

