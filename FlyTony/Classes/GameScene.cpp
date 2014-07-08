//
//  GameScene.cpp
//  TonyFlyingDream
//
//  Created by Giuseppe Lapenta on 18/02/14.
//
//

#include "GameScene.h"
#include "VisibleRect.h"
#include "Obstacle.h"
#include "SimpleAudioEngine.h"
#include "GKHWrapperCpp.h"
#include "PopUpMainMenu.h"
#include "PopUpRestartMenu.h"
#include "ProtocolAnalytics.h"

#define PTM_RATIO 32
#define GRAVITY 2500
#define DEFAULT_ROTATION_UP 20
#define DEFAULT_ROTATION_DOWN 90
#define FLOATING_GRAVITY_OPPOSITE GRAVITY - 5
#define FORCE_Y 650
#define FIXED_TIME_STEP 1/60


using namespace CocosDenshion;


enum
{
    kTagNewRecordLabel,
    kTagYouWinLabel,
};


enum
{
    kTagMainMenuButton = 216,
    kTagAcceptButton = 217,
    kTagRetryButton = 218,
};

static const float firstObstaclePosition = 400;
static const float distanceBetweenObstacles = 350.f;

static const char s_FontText[] = "fonts/text.fnt";
static const char s_FontScore[] = "fonts/score.fnt";



typedef enum
{
    LayerDrawingBackground,
    LayerDrawingMidground,
    LayerDrawingForeground,
    
}LayerDrawing;

typedef enum
{
    DrawingOrderGround,
    DrawingOrderClouds,
    DrawingOrderPipes,
    DrawingOrderGhost,
    DrawingOrderHero,
    
}DrawingOrder;



#pragma mark Constructor
GameScene::GameScene()
{
    
    GKHWrapperCpp gkh;
    gkh.setDelegate(this);
    
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE(_grounds);
    CC_SAFE_RELEASE(_clouds);
    CC_SAFE_RELEASE(_obstacles);
    CC_SAFE_RELEASE(_wheels);
    CC_SAFE_RELEASE(_tonyAnimation);
    CC_SAFE_RELEASE(_tonyGhostAnimation);
    CC_SAFE_RELEASE(_cloudBtnAnimation);
    delete _tracking;
    
    GKHWrapperCpp gkh;
    gkh.setDelegate(NULL);
    
}

CCScene* GameScene::scene(GameTrackingObjCpp obj)
{
    CCScene* scene = CCScene::create();
    
    GameScene* layer = GameScene::create(obj);
    
    scene->addChild(layer);
    
    return scene;
}

