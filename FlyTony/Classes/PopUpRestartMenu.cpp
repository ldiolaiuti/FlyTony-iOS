//
//  PopUpRestartMenu.cpp
//  FlyTony
//
//  Created by Luca Diolaiuti on 23/04/14.
//
//

#include "PopUpRestartMenu.h"
#include "SimpleAudioEngine.h"
#import "GKHWrapperCpp.h"


using namespace CocosDenshion;

static const int kForeground = 7;


static const char s_FontText[] = "fonts/text.fnt";
static const char s_FontScore[] = "fonts/score.fnt";
static const char s_FontScoreBig[] = "fonts/ScoreBig.fnt";

enum {
	kTagStencilNode = 205,
	kTagClipperNode = 206,
    kTagFirstBgLayer = 207,
    kTagSecondBgLayer = 208,
    kTagMainLayer   = 209,
    kTagLayerFriend = 210,
    kTagChallengeMenu =211,
    kTagBackMenu = 212,
    kTagTableLayer = 213,
    kTagFriendMenu = 214,
    kTagMainMenu = 215,
    kTagMainMenuButton = 216,
    kTagAcceptButton = 217,
    kTagRetryButton = 218,
};


#pragma mark Constructor

//constructor without challenge
PopUpRestartMenu::PopUpRestartMenu(int score, bool isNewRecord){
    
    _score=score;
    _isNewRecord=isNewRecord;
    _isChallenge=false;
    
    GKHWrapperCpp gkh;
    gkh.setDelegate(this);
    
    GTOWrapperCpp gto;
    gto.setDelegate(this);
    
    _challenges.clear();
    
}


//constructor with challenge
PopUpRestartMenu::PopUpRestartMenu(int score, bool isNewRecord, bool win){
    _score=score;
    _isNewRecord=isNewRecord;
    _win=win;
    _isChallenge=true;
    
    GKHWrapperCpp gkh;
    gkh.setDelegate(this);
    
    GTOWrapperCpp gto;
    gto.setDelegate(this);
    
    _challenges.clear();
    
}


PopUpRestartMenu::~PopUpRestartMenu(){
    if (_medalAnimation!=NULL)
        CC_SAFE_RELEASE(_medalAnimation);
}


PopUpRestartMenu* PopUpRestartMenu::create(int score, bool isNewRecord)
{
    PopUpRestartMenu* popUpLayer = new PopUpRestartMenu(score, isNewRecord);
    
    if (popUpLayer && popUpLayer->PopUpRestartMenu::init())
    {
        popUpLayer->autorelease();
        return popUpLayer;
    }
    CC_SAFE_DELETE(popUpLayer);
    return NULL;
}


PopUpRestartMenu* PopUpRestartMenu::createWithChallenge(int score, bool isNewRecord, bool win){
    PopUpRestartMenu* popUpLayer = new PopUpRestartMenu(score, isNewRecord, win);
    
    if (popUpLayer && popUpLayer->PopUpRestartMenu::init())
    {
        popUpLayer->autorelease();
        return popUpLayer;
    }
    CC_SAFE_DELETE(popUpLayer);
    return NULL;
}


