//
//  GameScene.h
//  TonyFlyingDream
//
//  Created by Giuseppe Lapenta on 18/02/14.
//
//

#ifndef __TonyFlyingDream__GameScene__
#define __TonyFlyingDream__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "NDKHelper/NDKHelper.h"
#include "GKHDelegate.h"
#include "GTODelegate.h"
#include "GTOWrapperCpp.h"

USING_NS_CC;

class GameScene : public CCLayer, public GKHDelegate, public GTODelegate
{
 
    CCSize          _screenSize;
    CCNode   *      _background;
    CCNode   *      _midground;
    CCNode   *      _foreground;
    CCSprite *      _player;
    CCSprite *      _ghost;
    CCSprite *      _background1;
    CCSprite *      _background2;
    CCSprite *      _backgroundTop1;
    CCSprite *      _backgroundTop2;
    CCSprite *      _wheelBg1;
    CCSprite *      _wheelBg2;
    CCSprite *      _wheel;
    CCSprite *      _recordRays;
    CCLabelBMFont*  _label;
    CCLabelBMFont*  _newRecordLabel;
    CCLabelBMFont*  _youWinLabel;
    CCLayerColor*   _layerColorWait;
    CCMenu*         _menu;
    GTOWrapperCpp*  _tracking;
    GameTrackingObjCpp _trackedInfo;
    vector<int>::iterator   _tapVectorIterator;
    vector<double>::iterator _seedVectorIterator;
    
    
    int             _score;
    int             _iterator;
    int             _ghostDeathIteration;
    int             _idSoundNewGroupScore;
    float           _yVel;
    float           _yVelGhost;
    float           _scrollSpeedBackground;
    float           _scrollSpeedMidGround;
    float           _scrollSpeedForeground;
    bool            _isTap;
    bool            _isNewRecord;
    bool            _isGameOver;
    float           _timeWaitLabel;
    float           _maxTimeWaitLabel;
    bool            _isChallenge;
    bool            _challengeWin;
    bool            _youWinLabelVisible;
    
    CCArray *       _grounds;
    CCArray *       _clouds;
    CCArray *       _obstacles;
    CCArray *       _wheels;
    CCAnimation*    _tonyAnimation;
    CCAnimation*    _tonyGhostAnimation;
    CCAnimation*    _cloudBtnAnimation;

    // Draw Layout
    void drawLayout();
    void drawBackground();
    void drawScoreLabel();
    void drawPlayer();
    void drawGhost();
    
    // Init
    void flyTony(CCObject * obj);
    void callbackFlyTony();
    
    //New Obstacle
    void spawnNewObstacle();
    
    //GameOver
    void gameOver();
    void callbackGameOver();
    void restart(CCObject *obj);
    void callbackRestart(CCObject *obj);
    
    //Reset
    void resetScene();
    
    
    // Collision
    void checkForCollision(float dt);
    
    //Record, challenge Label
    void updateScoreLabel();
    void showNewRecordLabel();
    void hideNewRecordLabel();
    void showYouWinLabel();
    void hideYouWinLabel();
    
    //Banner
    void bannerPressed(CCObject * obj);
    
    //Score
    void sendScore(int score);
    
    //Ghost movement
    void finishRotateUpGhost();
    void finishRotateUp();
        
public:
    
    GameScene();
    virtual ~GameScene();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void update(float dt);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);



    
    static CCScene* scene(GameTrackingObjCpp obj);
    
    
    
    static GameScene* create(GameTrackingObjCpp obj);
    bool init(GameTrackingObjCpp obj);

    
    
};
#endif /* defined(__TonyFlyingDream__GameScene__) */