GameScene* GameScene::create(GameTrackingObjCpp obj)
{
    GameScene *pRet = new GameScene();
    if (pRet && pRet->init(obj))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

bool GameScene::init(GameTrackingObjCpp obj)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    int indexOfSpriteSheet = (CC_CONTENT_SCALE_FACTOR() == 2) ? 2 : 1;

    for (int i = 0; i < indexOfSpriteSheet; i++)
    {
    	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat("GameAtlas%d.plist",i)->getCString(),
       																		  CCString::createWithFormat("GameAtlas%d.png",i)->getCString());
    }
    
    _tracking= new GTOWrapperCpp();
    _tracking->clear();
    
    _isChallenge=false;
    _youWinLabelVisible=false;
    
    
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    _scrollSpeedBackground = 200.f;
    _scrollSpeedMidGround = 40;
    _scrollSpeedForeground = _scrollSpeedBackground;
    _yVel = 0;
    _yVelGhost =0;
    _isTap = false;
    _isGameOver = false;
    _isNewRecord = false;
    _timeWaitLabel = 0;
    _maxTimeWaitLabel = 1;
    _idSoundNewGroupScore = 10;
    _iterator=0;

    //Aggiungo gli ostacoli
    _obstacles  = CCArray::create();
    _obstacles->retain();
    
    _clouds = CCArray::create();
    _clouds->retain();
    
    _grounds = CCArray::create();
    _grounds->retain();
    
    _wheels = CCArray::create();
    _wheels->retain();
    

    CCSprite *fxBackground   = CCSprite::create("background.png");
    fxBackground->setAnchorPoint(ccp(0, 0));
    fxBackground->setPosition(ccp(0, 0));
    this->addChild(fxBackground);
    
    _background = CCNode::create();
    _background->setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    _background->setAnchorPoint(ccp(0, 0));
    this->addChild(_background,LayerDrawingBackground);
    
    _midground = CCNode::create();
    _midground->setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    _midground->setAnchorPoint(ccp(0, 0));
    this->addChild(_midground,LayerDrawingMidground);
    
    _foreground = CCNode::create();
    _foreground->setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    _foreground->setAnchorPoint(ccp(0, 0));
    this->addChild(_foreground,LayerDrawingForeground);
    
    //Animazione dell'uccellino
    _tonyAnimation = CCAnimation::create();
    _tonyAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tony_0001"));
    _tonyAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tony_0002"));
    _tonyAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tony_0003"));
    _tonyAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tony_0004"));
    _tonyAnimation->setDelayPerUnit(0.08f);
    
    _tonyAnimation->retain();
    
    //Animazione del Ghost
    _tonyGhostAnimation = CCAnimation::create();
    _tonyGhostAnimation->addSpriteFrameWithFileName("Ghost/tonyGhost_01.png");
    _tonyGhostAnimation->addSpriteFrameWithFileName("Ghost/tonyGhost_02.png");
    _tonyGhostAnimation->addSpriteFrameWithFileName("Ghost/tonyGhost_03.png");
    _tonyGhostAnimation->addSpriteFrameWithFileName("Ghost/tonyGhost_04.png");
    _tonyGhostAnimation->setDelayPerUnit(0.08f);
    _tonyGhostAnimation->retain();
    
    _cloudBtnAnimation = CCAnimation::create();
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0002"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0003"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0004"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0005"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0006"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0007"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0008"));
    _cloudBtnAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloudTap_0009"));

    _cloudBtnAnimation->setDelayPerUnit(0.08f);

    _cloudBtnAnimation->setRestoreOriginalFrame(true);
    
    _cloudBtnAnimation->retain();
    
    _layerColorWait = cocos2d::CCLayerColor::create(ccc4(9, 13, 73, 102), _screenSize.width, _screenSize.height);
    _layerColorWait->setAnchorPoint(ccp(0, 0));
    this->addChild(_layerColorWait,
                   LayerDrawingForeground+1);
    
    
    
    _recordRays = CCSprite::createWithSpriteFrameName("recordRays");
    _recordRays->setPosition(ccp(_screenSize.width/2, _screenSize.height - 50));
    this->addChild(_recordRays,LayerDrawingForeground);
    _recordRays->setOpacity(0);
    
    _newRecordLabel = CCLabelBMFont::create("New Record!", s_FontText);
    _newRecordLabel->setPosition(ccp(_screenSize.width/2, _screenSize.height + _newRecordLabel->getContentSize().height * 2));
    this->addChild(_newRecordLabel,LayerDrawingForeground,kTagNewRecordLabel);
    
    _youWinLabel = CCLabelBMFont::create("You Win!", s_FontText);
    _youWinLabel->setPosition(ccp(_screenSize.width/2, _screenSize.height + _youWinLabel->getContentSize().height * 2));
    this->addChild(_youWinLabel,LayerDrawingForeground,kTagYouWinLabel);

    
    PopUpMainMenu* m = PopUpMainMenu::create();
    _layerColorWait->addChild(m);
    m->setCallbackFunc(this, callfuncN_selector(GameScene::flyTony));
    
    
    this->drawLayout();
    
    
    return true;
}




#pragma mark CCLayer Method
void GameScene::onEnter()
{
    CCLayer::onEnter();
    

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Suoni/suono_menu.mp3", true);
    
    
}

void GameScene::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    GKHWrapperCpp gkh;
    gkh.setDelegate(NULL);
}

bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!_isGameOver)
    {
        _isTap = true;
        _player->stopActionByTag(10);
        
        CCAction* action = CCSequence::create(CCEaseExponentialOut::create(CCRotateTo::create(0.1, -DEFAULT_ROTATION_UP)),
                                              CCEaseExponentialOut::create(CCRotateTo::create(0.4, 0)),
                                              CCCallFunc::create(this, callfunc_selector(GameScene::finishRotateUp)),
                                              NULL);
        action->setTag(10);
        
        _player->runAction(action);
        
        CCAnimate* animate = CCAnimate::create(_tonyAnimation);
        _player->runAction(animate);
        
        int idSound = (rand()%5) +1;
        
        CCString* idSoundString = CCString::createWithFormat("Suoni/flap_%d.mp3",idSound);
        
        SimpleAudioEngine::sharedEngine()->preloadEffect(idSoundString->getCString());
        SimpleAudioEngine::sharedEngine()->playEffect(idSoundString->getCString());
        
    }
    if(_layerColorWait->isVisible())
        return false;
    else
        return true;
}

void GameScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!_isGameOver)
    {
        finishRotateUp();
    }
}


#pragma mark Draw Layout

void GameScene::drawLayout()
{
    this-> drawBackground();
    this-> drawScoreLabel();
    this-> drawPlayer();
    this->drawGhost();
}