bool PopUpRestartMenu::init(){
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
    
    //Replay Button
    if (!_isChallenge){
        CCMenuItemImage* btn = CCMenuItemImage::create();
        btn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        btn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        btn->setTarget(this, menu_selector(PopUpRestartMenu::callbackPressed));
        CCLabelBMFont* restartLabel = CCLabelBMFont::create("restart", s_FontText);
        restartLabel->setPosition(ccp(btn->getContentSize().width/2, btn->getContentSize().height/2 - 6));
        btn->addChild(restartLabel);
        btn->setPosition(ccp(bg->getContentSize().width/2, bg->getContentSize().height/2+310));
        CCMenu* restartMenu = CCMenu::create(btn, NULL);
        restartMenu->setPosition(CCPointZero);
        bg->addChild(restartMenu);
    }else if (!_win){
        CCMenuItemImage* btn = CCMenuItemImage::create();
        btn->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        btn->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cloud"));
        btn->setTarget(this, menu_selector(PopUpRestartMenu::callbackPressed));
        CCLabelBMFont* restartLabel = CCLabelBMFont::create("retry", s_FontText);
        restartLabel->setPosition(ccp(btn->getContentSize().width/2, btn->getContentSize().height/2 - 6));
        btn->addChild(restartLabel);
        btn->setPosition(ccp(bg->getContentSize().width/2, bg->getContentSize().height/2+310));
        CCMenu* restartMenu = CCMenu::create(btn, NULL);
        restartMenu->setPosition(CCPointZero);
        btn->setTag(kTagRetryButton);
        bg->addChild(restartMenu);
    }
    
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
    okButton->setTarget(this, menu_selector(PopUpRestartMenu::callbackErrorLayer));
    CCLabelBMFont* okButtonLabel = CCLabelBMFont::create("OK", s_FontScore);
    okButton->setAnchorPoint(ccp(0,0));
    okButtonLabel->setPosition(ccp(okButton->getContentSize().width*0.5, okButton->getContentSize().height*0.5-6));
    okButtonLabel->setScale(1.5);
    okButton->addChild(okButtonLabel);
    CCMenu* errorLayerMenu = CCMenu::create(okButton, NULL);
    errorLayerMenu->setPosition(CCPointZero);
    errorBackground->addChild(errorLayerMenu);
    this->addChild(_errorLayer, 26);
    
    drawRestartMenu();
    
    _recordRays = CCSprite::createWithSpriteFrameName("recordRays");
    _recordRays->setPosition(ccp(screenSize.width/2, screenSize.height * 0.75));
    this->addChild(_recordRays,20);
    _recordRays->setOpacity(0);
    
    _challengeWonLabel = CCLabelBMFont::create("Challenge Won!", s_FontText);
    _challengeWonLabel->setPosition(ccp(screenSize.width/2, screenSize.height + _challengeWonLabel->getContentSize().height * 2));
    this->addChild(_challengeWonLabel,21);
    
    if(_isChallenge && _win){
        _challengeWonLabel->runAction(CCEaseBackOut::create(CCMoveTo::create(0.5, ccp(screenSize.width/2, screenSize.height *0.75))));
        _recordRays->runAction(CCFadeIn::create(0.5));
        _recordRays->runAction(CCRepeatForever::create(CCRotateBy::create(8, -360)));
    }
    
    if(_isChallenge){
        if(_win){
            SimpleAudioEngine::sharedEngine()->preloadEffect("Suoni/you_win.mp3");
            SimpleAudioEngine::sharedEngine()->playEffect("Suoni/you_win.mp3");
        }
        else{
            SimpleAudioEngine::sharedEngine()->preloadEffect("Suoni/you_lose.mp3");
            SimpleAudioEngine::sharedEngine()->playEffect("Suoni/you_lose.mp3");
        }
    }
    
    return true;
}


#pragma mark CCLayer Method
void PopUpRestartMenu::onEnter(){
    CCLayer::onEnter();
    
    GKHWrapperCpp gkh;
    gkh.loadChallenges();
}

#pragma mark Draw Layout

