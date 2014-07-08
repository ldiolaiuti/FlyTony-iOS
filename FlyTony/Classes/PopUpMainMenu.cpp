//
//  PopUpMainMenu.cpp
//  FlyTony
//
//  Created by Luca Diolaiuti on 17/04/14.
//
//

#include "PopUpMainMenu.h"
#include "SimpleAudioEngine.h"
#import "GKHWrapperCpp.h"


using namespace CocosDenshion;

static const int kForeground = 7;


static const char s_FontText[] = "fonts/text.fnt";
static const char s_FontScore[] = "fonts/score.fnt";

enum {
	kTagStencilNode = 105,
	kTagClipperNode = 106,
    kTagFirstBgLayer = 107,
    kTagSecondBgLayer = 108,
    kTagMainLayer   = 109,
    kTagLayerChallenge = 110,
    kTagChallengeMenu =111,
    kTagMainMenu = 112,
    kTagBackMenu = 113,
    kTagFlyButton = 114,
    kTagChallengeButton = 115,
};

#pragma mark Constructor

PopUpMainMenu::PopUpMainMenu(){
    
//If platform iOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GKHWrapperCpp gkh;
    gkh.setDelegate(this);
    
    GTOWrapperCpp gto;
    gto.setDelegate(this);
    
    
    
    _challenges.clear();
#endif
    
}


PopUpMainMenu::~PopUpMainMenu(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GKHWrapperCpp gkh;
    gkh.setDelegate(NULL);
    
    
    GTOWrapperCpp gto;
    gto.setDelegate(NULL);
#endif
    
}


PopUpMainMenu* PopUpMainMenu::create()
{
    PopUpMainMenu* popUpLayer = new PopUpMainMenu();
    
    if (popUpLayer && popUpLayer->PopUpMainMenu::init())
    {
        popUpLayer->autorelease();
        return popUpLayer;
    }
    CC_SAFE_DELETE(popUpLayer);
    return NULL;
}