void GameScene::drawBackground()
{
    // bottom
    _background1 = CCSprite::create("clouds_part1.png");
    _background1->setPosition(ccp(0, 0));
    _background1->setAnchorPoint(ccp(0, 0));
    _foreground->addChild(_background1,DrawingOrderGround);
    
    _background2 = CCSprite::create("clouds_part2.png");
    _background2->setPosition(ccp(_background1->getPositionX() + _background1->getContentSize().width, 0));
    _background2->setAnchorPoint(ccp(0, 0));
    _foreground->addChild(_background2,DrawingOrderGround);
    
    // top
    _backgroundTop1 = CCSprite::create("clouds_part1.png");
    _backgroundTop1->setPosition(ccp(0, this->getContentSize().height));
    _backgroundTop1->setAnchorPoint(ccp(0, 1));
    _backgroundTop1->setFlipY(true);
    
    _foreground->addChild(_backgroundTop1,DrawingOrderClouds);
    
    _backgroundTop2 = CCSprite::create("clouds_part2.png");
    _backgroundTop2->setPosition(ccp(_backgroundTop1->getPositionX() + _backgroundTop1->getContentSize().width, this->getContentSize().height));
    _backgroundTop2->setFlipY(true);
    _backgroundTop2->setAnchorPoint(ccp(0, 1));
    
    _foreground->addChild(_backgroundTop2,DrawingOrderClouds);
    
    _grounds->addObject(_background1);
    _grounds->addObject(_background2);
    
    _clouds->addObject(_backgroundTop1);
    _clouds->addObject(_backgroundTop2);
    
    // Wheel
    _wheelBg1 = CCSprite::create("wheelBg_01.png");
    _wheelBg1->setPosition(ccp(0, 84));
    _wheelBg1->setAnchorPoint(ccp(0, 0));
    _midground->addChild(_wheelBg1);
    
    _wheel = CCSprite::create("wheel.png");
    _wheel->setPosition(ccp(394, 242));
    _wheelBg1->addChild(_wheel, -1);
    _wheel->runAction(CCRepeatForever::create(CCRotateBy::create(8, 360)));
    
    _wheelBg2 = CCSprite::create("wheelBg_02.png");
    _wheelBg2->setPosition(ccp(_wheelBg1->getPositionX() + _wheelBg1->getContentSize().width, 84));
    _wheelBg2->setAnchorPoint(ccp(0, 0));
    _midground->addChild(_wheelBg2);
    
    _wheels->addObject(_wheelBg1);
    _wheels->addObject(_wheelBg2);
}


void GameScene::drawScoreLabel()
{
    _score = 0;
    
    _label = CCLabelBMFont::create(CCString::createWithFormat("%d",_score)->getCString(), s_FontScore);
    
    _label->setPosition(ccp(_screenSize.width/2, _screenSize.height/2 + 200));
    _label->setScale(2);
    
    this->addChild(_label,LayerDrawingForeground);
    _label->setVisible(false);
}

void GameScene::drawPlayer()
{
    
    _player = CCSprite::createWithSpriteFrameName("tony_0001");
    this->addChild(_player,DrawingOrderHero);
    _player->setPosition(ccp(200, 400));
    _player->setVisible(false);
}


void GameScene::drawGhost(){
    
    _ghost = CCSprite::create("Ghost/tonyGhost_01.png");
    _ghost->setOpacity(80);
    this->addChild(_ghost,DrawingOrderGhost);
    _ghost->setPosition(ccp(200, 400));
    _ghost->setVisible(false);
}


#pragma mark Init Game

void GameScene::flyTony(cocos2d::CCObject *obj)
{
    SimpleAudioEngine::sharedEngine()->preloadEffect("Suoni/pop.mp3");
    SimpleAudioEngine::sharedEngine()->playEffect("Suoni/pop.mp3");
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    CCMenuItem* item = (CCMenuItem*) obj;
    
    float btnX=_layerColorWait->getContentSize().width/2-130;
    float btnY=_layerColorWait->getContentSize().height/2-180;
    
    if (item->getUserData()!=NULL){
        GameTrackingObjCpp* gto = (struct GameTrackingObjCpp*) item->getUserData();
        _trackedInfo.seed=gto->seed;
        _trackedInfo.tapIteration=gto->tapIteration;
        _trackedInfo.score=gto->score;
        _tapVectorIterator=_trackedInfo.tapIteration->begin();
        _seedVectorIterator=_trackedInfo.seed->begin();
        _ghostDeathIteration=_trackedInfo.tapIteration->at(_trackedInfo.tapIteration->size()-1);
        _trackedInfo.tapIteration->pop_back();
        _isChallenge=true;
        _challengeWin=false;
        _obstacles->removeAllObjects();
    }
    if (_isChallenge) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Suoni/flytonychallenge.mp3", true);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    }else{
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Suoni/tonyfly.mp3", true);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    }
    
    this->spawnNewObstacle();
    this->spawnNewObstacle();
    this->spawnNewObstacle();

    CCMenuItemImage* btn = CCMenuItemImage::create();
    btn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
    btn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));

    _layerColorWait->removeAllChildrenWithCleanup(true);
    _layerColorWait->addChild(btn);
    
    btn->setPosition(ccp(btnX, btnY));
    btn->getNormalImage()->runAction(CCSequence::create(CCAnimate::create(_cloudBtnAnimation),
                                      CCCallFunc::create(this, callfunc_selector(GameScene::callbackFlyTony)),
                                      NULL));
}