void PopUpRestartMenu::drawRestartMenu(){
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    
    // Main Layer
    CCLayer* mainLayer = CCLayer::create();
    mainLayer->setContentSize(CCSizeMake(secondBg->getContentSize().width/2, secondBg->getContentSize().height));
    mainLayer->setPosition(ccp(0, 0));
    secondBg->addChild(mainLayer,kForeground,kTagMainLayer);
    
    
    //Score Label
    CCMenuItemImage* scoreItem= CCMenuItemImage::create();
    scoreItem->setAnchorPoint(CCPointZero);
    CCScale9Sprite* scoreImg= CCScale9Sprite::create("buttons/blu_bg.png");
    scoreImg->setContentSize(CCSizeMake(200, 200));
    scoreImg->setAnchorPoint(ccp(0,0));
    scoreImg->setPosition(ccp(scoreItem->getContentSize().width, scoreItem->getContentSize().height));
    scoreItem->addChild(scoreImg);
    scoreItem->setContentSize(CCSizeMake(200, 200));
    if(_isChallenge)
        scoreItem->setPosition(ccp(0,210));
    else
        scoreItem->setPosition(ccp(0, 320));
    CCLabelBMFont* scoreLabel = CCLabelBMFont::create("Score", s_FontScore);
    scoreLabel->setPosition(ccp(scoreItem->getContentSize().width/2, scoreItem->getContentSize().height*0.9-30));
    scoreLabel->setScale(1.3);
    scoreItem->addChild(scoreLabel);
    CCLabelBMFont* scored = CCLabelBMFont::create(CCString::createWithFormat("%d",_score)->getCString(), s_FontScoreBig);
    scored->setPosition(ccp(scoreItem->getContentSize().width/2, scoreItem->getContentSize().height/2-30));
    scored->setScale(1.5);
    scoreItem->addChild(scored);
    
    
    //Record Label
    CCMenuItemImage* recordItem= CCMenuItemImage::create();
    recordItem->setAnchorPoint(CCPointZero);
    CCScale9Sprite* recordImg= CCScale9Sprite::create("buttons/bluHighlight_bg.png");
    recordImg->setContentSize(CCSizeMake(200, 200));
    recordImg->setAnchorPoint(ccp(0,0));
    recordImg->setPosition(ccp(recordItem->getContentSize().width, recordItem->getContentSize().height));
    recordItem->addChild(recordImg);
    recordItem->setContentSize(CCSizeMake(200, 200));
    if (_isChallenge)
        recordItem->setPosition(ccp(210, 210));
    else
        recordItem->setPosition(ccp(210, 320));
    
    if(_isNewRecord){
        CCSprite* nastro = CCSprite::create("nastro.png");
        nastro->setPosition(ccp(100, 130));
        recordItem->addChild(nastro);
        CCSprite* badge = CCSprite::create("medal0001.png");
        badge->setPosition(ccp(100, 55));
        recordItem->addChild(badge);
        _medalAnimation = CCAnimation::create();
        _medalAnimation->addSpriteFrameWithFileName("medal0001.png");
        _medalAnimation->addSpriteFrameWithFileName("medal0002.png");
        _medalAnimation->addSpriteFrameWithFileName("medal0003.png");
        _medalAnimation->addSpriteFrameWithFileName("medal0004.png");
        for(int i=0; i<25; i++){
            _medalAnimation->addSpriteFrameWithFileName("medal0001.png");
        }
        _medalAnimation->setDelayPerUnit(0.08f);
        _medalAnimation->retain();
        badge->runAction(CCRepeatForever::create(CCAnimate::create(_medalAnimation)));
        CCLabelBMFont* recordLabel = CCLabelBMFont::create("Record!", s_FontScore);
        recordLabel->setPosition(ccp(recordItem->getContentSize().width/2, recordItem->getContentSize().height*0.9-30));
        recordLabel->setScale(1.3);
        recordItem->addChild(recordLabel);
    }else{
        CCLabelBMFont* recordLabel = CCLabelBMFont::create("Highscore", s_FontScore);
        recordLabel->setPosition(ccp(recordItem->getContentSize().width/2, recordItem->getContentSize().height*0.9-30));
        recordLabel->setScale(1.3);
        recordItem->addChild(recordLabel);
        CCLabelBMFont* record = CCLabelBMFont::create(CCUserDefault::sharedUserDefault()->getStringForKey("currentScore","0").c_str(),      s_FontScoreBig);
        record->setPosition(ccp(recordItem->getContentSize().width/2, recordItem->getContentSize().height/2-30));
        record->setScale(1.5);
        recordItem->addChild(record);
    }
    
    
    //Leaderbord Button
    CCMenuItemImage* leaderboardItem= CCMenuItemImage::create();
    leaderboardItem->setAnchorPoint(CCPointZero);
    CCScale9Sprite* leaderboardImg= CCScale9Sprite::create("buttons/violet_bg.png");
    leaderboardImg->setContentSize(CCSizeMake(410, 100));
    leaderboardImg->setAnchorPoint(ccp(0,0));
    leaderboardImg->setPosition(ccp(leaderboardItem->getContentSize().width, leaderboardItem->getContentSize().height));
    leaderboardItem->addChild(leaderboardImg);
    leaderboardItem->setContentSize(CCSizeMake(410, 100));
    if(_isChallenge)
        leaderboardItem->setPosition(ccp(0,105));
    else
        leaderboardItem->setPosition(ccp(0, 210));
    leaderboardItem->setTarget(this, menu_selector(PopUpRestartMenu::showLeaderBoard));
    CCLabelBMFont* leaderboardLabel = CCLabelBMFont::create("Leaderboard", s_FontScore);
    leaderboardLabel->setAnchorPoint(ccp(0, 0.5));
    leaderboardLabel->setPosition(ccp(20, leaderboardItem->getContentSize().height*0.5-6));
    leaderboardLabel->setScale(1.2);
    leaderboardItem->addChild(leaderboardLabel);
    
    
    //Challenge Button
    CCMenuItemImage* challengeItem= CCMenuItemImage::create();
    if(!_isChallenge){
        challengeItem->setAnchorPoint(CCPointZero);
        CCScale9Sprite* challengeImg= CCScale9Sprite::create("buttons/violet_bg.png");
        challengeImg->setContentSize(CCSizeMake(410, 100));
        challengeImg->setAnchorPoint(ccp(0,0));
        challengeImg->setPosition(ccp(challengeItem->getContentSize().width, challengeItem->getContentSize().height));
        challengeItem->addChild(challengeImg);
        challengeItem->setContentSize(CCSizeMake(410, 100));
        challengeItem->setPosition(ccp(0, 105));
        challengeItem->setTarget(this, menu_selector(PopUpRestartMenu::showFriendTable));
        CCSprite* forwardArrow = CCSprite::create("arrow_ico.png");
        forwardArrow->setFlipX(true);
        forwardArrow->setAnchorPoint(ccp(0.5,0.5));
        forwardArrow->setPosition(ccp(400-forwardArrow->getContentSize().width/2, challengeImg->getContentSize().height/2));
        challengeImg->addChild(forwardArrow);
        CCLabelBMFont* challengeLabel = CCLabelBMFont::create("Challenge Friends", s_FontScore);
        challengeLabel->setAnchorPoint(ccp(0, 0.5));
        challengeLabel->setPosition(ccp(20, challengeItem->getContentSize().height*0.5-6));
        challengeLabel->setScale(1.2);
        challengeItem->addChild(challengeLabel);
    }else if(_isChallenge && !_win){
        challengeItem->setAnchorPoint(CCPointZero);
        CCScale9Sprite* challengeImg= CCScale9Sprite::create("buttons/bluHighlight_bg.png");
        challengeImg->setContentSize(CCSizeMake(410, 100));
        challengeImg->setAnchorPoint(ccp(0,0));
        challengeImg->setPosition(ccp(challengeItem->getContentSize().width, challengeItem->getContentSize().height));
        challengeItem->addChild(challengeImg);
        challengeItem->setContentSize(CCSizeMake(410, 100));
        challengeItem->setPosition(ccp(0, 420));
        CCLabelBMFont* challengeLabel = CCLabelBMFont::create("Challenge Lost!", s_FontScore);
        challengeLabel->setAnchorPoint(ccp(0.5, 0.5));
        challengeLabel->setPosition(ccp(challengeItem->getContentSize().width*0.5f, challengeItem->getContentSize().height*0.5-6));
        challengeLabel->setScale(1.5);
        challengeItem->addChild(challengeLabel);
    }
    
    
    //Main menu Button
    CCMenuItemImage* startItem= CCMenuItemImage::create();
    startItem->setAnchorPoint(CCPointZero);
    startItem->setTag(kTagMainMenuButton);
    CCScale9Sprite* startImg= CCScale9Sprite::create("buttons/violet_bg.png");
    startImg->setContentSize(CCSizeMake(410, 100));
    startImg->setAnchorPoint(ccp(0,0));
    startImg->setPosition(ccp(startItem->getContentSize().width, startItem->getContentSize().height));
    startItem->addChild(startImg);
    startItem->setContentSize(CCSizeMake(410, 100));
    startItem->setPosition(ccp(0, 0));
    startItem->setTarget(this, menu_selector(PopUpRestartMenu::callbackPressed));
    CCLabelBMFont* startLabel = CCLabelBMFont::create("Main Menu", s_FontScore);
    startLabel->setAnchorPoint(ccp(0, 0.5));
    startLabel->setPosition(ccp(20, startItem->getContentSize().height*0.5-6));
    startLabel->setScale(1.2);
    startItem->addChild(startLabel);
    
    
    CCMenu* menu = CCMenu::create(scoreItem, recordItem, leaderboardItem, challengeItem, startItem, NULL);
    menu->setPosition(CCPointZero);
    mainLayer->addChild(menu, kForeground, kTagMainMenu);
    
    //
    //Friend View Layer
    //
    
    CCLayer* friendLayer = CCLayer::create();
    friendLayer->setContentSize(CCSizeMake(secondBg->getContentSize().width/2,secondBg->getContentSize().height));
    friendLayer->setAnchorPoint(CCPointZero);
    friendLayer->setPosition(ccp(secondBg->getContentSize().width/2+1, 0));
    secondBg->addChild(friendLayer,kForeground,kTagLayerFriend);
    
    //Background for table and message box
    CCLayerColor* background=CCLayerColor::create(ccc4(33,43,112, 150), 410, 450);
    background->setPosition(ccp(0,70));
    background->setAnchorPoint(CCPointZero);
    CCLabelBMFont* chooseLabel = CCLabelBMFont::create("Choose a friend", s_FontScore);
    chooseLabel->setAnchorPoint(CCPointZero);
    chooseLabel->setPosition(ccp(10, 380));
    chooseLabel->setScale(1.2);
    background->addChild(chooseLabel);
    background->setTouchPriority(-303);
    
    //Table
    _tableView = CCTableView::create(this, CCSizeMake(410, 280));
	_tableView->setDirection(kCCScrollViewDirectionVertical);
	_tableView->setPosition(ccp(0, 100));
	_tableView->setDelegate(this);
	_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	background->addChild(_tableView);
	_tableView->reloadData();
    
    //Message Box
    _messageBox=CCEditBox::create(CCSizeMake(400, 90), CCScale9Sprite::create("white_bg.png"));
    _messageBox->setAnchorPoint(ccp(0, 0));
    _messageBox->setPosition(ccp(5,10));
    _messageBox->setFontColor(ccBLACK);
    _messageBox->setFont("Helvetica", 30);
    _messageBox->setPlaceHolder("I bet I can fly better than you!");
    _messageBox->setPlaceholderFontColor(ccBLACK);
    _messageBox->setMaxLength(30);
    _messageBox->setReturnType(kKeyboardReturnTypeDone);
    background->addChild(_messageBox);
    friendLayer->addChild(background, kForeground+1, kTagTableLayer);
    
    //Back Button
    CCMenuItemImage* backBtn= CCMenuItemImage::create();
    backBtn->setAnchorPoint(CCPointZero);
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
    backBtn->setTarget(this, menu_selector(PopUpRestartMenu::showMainMenu));
    backBtn->setPosition(CCPointZero);
    
    //ok Button
    CCMenuItemImage* okBtn= CCMenuItemImage::create();
    okBtn->setAnchorPoint(CCPointZero);
    okBtn->setTag(kTagAcceptButton);
    CCScale9Sprite* okImg= CCScale9Sprite::create("buttons/violet_bg.png");
    okImg->setContentSize(CCSizeMake(200, 60));
    okImg->setAnchorPoint(ccp(0,0));
    okImg->setPosition(ccp(0,0));
    okBtn->addChild(okImg);
    okBtn->setContentSize(CCSizeMake(200, 60));
    CCLabelBMFont* okLabel = CCLabelBMFont::create("Challenge!", s_FontScore);
    okLabel->setPosition(ccp(backBtn->getContentSize().width/2, backBtn->getContentSize().height/2-6));
    okLabel->setScale(1.2);
    okBtn->addChild(okLabel);
    okBtn->setTarget(this, menu_selector(PopUpRestartMenu::sendChallengeToFriends));
    okBtn->setPosition(ccp(210, 0));
    
    
    CCMenu* friendMenu = CCMenu::create(backBtn, okBtn, NULL);
    friendMenu->setPosition(CCPointZero);
    friendLayer->addChild(friendMenu, kForeground, kTagFriendMenu);
    
    GKHWrapperCpp gkh;
    gkh.getLocalPlayerFriends();
    
    friendLayer->setVisible(false);
}