bool PopUpMainMenu::init(){
    if(!CCLayer::init())
    {
        return false;
    }

    this->setTouchEnabled(true);
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    // Add background layers to the scene.
    CCLayerColor *bg = CCLayerColor::create(ccc4(70, 31, 90, 0));
    bg->setPosition(CCPointZero);
    this->addChild(bg,kForeground,kTagFirstBgLayer);
    
    //Add Layer For movement elements
    float width = 411;
    float height = 520;
    
    // Clipper for others layers
    CCClippingNode *clipper = CCClippingNode::create();
    clipper->setTag( kTagClipperNode );
    clipper->setContentSize(CCSizeMake(width, height));
    clipper->setAnchorPoint(ccp(0.5,0));
    clipper->setPosition(ccp(screenSize.width/2, 180));
    this->addChild(clipper,kForeground+1);
    
    CCDrawNode *stencil = CCDrawNode::create();
    CCPoint rectangle[4];
    rectangle[0] = ccp(0, 0);
    rectangle[1] = ccp(clipper->getContentSize().width, 0);
    rectangle[2] = ccp(clipper->getContentSize().width, clipper->getContentSize().height);
    rectangle[3] = ccp(0, clipper->getContentSize().height);
    
    ccColor4F white = {1, 1, 1, 1};
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clipper->setStencil(stencil);
    
    // Layer clipped
    CCLayerColor *secondBg = CCLayerColor::create(ccc4(70, 31, 90, 0));
    secondBg->setContentSize(CCSizeMake(2*width, height));
    secondBg->setAnchorPoint(CCPointZero);
    secondBg->setPosition(CCPointZero);
    
    clipper->addChild(secondBg,kForeground,kTagSecondBgLayer);
    
    
    //Layer con spinner per attesa
    _layerColorWait = cocos2d::CCLayerColor::create(ccc4(9, 13, 73, 102), screenSize.width, screenSize.height);
    _layerColorWait->setAnchorPoint(ccp(0, 0));
    _layerColorWait->setVisible(false);
    _spinner = CCSprite::create("spinner.png");
    _spinner->setAnchorPoint(ccp(0.5,0.5));
    _spinner->setPosition(ccp(_layerColorWait->getContentSize().width/2, _layerColorWait->getContentSize().height/2));
    _layerColorWait->addChild(_spinner);
    this->addChild(_layerColorWait, 25);
    
    
    //Layer errore
    _errorLayer = CCLayerColor::create(ccc4(9, 13, 73, 102), screenSize.width, screenSize.height);
    _errorLayer->setAnchorPoint(CCPointZero);
    _errorLayer->setVisible(false);
    CCLayerColor* errorBackground = CCLayerColor::create(ccc4(33, 43, 112, 255), 460, 600);
    errorBackground->setAnchorPoint(ccp(0, 0));
    errorBackground->setPosition(ccp(_errorLayer->getContentSize().width/2-230, _errorLayer->getContentSize().height/2-300));
    _errorLayer->addChild(errorBackground);
    CCSprite* tonyPopupFail = CCSprite::create("tonyPopupFail.png");
    tonyPopupFail->setAnchorPoint(ccp(0.5,0));
    tonyPopupFail->setPosition(ccp(230, 340));
    errorBackground->addChild(tonyPopupFail);
    CCLabelBMFont* connectionFailed = CCLabelBMFont::create("CONNESSIONE FALLITA", s_FontScore);
    connectionFailed->setAnchorPoint(ccp(0.5, 0));
    connectionFailed->setScale(1.2);
    connectionFailed->setPosition(ccp(230,500));
    errorBackground->addChild(connectionFailed);
    CCLabelBMFont* connectionError = CCLabelBMFont::create("Si è presentato un errore", s_FontScore);
    connectionError->cocos2d::CCNode::setAnchorPoint(ccp(0.5,0));
    connectionError->setPosition(ccp(230,220));
    errorBackground->addChild(connectionError);
    CCLabelBMFont* tryLater = CCLabelBMFont::create("\nRiprova più tardi",s_FontScore);
    tryLater->cocos2d::CCNode::setAnchorPoint(ccp(0.5,0));
    tryLater->setPosition(ccp(230,180));
    errorBackground->addChild(tryLater);
    
    CCMenuItemImage* okButton = CCMenuItemImage::create();
    okButton->setAnchorPoint(CCPointZero);
    CCScale9Sprite* okButtonImg= CCScale9Sprite::create("buttons/violet_bg.png");
    okButtonImg->setContentSize(CCSizeMake(264, 100));
    okButtonImg->setAnchorPoint(ccp(0,0));
    okButtonImg->setPosition(ccp(okButton->getContentSize().width, okButton->getContentSize().height));
    okButton->addChild(okButtonImg);
    okButton->setContentSize(CCSizeMake(264, 100));
    okButton->setPosition(ccp(98, 50));
    okButton->setTarget(this, menu_selector(PopUpMainMenu::callbackErrorLayer));
    CCLabelBMFont* okButtonLabel = CCLabelBMFont::create("OK", s_FontScore);
    okButton->setAnchorPoint(ccp(0,0));
    okButtonLabel->setPosition(ccp(okButton->getContentSize().width*0.5, okButton->getContentSize().height*0.5-6));
    okButtonLabel->setScale(1.5);
    okButton->addChild(okButtonLabel);
    CCMenu* errorLayerMenu = CCMenu::create(okButton, NULL);
    errorLayerMenu->setPosition(CCPointZero);
    errorBackground->addChild(errorLayerMenu);
    _errorLayer->setTouchEnabled(false);
    this->addChild(_errorLayer, 26);
    
    
    drawStartMenu();
    
    GKHWrapperCpp gkh;
    gkh.loadChallenges();
    
    return true;
}


#pragma mark CCLayer Method
void PopUpMainMenu::onEnter(){
    CCLayer::onEnter();
}


bool PopUpMainMenu::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}