void GameScene::callbackFlyTony()
{
    _player->setVisible(true);
    if(_isChallenge)
        _ghost->setVisible(true);
    _label->setVisible(true);
    _layerColorWait->setVisible(false);
    
    this->scheduleUpdate();
}


#pragma mark Update

void GameScene::update(float dt)
{
    _background->setPosition(ccp(_background->getPositionX() - (_scrollSpeedBackground *FIXED_TIME_STEP), _background->getPositionY()));
    _midground->setPosition(ccp(_midground->getPositionX() - (_scrollSpeedMidGround *FIXED_TIME_STEP), _midground->getPositionY()));
    _foreground->setPosition(ccp(_background->getPositionX() - (_scrollSpeedForeground *FIXED_TIME_STEP), _background->getPositionY()));
    
    _iterator++;
    
    
    //Ghost movement
    if(_isChallenge){
        if(_tapVectorIterator != _trackedInfo.tapIteration->end()){
            int endTap;
            if (_iterator==*_tapVectorIterator-1){
                
                _ghost->stopActionByTag(11);
            
                CCAction* action = CCSequence::create(CCEaseExponentialOut::create(CCRotateTo::create(0.1, -DEFAULT_ROTATION_UP)),
                                                      CCEaseExponentialOut::create(CCRotateTo::create(0.4, 0)),
                                                      CCCallFunc::create(this, callfunc_selector(GameScene::finishRotateUpGhost)),
                                                      NULL);
                action->setTag(11);
                _ghost->runAction(action);
            
                CCAnimate* animate = CCAnimate::create(_tonyGhostAnimation);
                _ghost->runAction(animate);
            }
            if(_iterator==*_tapVectorIterator){
                CCLOG("yvelghost %f",_yVelGhost);
                _yVelGhost = (FORCE_Y);
                endTap=*_tapVectorIterator+5;
                ++_tapVectorIterator;
            }
        
            _yVelGhost-= (0.016 * GRAVITY);
        
            if (_yVelGhost >=FORCE_Y)
            {
                _yVelGhost = FORCE_Y;
            
            }else if (_yVelGhost <= 2 * -GRAVITY)
            {
                _yVelGhost = 2 * -GRAVITY;
            }
        
            _ghost->setPositionY(_ghost->getPositionY() + (_yVelGhost * FIXED_TIME_STEP));
        
            if (_ghost->getPositionY() > VisibleRect::top().y + _ghost->getContentSize().height)
            {
                _yVelGhost -= 1.5*FORCE_Y;
                _ghost->setPositionY(_ghost->getPositionY() + (_yVelGhost * FIXED_TIME_STEP));
                finishRotateUpGhost();
            }
            
            if(_iterator==endTap){
                finishRotateUpGhost();
            }
        }
    
        if(_ghostDeathIteration==_iterator){
            _ghost->stopAllActions();
            _ghost->runAction(CCMoveTo::create(_ghost->getPositionY() / _screenSize.height, ccp(_ghost->getPositionX(), VisibleRect::bottom().y - _ghost->getContentSize().height)));
            _ghost->runAction(CCEaseExponentialIn::create(CCRotateTo::create(0.3, DEFAULT_ROTATION_DOWN)));
        }
        
    }
    
    
//Tony flying movement
    if (_isTap)
    {
        _yVel = (FORCE_Y);
        _isTap = false;
        _tracking->addTapIteration(_iterator);
    }

    _yVel -= (0.016 * GRAVITY);
    
    if (_yVel >= FORCE_Y)
    {
        _yVel = FORCE_Y;
        
    }else if (_yVel <= 2 * -GRAVITY)
    {
        _yVel = 2 * -GRAVITY;
    }
    
    _player->setPositionY(_player->getPositionY() + (_yVel * FIXED_TIME_STEP));

    if (_player->getPositionY() > VisibleRect::top().y + _player->getContentSize().height)
    {
        _yVel -= 1.5*FORCE_Y;
        _player->setPositionY(_player->getPositionY() + (_yVel * FIXED_TIME_STEP));
        this->ccTouchEnded(NULL, NULL);
    }
    else if (_player->getPositionY() < VisibleRect::bottom().y - _player->getContentSize().height)
    {
        _tracking->addTapIteration(_iterator);
        this->gameOver();
        this->unscheduleUpdate();
    }
    
    // Grounds Movement
    CCObject* obj;
    CCARRAY_FOREACH(_grounds, obj)
    {
        CCSprite* ground = (CCSprite*)obj;
        
        // get the world position of the ground
        CCPoint groundWorldPosition = _foreground->convertToWorldSpace(ground->getPosition());
        
        // get the screen position of the ground
        CCPoint groundScreenPosition = this->convertToNodeSpace(groundWorldPosition);
            
        // if the left corner is one complete width off the screen, move it to the right
        if (groundScreenPosition.x <= (-1 * ground->getContentSize().width))
        {
            ground->setPosition(ccp(ground->getPositionX() + 2 * ground->getContentSize().width, ground->getPositionY()));
        }
        
    }
    
    // Clouds Movement
    
    CCARRAY_FOREACH(_clouds, obj)
    {
        CCSprite* cloud = (CCSprite*)obj;
        
        // get the world position of the ground
        CCPoint cloudWorldPosition = _foreground->convertToWorldSpace(cloud->getPosition());
        
        // get the screen position of the ground
        CCPoint cloudScreenPosition = this->convertToNodeSpace(cloudWorldPosition);
        
        // if the left corner is one complete width off the screen, move it to the right
        if (cloudScreenPosition.x <= (-1 * cloud->getContentSize().width))
        {
            cloud->setPosition(ccp(cloud->getPositionX() + 2 * cloud->getContentSize().width, cloud->getPositionY()));
        }
        
    }
    
    // Wheel Movement
    
    CCARRAY_FOREACH(_wheels, obj)
    {
        CCSprite* wheelBg = (CCSprite*)obj;
        
        CCPoint wheelBgWorldPosition = _midground->convertToWorldSpace(wheelBg->getPosition());
        
        CCPoint wheelBgScreenPosition = this->convertToNodeSpace(wheelBgWorldPosition);
        
        if (wheelBgScreenPosition.x <= (-1 * wheelBg->getContentSize().width))
        {
            wheelBg->setPosition(ccp(wheelBg->getPositionX() + 2 * wheelBg->getContentSize().width, wheelBg->getPositionY()));
        }
    }
    
    CCArray* offScreenObstacles = NULL;
    
    CCARRAY_FOREACH(_obstacles, obj)
    {
        CCNode* obstacle = (CCNode* )obj;
        
        CCPoint obstacleWorldPosition = _foreground->convertToWorldSpace(obstacle->getPosition());
        CCPoint obstacleScreenPosition = this->convertToNodeSpace(obstacleWorldPosition);
        
        if (obstacleScreenPosition.x < -obstacle->getContentSize().width*2)
        {
            if (offScreenObstacles == NULL)
            {
                offScreenObstacles = CCArray::create();
            }
            
            offScreenObstacles->addObject(obstacle);
        }
    }

    CCARRAY_FOREACH(offScreenObstacles, obj)
    {
        CCNode* obstacleToRemove = (CCNode *)obj;
        obstacleToRemove->removeFromParentAndCleanup(true);
        _obstacles->removeObject(obstacleToRemove);
        
        this->spawnNewObstacle();
    }

    checkForCollision(dt);
}