#pragma mark CCTableView Method
void PopUpRestartMenu::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    FriendViewCell* auxCell =(FriendViewCell*) cell;
    auxCell->touched();
    _selected.at(auxCell->getIdx())= auxCell->isSelected();
}

CCSize PopUpRestartMenu::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(410, 55);
}

CCTableViewCell* PopUpRestartMenu::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    std::string namePlayer = _friends.at(idx).alias;
    
    CCTableViewCell *cell = table->dequeueCell();
    if (cell == NULL) {
        cell = new FriendViewCell(410, 60);
        cell->autorelease();
        cell->setContentSize(CCSizeMake(410, 60));
        CCLabelBMFont *labelPlayer =CCLabelBMFont::create(namePlayer.c_str(), s_FontScore);
        labelPlayer->setAnchorPoint(ccp(0, 0.5));
        labelPlayer->setPosition(ccp(55, cell->getContentSize().height*0.5f));
        cell->addChild(labelPlayer);
        labelPlayer->setTag(123);
        
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(namePlayer.c_str());
    }
    
    _selected.push_back(false);
    
    return cell;
}


unsigned int PopUpRestartMenu::numberOfCellsInTableView(CCTableView *table)
{
    return _friends.size();
}


#pragma mark Buttons Action

void PopUpRestartMenu::showFriendTable(CCObject *obj){
    GKHWrapperCpp gkh;
    if(gkh.isLocalPlayerAuthenticated()){
        CCNode* clipper = this->getChildByTag(kTagClipperNode);
        CCLayer* secondBg = (CCLayer*)clipper->getChildByTag(kTagSecondBgLayer);
        CCLayer* friendLayer = (CCLayer*)secondBg->getChildByTag(kTagLayerFriend);
        CCMenu* friendMenu = (CCMenu*) friendLayer->getChildByTag(kTagFriendMenu);
        friendMenu->setTouchEnabled(true);
        _messageBox->setTouchEnabled(true);
        _tableView->setTouchEnabled(true);
        friendLayer->setVisible(true);
    
        //Set main layer untouchable
        CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
        CCMenu* mainMenu = (CCMenu*) mainLayer->getChildByTag(kTagMainMenu);
        mainMenu->setTouchEnabled(false);
    
        secondBg->runAction(CCMoveBy::create(0.2f, ccp(-secondBg->getContentSize().width/2,0)));
    }
    else{
        gkh.authenticateLocalPlayer();
    }
}