#pragma mark Draw Layout
void PopUpMainMenu::drawStartMenu(){
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    
    // Main Layer
    CCLayer* mainLayer = CCLayer::create();
    mainLayer->setContentSize(CCSizeMake(secondBg->getContentSize().width/2, secondBg->getContentSize().height));
    mainLayer->setPosition(ccp(0, 0));
    secondBg->addChild(mainLayer,kForeground,kTagMainLayer);

    
    //Fly Button
    CCMenuItemImage* btn = CCMenuItemImage::create();
    btn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
    btn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
    btn->setTarget(this, menu_selector(PopUpMainMenu::callbackPressed));
    CCLabelBMFont* flyLabel = CCLabelBMFont::create("Fly", s_FontText);
    flyLabel->setPosition(ccp(btn->getContentSize().width/2, btn->getContentSize().height/2 - 6));
    flyLabel->setScale(1.5);
    btn->addChild(flyLabel);
    btn->setTag(kTagFlyButton);
    btn->setUserData(NULL);
    btn->setPosition(ccp(mainLayer->getContentSize().width/2, 300));
    
    
    //Challenge Button
    CCMenuItemImage* challengeBtn= CCMenuItemImage::create();
    CCScale9Sprite* challengeImg= CCScale9Sprite::create("buttons/violet_bg.png");
    challengeImg->setContentSize(CCSizeMake(410, 100));
    challengeImg->setAnchorPoint(ccp(0,0));
    challengeImg->setPosition(ccp(0,0));
    challengeBtn->addChild(challengeImg);
    challengeBtn->setContentSize(CCSizeMake(410, 100));
    challengeBtn->setTarget(this, menu_selector(PopUpMainMenu::showChallengeTable));
    CCSprite* forwardArrow = CCSprite::create("arrow_ico.png");
    forwardArrow->setAnchorPoint(ccp(0.5, 0.5));
    forwardArrow->setFlipX(true);
    forwardArrow->setPosition(ccp(400-forwardArrow->getContentSize().width/2, challengeImg->getContentSize().height/2));
    challengeImg->addChild(forwardArrow);
    CCSprite* newButton = CCSprite::create("buttons/new_icon.png");
    newButton->setAnchorPoint(ccp(0,0.8));
    newButton->setPosition(ccp(0, 100));
    challengeBtn->addChild(newButton);
    CCLabelBMFont* challengeLabel = CCLabelBMFont::create("Challenges", s_FontScore);
    challengeLabel->setPosition(ccp(challengeBtn->getContentSize().width/2, challengeBtn->getContentSize().height/2-6));
    challengeLabel->setScale(1.2);
    challengeBtn->addChild(challengeLabel);
    challengeBtn->setAnchorPoint(CCPointZero);
    challengeBtn->setPosition(ccp(0, -100));
    challengeBtn->setTag(kTagChallengeButton);
    challengeBtn->setVisible(false);
    
    CCMenu* menu = CCMenu::create(btn, challengeBtn, NULL);
    menu->setPosition(CCPointZero);
    menu->setTouchPriority(-303);
    mainLayer->addChild(menu, kForeground, kTagMainMenu);
    
    
    //
    //Challenge View Layer
    //
    
    CCLayer* challengeLayer = CCLayer::create();
    challengeLayer->setContentSize(CCSizeMake(secondBg->getContentSize().width/2,secondBg->getContentSize().height));
    challengeLayer->setAnchorPoint(CCPointZero);
    challengeLayer->setPosition(ccp(secondBg->getContentSize().width/2+1, 0));
    secondBg->addChild(challengeLayer,kForeground,kTagLayerChallenge);
    
    //Background for table and message box
    CCLayerColor *background=CCLayerColor::create(ccc4(33,43,112, 150), 410, 450);
    background->setPosition(ccp(0,70));
    background->setAnchorPoint(CCPointZero);
    
    //Table
    _tableView = CCTableView::create(this, CCSizeMake(410, 450));
	_tableView->setDirection(kCCScrollViewDirectionVertical);
    _tableView->setAnchorPoint(CCPointZero);
	_tableView->setPosition(CCPointZero);
	_tableView->setDelegate(this);
	_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	background->addChild(_tableView);
	_tableView->reloadData();
    
    challengeLayer->addChild(background);
    
    
    
    //Back Button
    CCMenuItemImage* backBtn= CCMenuItemImage::create();
    CCScale9Sprite* backImg= CCScale9Sprite::create("buttons/violet_bg.png");
    backImg->setContentSize(CCSizeMake(200, 60));
    backImg->setAnchorPoint(ccp(0,0));
    backImg->setPosition(ccp(0,0));
    backBtn->addChild(backImg);
    backBtn->setContentSize(CCSizeMake(200, 60));
    CCSprite* backArrow = CCSprite::create("arrow_ico.png");
    backArrow->setAnchorPoint(ccp(0, 0.5));
    backArrow->setPosition(ccp(10, backImg->getContentSize().height/2));
    backImg->addChild(backArrow);
    CCLabelBMFont* backLabel = CCLabelBMFont::create("Back", s_FontScore);
    backLabel->setPosition(ccp(backBtn->getContentSize().width/2, backBtn->getContentSize().height/2-6));
    backLabel->setScale(1.2);
    backBtn->addChild(backLabel);
    backBtn->setTarget(this, menu_selector(PopUpMainMenu::showMainMenu));
    backBtn->setPosition(ccp(backBtn->getContentSize().width/2,backBtn->getContentSize().height/2));
    CCMenu* menu1 = CCMenu::create(backBtn, NULL);
    menu1->setPosition(CCPointZero);
    menu1->setTouchPriority(-302);
    challengeLayer->addChild(menu1, kForeground, kTagBackMenu);
    
    challengeLayer->setVisible(false);
    
}