#pragma mark Obstacles

void GameScene::spawnNewObstacle()
{
    CCNode* previousObstacle = (CCNode*)_obstacles->lastObject();
    float previousObstacleXPosition;

    if (previousObstacle == NULL)
    {
        // this is the first obstacle
        previousObstacleXPosition = firstObstaclePosition;
    }
    
    else
    {
        previousObstacleXPosition = previousObstacle->getPositionX();
    }
    
    CCPoint position = ccp(previousObstacleXPosition + distanceBetweenObstacles,0);
    
    Obstacle* obstacle = Obstacle::create();
    obstacle->setPosition(position);
    
    float seed;
    
    if(_isChallenge && _seedVectorIterator!=_trackedInfo.seed->end()){
        seed = *_seedVectorIterator;
        _seedVectorIterator++;
    }
    else{
        seed =rand()%INT64_MAX;
        
    }
    //register the seed
    _tracking->addSeed(seed);
    obstacle->setupRandomPosition(seed);
    
    _foreground->addChild(obstacle,DrawingOrderPipes);
    _obstacles->addObject(obstacle);
}


#pragma mark Collision

void GameScene::checkForCollision(float dt)
{
    _timeWaitLabel += dt;
    CollisionType type;
    
    CCObject* obj;
    CCARRAY_FOREACH(_obstacles, obj)
    {
        Obstacle* obstacle = (Obstacle *)obj;
        
        CCPoint obstacleWorldPosition = _foreground->convertToWorldSpace(obstacle->getPosition());
        CCPoint obstacleScreenPosition = this->convertToNodeSpace(obstacleWorldPosition);
        
        type = obstacle->checkCollisionWithPlayer(_player, obstacleScreenPosition);
        
        if (type == CollisionTypePipe)
        {
            _tracking->addTapIteration(_iterator);
            gameOver();
            this->unscheduleUpdate();
        }
        else if (type == CollisionTypeGoal)
        {
            if (_timeWaitLabel > _maxTimeWaitLabel)
            {
                int idSound = (rand() % 2) + 1;
                CCString* idSoundString = CCString::createWithFormat("Suoni/pass_%d.mp3",idSound);
                SimpleAudioEngine::sharedEngine()->preloadEffect(idSoundString->getCString());
                SimpleAudioEngine::sharedEngine()->playEffect(idSoundString->getCString());
                
                _timeWaitLabel = 0;
                updateScoreLabel();
            }
        }
    }
}

