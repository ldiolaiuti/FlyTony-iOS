//
//  Obstacle.cpp
//  TonyFlyingDream
//
//  Created by Giuseppe Lapenta on 19/02/14.
//
//

#include "Obstacle.h"
#include "VisibleRect.h"

#define PTM_RATIO 32


// visibility on a 3,5-inch iPhone ends a 88 points and we want some meat
static const float minimumYPositionTopPipe = 80;

// visibility ends at 480 and we want some meat
static const float maximumYPositionBottomPipe = 700.f;

// distance between top and bottom pipe
static const float pipeDistance = 180.0f;

// calculate the end of the range of top pipe
static const float maximumYPositionTopPipe = maximumYPositionBottomPipe - pipeDistance;


Obstacle::Obstacle()
{
    
}

Obstacle::~Obstacle()
{
    
}




bool Obstacle::init()
{
    if (!CCNode::init())
    {
        return false;
    }
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    this->setContentSize(CCSizeMake(80, 640));
    this->setAnchorPoint(ccp(0, 0));
    
    _topPipe = CCSprite::create("obstacleTopUp.png");
    _topPipe->setAnchorPoint(ccp(0.5, 0));
    _topPipe->setPosition(ccp(this->getContentSize().width/2,0));
    
    CCSprite* basePipeUp = CCSprite::create("obstacleBase.png");
    basePipeUp->setAnchorPoint(ccp(0.5f, 0));
    basePipeUp->setPosition(ccp(_topPipe->getContentSize().width/2, _topPipe->getContentSize().height));
    basePipeUp->setScaleY(20);
    
    _topPipe->addChild(basePipeUp);
    
    _bottomPipe = CCSprite::create("obstacleTopDown.png");
    _bottomPipe->setAnchorPoint(ccp(0.5, 1.0));
    _bottomPipe->setPositionX(this->getContentSize().width/2);
    
    CCSprite* basePipeDown = CCSprite::create("obstacleBase.png");
    basePipeDown->setAnchorPoint(ccp(0.5f, 1.0));
    basePipeDown->setPosition(ccp(_bottomPipe->getContentSize().width/2,0));
    basePipeDown->setScaleY(200);

    _bottomPipe->addChild(basePipeDown);

    _goal = CCNode::create();
    _goal->setContentSize(CCSizeMake(20, 640));
    _goal->setAnchorPoint(ccp(0, 0));
    _goal->setPosition(ccp(this->getContentSize().width/2 - _goal->getContentSize().width/2, 0));
    
    this->addChild(_topPipe);
    this->addChild(_bottomPipe);
    this->addChild(_goal);
    
    return true;
}


void Obstacle::setupRandomPosition(float random)
{
    float range = maximumYPositionTopPipe - minimumYPositionTopPipe;
    
    _topPipe->setPosition(ccp(_topPipe->getPositionX(), _screenSize.height - (minimumYPositionTopPipe + (random * range))));
    
    _bottomPipe->setPosition(ccp(_bottomPipe->getPositionX(), _topPipe->getPositionY() - pipeDistance));

}

CollisionType Obstacle::checkCollisionWithPlayer(CCSprite *sprite, const CCPoint& point)
{
    CollisionType type = CollisionTypeNone;
    
    CCRect bottomPipeRect = CCRectMake(
                                       point.x,
                                       point.y,
                                       _bottomPipe->getContentSize().width/2,
                                       _bottomPipe->getPositionY());
    
    CCRect topPipeRect = CCRectMake(
                                       point.x,
                                       _topPipe->getPositionY(),
                                       _topPipe->getContentSize().width/2,
                                       _screenSize.height);
    
    CCRect goalRect = CCRectMake(
                                       point.x,
                                       point.y,
                                       _goal->getContentSize().width,
                                        VisibleRect::getVisibleRect().size.height);
    
    
    CCRect spriteBoundingBox = CCRectMake(sprite->getPositionX() - sprite->getContentSize().width/4,
                                          sprite->getPositionY() - sprite->getContentSize().height/4,
                                          sprite->getContentSize().width/2,
                                          sprite->getContentSize().height/2);
    
    if (bottomPipeRect.intersectsRect(spriteBoundingBox))
    {
        type = CollisionTypePipe;
    }
    else if (topPipeRect.intersectsRect(spriteBoundingBox))
    {
        type = CollisionTypePipe;
    }
    else if(goalRect.intersectsRect(spriteBoundingBox))
    {
        type = CollisionTypeGoal;
    }
    
    return type;
}