void PopUpRestartMenu::showMainMenu(CCObject *obj){
    
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*) mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(true);
    mainLayer->setVisible(true);
    
    //Set second layer untouchable
    CCNode* friendLayer = secondBg->getChildByTag(kTagLayerFriend);
    CCMenu* friendMenu = (CCMenu*) friendLayer->getChildByTag(kTagFriendMenu);
    friendMenu->setTouchEnabled(false);
    _messageBox->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
    secondBg->runAction(CCMoveBy::create(0.2f, ccp(secondBg->getContentSize().width/2,0)));
    
}


void PopUpRestartMenu::showLeaderBoard(CCObject * obj){
    
    GKHWrapperCpp* gkhw;
    gkhw->showLeaderBoard();
    
}

#pragma mark CallBack from Game Center
void PopUpRestartMenu::onFriendsListRecieved(std::vector<std::string> friends)
{
    GKHWrapperCpp gkh;
    gkh.getPlayerInfo(friends);
    
}

void PopUpRestartMenu::onPlayerInfoReceived(std::vector<GKPlayerCpp> playerInfo)
{
    _friends=playerInfo;
    _tableView->reloadData();
}

void PopUpRestartMenu::sendChallengeToFriends(CCObject *obj){
    char* message = (char*)_messageBox->getText();
    
    _item=(CCMenuItem*)obj;
    
    if(strcmp(_messageBox->getText(), "")==0){
        message=(char*)_messageBox->getPlaceHolder();
    }
    
    std::vector<std::string> array;
    
    for (int i = 0; i<_selected.size(); i++){
        if(_selected.at(i))
            array.push_back(_friends.at(i).playerID);
    }
    
    GTOWrapperCpp gto;
    gto.sendScoreChallengeToPlayers(array, _score, message);
    
    _layerColorWait->setVisible(true);
    _spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
    
    
    //disable touch on lower layer
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*) mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(false);
    mainLayer->setVisible(true);
    
    //Set second layer untouchable
    CCNode* friendLayer = secondBg->getChildByTag(kTagLayerFriend);
    CCMenu* friendMenu = (CCMenu*) friendLayer->getChildByTag(kTagFriendMenu);
    friendMenu->setTouchEnabled(false);
    _messageBox->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
}