#pragma mark GameOver

void GameScene::gameOver()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    SimpleAudioEngine::sharedEngine()->preloadEffect("Suoni/splat.mp3");
    SimpleAudioEngine::sharedEngine()->playEffect("Suoni/splat.mp3");
    _isGameOver = true;
    
    _label->setVisible(false);
    
    _ghost->setVisible(false);
    
    this->hideNewRecordLabel();
    _player->stopAllActions();
    _player->runAction(CCMoveTo::create(_player->getPositionY() / _screenSize.height, ccp(_player->getPositionX(), VisibleRect::bottom().y - _player->getContentSize().height)));
    _player->runAction(CCEaseExponentialIn::create(CCRotateTo::create(0.3, DEFAULT_ROTATION_DOWN)));
    
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(255, 255, 255, 255), _screenSize.width, _screenSize.height);
    this->addChild(layer,LayerDrawingForeground);
    layer->setOpacity(0);
    
    layer->runAction(CCSequence::create(CCFadeIn::create(0.1),
                                        CCFadeOut::create(0.1),
                                        CCRemoveSelf::create(),
                                        NULL));
    
    float offset = 3;
    
    CCPoint initialPosition = this->getPosition();
    
    CCMoveTo* moveUpL = CCMoveTo::create(0.02, ccp(this->getPositionX()-offset, this->getPositionY()+offset));
    CCMoveTo* moveUpR = CCMoveTo::create(0.02, ccp(this->getPositionX()+offset, this->getPositionY()+offset));
    CCMoveTo* moveDownL = CCMoveTo::create(0.02, ccp(this->getPositionX() - offset, this->getPositionY() - offset));
    CCMoveTo* moveDownR = CCMoveTo::create(0.02, ccp(this->getPositionX() + offset, this->getPositionY() - offset));
    
    this->runAction(CCSequence::create(moveUpL,
                                       moveUpR,
                                       moveDownR,
                                       moveDownL,
                                       moveDownR,
                                       moveUpL,
                                       moveDownL,
                                       moveUpR,
                                       moveUpL,
                                       moveUpR,
                                       moveDownR,
                                       moveDownL,
                                       moveDownR,
                                       moveUpL,
                                       moveDownL,
                                       moveUpR,
                                       moveDownR,
                                       moveDownL,
                                       moveDownR,
                                       moveUpL,
                                       moveDownL,
                                       moveUpR,
                                       CCPlace::create(initialPosition),
                                       CCCallFunc::create(this, callfunc_selector(GameScene::callbackGameOver)),
                                       NULL));
    
    
    
}

void GameScene::callbackGameOver()
{
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Suoni/suono_menu.mp3", true);
    
    
    sendScore(_score);
    _tracking->addScore(_score);
    
    
    _layerColorWait->removeAllChildrenWithCleanup(true);
    _layerColorWait->setVisible(true);
    
    PopUpRestartMenu* restartMenu;
    if(_isChallenge){
        restartMenu = PopUpRestartMenu::createWithChallenge(_score, _isNewRecord, _challengeWin);
        _layerColorWait->addChild(restartMenu);
        restartMenu->setCallbackFunc(this, callfuncN_selector(GameScene::restart));
        _isChallenge=false;
    }
    else{
        restartMenu = PopUpRestartMenu::create(_score, _isNewRecord);
        _layerColorWait->addChild(restartMenu);
        restartMenu->setCallbackFunc(this, callfuncN_selector(GameScene::restart));
    }
    
}