#pragma mark CCTableView Method

void PopUpMainMenu::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    _layerColorWait->setVisible(true);
    _spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
    
    //disable touch on lower layer
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(false);
    mainLayer->setVisible(true);
    
    CCLayer* challengeLayer =(CCLayer*) secondBg->getChildByTag(kTagLayerChallenge);
    CCMenu* backMenu = (CCMenu*) challengeLayer->getChildByTag(kTagBackMenu);
    backMenu->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GTOWrapperCpp gto;
    gto.retriveChallenge(_challenges[cell->getIdx()].gkScore.context);
#endif
    
    
}


CCSize PopUpMainMenu::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    
    return CCSizeMake(410, 138);
}


CCTableViewCell* PopUpMainMenu::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    std::string namePlayer = _friends[idx].alias;
    std::string message = _challenges[idx].message;
    
    CCTableViewCell *cell = table->dequeueCell();
    if (cell == NULL) {
        cell = new ChallengeViewCell(410, 138, (char*)namePlayer.c_str(), (char*)message.c_str());
        cell->setAnchorPoint(CCPointZero);
        cell->autorelease();
        cell->setContentSize(CCSizeMake(410, 138));
        CCLabelBMFont *labelPlayer =CCLabelBMFont::create(namePlayer.c_str(), s_FontScore);
        labelPlayer->setAnchorPoint(CCPointZero);
        labelPlayer->setPosition(ccp(15, 33));
        cell->addChild(labelPlayer);
        labelPlayer->setTag(123);
        CCLabelBMFont *labelMessage =CCLabelBMFont::create(message.c_str(), s_FontScore);
        labelMessage->setAnchorPoint(CCPointZero);
        labelMessage->setPosition(ccp(20, 90));
        labelMessage->setColor(ccBLACK);
        labelMessage->setScale(0.75);
        cell->addChild(labelMessage);
        labelMessage->setTag(124);
        
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(namePlayer.c_str());
        CCLabelTTF *label2=(CCLabelTTF*)cell->getChildByTag(124);
        label2->setString(message.c_str());
    }
    
    
    return cell;
}


unsigned int PopUpMainMenu::numberOfCellsInTableView(CCTableView *table)
{
    return _challenges.size();
}


#pragma mark Buttons Action




void PopUpMainMenu::showChallengeTable(CCObject * obj){
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* challengeLayer = secondBg->getChildByTag(kTagLayerChallenge);
    CCMenu* backMenu = (CCMenu*) challengeLayer->getChildByTag(kTagBackMenu);
    backMenu->setTouchEnabled(true);
    _tableView->setTouchEnabled(true);
    challengeLayer->setVisible(true);
    
    CCLayer* mainLayer = (CCLayer*)secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(false);
    
    secondBg->runAction(CCMoveBy::create(0.2f, ccp(-secondBg->getContentSize().width/2,0)));
    
}


