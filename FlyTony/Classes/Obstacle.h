//
//  Obstacle.h
//  TonyFlyingDream
//
//  Created by Giuseppe Lapenta on 19/02/14.
//
//

#ifndef __TonyFlyingDream__Obstacle__
#define __TonyFlyingDream__Obstacle__

#include <iostream>
#include "cocos2d.h"
#include "GameScene.h"


USING_NS_CC;

typedef enum
{
    CollisionTypeNone,
    CollisionTypeGoal,
    CollisionTypePipe,
    
}CollisionType;


class Obstacle : public CCNode
{
    
    CCSize _screenSize;
    
    CCSprite* _topPipe;
    CCSprite* _bottomPipe;
    CCNode*   _goal;
    
    GameScene* _game;
    
public:
    
    Obstacle();
    virtual ~Obstacle();
    
    bool init();
    
    void setupRandomPosition(float seed);

    CREATE_FUNC(Obstacle);
    
    CollisionType checkCollisionWithPlayer(CCSprite * sprite, const CCPoint& point);
    
};

#endif /* defined(__TonyFlyingDream__Obstacle__) */