void GameScene::restart(cocos2d::CCObject *obj)
{
    CCMenuItem* object = (CCMenuItem*) obj;

    //Callback from MainMenu or Challenge Buttons
    if (object->getTag()==kTagMainMenuButton || object->getTag()==kTagAcceptButton) {
        _yVel = 0;
        _yVelGhost =0;
        _isTap = false;
        _isGameOver = false;
        _isNewRecord = false;
        _timeWaitLabel = 0;
        _score = 0;
        _idSoundNewGroupScore = 10;
        _label->setVisible(false);
        _label->setString("0");
        
        _layerColorWait->removeAllChildrenWithCleanup(true);
        _background->setPosition(CCPointZero);
        _background1->setPosition(CCPointZero);
        _background2->setPosition(ccp(_background1->getPositionX() + _background1->getContentSize().width, 0));
        _backgroundTop1->setPosition(ccp(0, this->getContentSize().height));
        _backgroundTop2->setPosition(ccp(_backgroundTop1->getPositionX() + _backgroundTop1->getContentSize().width, this->getContentSize().height));
        
        _iterator=0;
        
        _midground->setPosition(CCPointZero);
        _foreground->setPosition(CCPointZero);
        
        CCObject* o;
        
        CCARRAY_FOREACH(_obstacles, o)
        {
            CCNode* obstacleToRemove = (CCNode *)o;
            obstacleToRemove->removeFromParentAndCleanup(true);
        }
        
        _obstacles->removeAllObjects();
        
        _player->setPosition(ccp(200, 400));
        _player->setRotation(0);
        _player->setVisible(false);
        
        _ghost->setPosition(ccp(200, 400));
        _ghost->setRotation(0);
        _ghost->setVisible(false);
        
        _tracking->clear();
        
        _layerColorWait->removeAllChildrenWithCleanup(true);
        PopUpMainMenu* mainMenu = PopUpMainMenu::create();
        _layerColorWait->addChild(mainMenu);
        mainMenu->setCallbackFunc(this, callfuncN_selector(GameScene::flyTony));
    }
    else if (object->getTag()==kTagRetryButton) {
        CCMenuItemImage* btn = CCMenuItemImage::create();
        btn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        btn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        
        _layerColorWait->removeAllChildrenWithCleanup(true);
        _layerColorWait->addChild(btn);
        
        //Reset game info
        _tapVectorIterator=_trackedInfo.tapIteration->begin();
        _seedVectorIterator=_trackedInfo.seed->begin();
        _isChallenge=true;
        _challengeWin=false;
        
        SimpleAudioEngine::sharedEngine()->playEffect("Suoni/pop.mp3");
        btn->setPosition(ccp(_layerColorWait->getContentSize().width/2-130, _layerColorWait->getContentSize().height/2+130));
        btn->getNormalImage()->runAction(CCSequence::create(
                                                            CCAnimate::create(_cloudBtnAnimation),
                                                            CCCallFunc::create(this, callfunc_selector(GameScene::callbackRestart)),
                                                            NULL));
    }
    //Callback from restart button
    else{
        CCMenuItemImage* btn = CCMenuItemImage::create();
        btn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        btn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
    
        _layerColorWait->removeAllChildrenWithCleanup(true);
        _layerColorWait->addChild(btn);
    
        SimpleAudioEngine::sharedEngine()->playEffect("Suoni/pop.mp3");
        btn->setPosition(ccp(_layerColorWait->getContentSize().width/2-130, _layerColorWait->getContentSize().height/2+130));
        btn->getNormalImage()->setTag(object->getTag());
        btn->getNormalImage()->runAction(CCSequence::create(
                                                        CCAnimate::create(_cloudBtnAnimation),
                                                        CCCallFunc::create(this, callfunc_selector(GameScene::callbackRestart)),
                                                        NULL));

    }
}


void GameScene::callbackRestart(CCObject *obj)
{
    _yVel = 0;
    _yVelGhost =0;
    _isTap = false;
    _isGameOver = false;
    _isNewRecord = false;
    _timeWaitLabel = 0;
    _score = -1;
    _idSoundNewGroupScore = 10;
    _label->setVisible(true);
    _label->setString("0");

    _layerColorWait->removeAllChildrenWithCleanup(true);
    _layerColorWait->setVisible(false);
    _background->setPosition(CCPointZero);
    _background1->setPosition(CCPointZero);
    _background2->setPosition(ccp(_background1->getPositionX() + _background1->getContentSize().width, 0));
    _backgroundTop1->setPosition(ccp(0, this->getContentSize().height));
    _backgroundTop2->setPosition(ccp(_backgroundTop1->getPositionX() + _backgroundTop1->getContentSize().width, this->getContentSize().height));
    
    
    _iterator=0;



    _midground->setPosition(CCPointZero);
    _foreground->setPosition(CCPointZero);
    
    CCObject* object;

    CCARRAY_FOREACH(_obstacles, object)
    {
        CCNode* obstacleToRemove = (CCNode *)object;
        obstacleToRemove->removeFromParentAndCleanup(true);
    }

    _obstacles->removeAllObjects();
    
    _player->setPosition(ccp(200, 400));
    _player->setRotation(0);
    _player->setVisible(true);
    
    _ghost->setPosition(ccp(200, 400));
    _ghost->setRotation(0);
    
    _tracking->clear();
    
    if (_isChallenge) {
        _ghost->setVisible(true);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Suoni/flytonychallenge.mp3", true);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    }else{
        _ghost->setVisible(false);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Suoni/tonyfly.mp3", true);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    }
    
    this->spawnNewObstacle();
    this->spawnNewObstacle();
    this->spawnNewObstacle();
    
    this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.8),
                                                     CCCallFunc::create(this, callfunc_selector(GameScene::resetScene))));
}

#pragma mark Reset