#pragma mark Callback
//Challenge sended, then return to main menu
void PopUpRestartMenu::onChallengeSended(){
    _layerColorWait->setVisible(false);
    _spinner->stopAllActions();
    if(_item!=NULL)
        callbackPressed(_item);
    else
        this->onSendingChallengeError();
}

//Challenge not sended, show error message
void PopUpRestartMenu::onSendingChallengeError(){
    _layerColorWait->setVisible(false);
    _spinner->stopAllActions();
    _errorLayer->setVisible(true);
    
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*) mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(false);
    mainLayer->setVisible(true);
    
    //Set second layer untouchable
    CCNode* friendLayer = secondBg->getChildByTag(kTagLayerFriend);
    CCMenu* friendMenu = (CCMenu*) friendLayer->getChildByTag(kTagFriendMenu);
    friendMenu->setTouchEnabled(false);
    _messageBox->setTouchEnabled(false);
    _tableView->setTouchEnabled(false);
    
    secondBg->runAction(CCMoveBy::create(0.2f, ccp(secondBg->getContentSize().width/2,0)));
}

void PopUpRestartMenu::callbackErrorLayer(){
    _errorLayer->setVisible(false);
    
    CCNode* clipper = this->getChildByTag(kTagClipperNode);
    CCNode* secondBg = clipper->getChildByTag(kTagSecondBgLayer);
    CCNode* mainLayer = secondBg->getChildByTag(kTagMainLayer);
    CCMenu* mainMenu = (CCMenu*) mainLayer->getChildByTag(kTagMainMenu);
    mainMenu->setTouchEnabled(true);
}



#pragma mark Set callback to Gamescene
void PopUpRestartMenu::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
    _callbackListener = target;
    _callback = callfun;
}


void PopUpRestartMenu::callbackPressed(cocos2d::CCObject *obj)
{
    CCMenuItem* item = (CCMenuItem*) obj;
    //Show intertitial when come back to main menu
    if(item->getTag()==kTagMainMenuButton){
        SendMessageWithParams(string("loadInterstitial"), NULL);
    }
    
    GKHWrapperCpp gkh;
    gkh.setDelegate(NULL);
    
    
    GTOWrapperCpp gto;
    gto.setDelegate(NULL);
    
    CCNode* node = dynamic_cast<CCNode*>(obj);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
}