void PopUpMainMenu::showMainMenu(CCObject *obj){
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(true);
    mainLayer->setVisible(true);
    
    CCLayer* challengeLayer =(CCLayer*) secondBg->getChildByTag(kTagLayerChallenge);
    CCMenu* backMenu = (CCMenu*) challengeLayer->getChildByTag(kTagBackMenu);
    backMenu->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
    secondBg->runAction(CCMoveBy::create(0.2f, ccp(secondBg->getContentSize().width/2,0)));
    
}


#pragma mark CallBack from Game Center
void PopUpMainMenu::onChallengeLoaded(std::map<std::string, GKChallengeCpp> challenges)
{
    int i=0;
    std::vector<std::string> array;
    
    for (std::map<std::string,GKChallengeCpp>::iterator it=challenges.begin(); it!=challenges.end(); ++it){
        if (it->second.gkScore.context!=0) {
            _challenges[i++]=it->second;
            array.push_back(it->second.issuingPlayerID);
            
        }
    }
    
    if(_challenges.size()){
        CCNode* clipper = this->getChildByTag(kTagClipperNode);
        CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
        CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
        CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
        CCMenuItem* challengeButton = (CCMenuItem*)mainMenu->getChildByTag(kTagChallengeButton);
        challengeButton->setVisible(true);
        challengeButton->runAction(CCEaseSineIn::create(CCMoveTo::create(0.1f, ccp(0,0))));
        GKHWrapperCpp gkh;
        gkh.getPlayerInfo(array);
    }
    
}

void PopUpMainMenu::onPlayerInfoReceived(std::vector<GKPlayerCpp> playerInfo){
    _friends=playerInfo;
    _tableView->reloadData();
}


#pragma mark Callback from GameTrackingEngine

void PopUpMainMenu::onGameTrackingObtained(GameTrackingObjCpp obj){
    GameTrackingObjCpp* gameTO = new GameTrackingObjCpp();
    gameTO->score=obj.score;
    gameTO->seed=new vector<double>();
    gameTO->seed=obj.seed;
    gameTO->tapIteration=new vector<int>();
    gameTO->tapIteration=obj.tapIteration;
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
    CCMenuItem* flyButton = (CCMenuItem*)mainMenu->getChildByTag(kTagFlyButton);
    flyButton->setUserData(gameTO);
    mainMenu->removeChildByTag(kTagChallengeButton);
    mainMenu->setTouchEnabled(true);
    mainLayer->setVisible(true);
    
    CCLayer* challengeLayer =(CCLayer*) secondBg->getChildByTag(kTagLayerChallenge);
    CCMenu* backMenu = (CCMenu*) challengeLayer->getChildByTag(kTagBackMenu);
    backMenu->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
    _layerColorWait->setVisible(false);
    _spinner->stopAllActions();
    
    secondBg->runAction(CCMoveBy::create(0.2f, ccp(secondBg->getContentSize().width/2,0)));
}

void PopUpMainMenu::onGameTrackingFailed(){
    _layerColorWait->setVisible(false);
    _spinner->stopAllActions();
    _errorLayer->setVisible(true);
    
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(false);
    mainLayer->setVisible(true);
    
    CCLayer* challengeLayer =(CCLayer*) secondBg->getChildByTag(kTagLayerChallenge);
    CCMenu* backMenu = (CCMenu*) challengeLayer->getChildByTag(kTagBackMenu);
    backMenu->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
    secondBg->runAction(CCMoveBy::create(0.2f, ccp(secondBg->getContentSize().width/2,0)));
}

#pragma mark Callback

void PopUpMainMenu::callbackErrorLayer(CCObject *obj){
    _errorLayer->setVisible(false);
    
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*)mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(true);
}

#pragma mark Set callback to GameScene

void PopUpMainMenu::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
    _callbackListener = target;
    _callback = callfun;
}


void PopUpMainMenu::callbackPressed(cocos2d::CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
}