void GameScene::resetScene()
{
    this->updateScoreLabel();
    this->scheduleUpdate();
}


#pragma mark Label

void GameScene::updateScoreLabel()
{
    _score++;
    
    if (_score > 0 && _score%10 == 0)
    {
        CCString* idSoundString = CCString::createWithFormat("Suoni/%d.mp3",_idSoundNewGroupScore);
        
        SimpleAudioEngine::sharedEngine()->playEffect(idSoundString->getCString());
        
        _idSoundNewGroupScore++;
        
        if (_idSoundNewGroupScore > 34)
        {
            _idSoundNewGroupScore = 34;
        }
    }
    
    if(_isChallenge && _score>=_trackedInfo.score){
        _youWinLabelVisible=true;
        showYouWinLabel();
    }
    if (_score > atoi(CCUserDefault::sharedUserDefault()->getStringForKey("currentScore", "").c_str()))
    {
        if (!_youWinLabelVisible) {
            showNewRecordLabel();
        }else{
            _isNewRecord=true;
        }
    
        CCUserDefault::sharedUserDefault()->setStringForKey("currentScore", CCString::createWithFormat("%d",_score)->getCString());
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    _label->setString(CCString::createWithFormat("%d",_score)->getCString());
}

void GameScene::showNewRecordLabel()
{
    if (!_isNewRecord)
    {
        _isNewRecord = true;
        
        _newRecordLabel->runAction(CCEaseBackOut::create(CCMoveTo::create(0.5, ccp(_screenSize.width/2, _screenSize.height - 80))));
        _recordRays->runAction(CCFadeIn::create(0.5));
        _recordRays->runAction(CCRepeatForever::create(CCRotateBy::create(8, -360)));
        
        this->runAction(CCSequence::create(CCDelayTime::create(3),
                                           CCCallFunc::create(this, callfunc_selector(GameScene::hideNewRecordLabel)),
                                           NULL));
        
        SimpleAudioEngine::sharedEngine()->preloadEffect("Suoni/new_record.mp3");
        SimpleAudioEngine::sharedEngine()->playEffect("Suoni/new_record.mp3");
    }
}

void GameScene::hideNewRecordLabel()
{
    _newRecordLabel->runAction(CCEaseBackIn::create(CCMoveTo::create(0.5, ccp(_screenSize.width/2, _screenSize.height + _newRecordLabel->getContentSize().height * 2))));
    
    _recordRays->stopAllActions();
    
    if (_recordRays->getOpacity() > 0)
    {
        _recordRays->runAction(CCFadeOut::create(0.5));
    }
    _youWinLabelVisible=false;
}

void GameScene::showYouWinLabel()
{
    if (!_challengeWin)
    {
        
        _challengeWin = true;
        
        _youWinLabel->runAction(CCEaseBackOut::create(CCMoveTo::create(0.5, ccp(_screenSize.width/2, _screenSize.height - 80))));
        _recordRays->runAction(CCFadeIn::create(0.5));
        _recordRays->runAction(CCRepeatForever::create(CCRotateBy::create(8, -360)));
        
        this->runAction(CCSequence::create(CCDelayTime::create(3),
                                           CCCallFunc::create(this, callfunc_selector(GameScene::hideYouWinLabel)),
                                           NULL));
        
        SimpleAudioEngine::sharedEngine()->preloadEffect("Suoni/new_record.mp3");
        SimpleAudioEngine::sharedEngine()->playEffect("Suoni/new_record.mp3");
    }
}

void GameScene::hideYouWinLabel()
{
    _youWinLabel->runAction(CCEaseBackIn::create(CCMoveTo::create(0.5, ccp(_screenSize.width/2, _screenSize.height + _newRecordLabel->getContentSize().height * 2))));
    
    _recordRays->stopAllActions();
    
    if (_recordRays->getOpacity() > 0)
    {
        _recordRays->runAction(CCFadeOut::create(0.5));
    }
}


#pragma mark Ad Banner and Game Center

void GameScene::bannerPressed(cocos2d::CCObject *obj)
{
    //NdkHelper
    SendMessageWithParams(string("openApp"),NULL);
}

void GameScene::sendScore(int score){
    GKHWrapperCpp gkh;
    gkh.submitScoreToCatagory(score, "eu.balzo.tonyfly.leaderboard");
}



#pragma mark Movement

void GameScene::finishRotateUp()
{
    _player->stopActionByTag(10);
    CCAction* action = CCEaseExponentialIn::create(CCRotateTo::create(0.6, DEFAULT_ROTATION_DOWN));
    action->setTag(10);
    _player->runAction(action);
}

void GameScene::finishRotateUpGhost(){
    _ghost->stopActionByTag(11);
    CCAction* action = CCEaseExponentialIn::create(CCRotateTo::create(0.6, DEFAULT_ROTATION_DOWN));
    action->setTag(11);
    _ghost->runAction(action);
}
