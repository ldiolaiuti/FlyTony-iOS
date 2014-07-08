//
//  PopUpMainMenu.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 17/04/14.
//
//

#ifndef __FlyTony__PopUpMainMenu__
#define __FlyTony__PopUpMainMenu__

#include <iostream>
#include "cocos2d.h"
#include "CCTableView.h"
#include "cocos-ext.h"
#include "NDKHelper/NDKHelper.h"
#include "GKHDelegate.h"
#include "GTOWrapperCpp.h"
#include "GTODelegate.h"
#include "ChallengeViewCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PopUpMainMenu: public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate, public GKHDelegate, public GTODelegate{
    
    CCMenu*                         _menu;
    CCTableView*                    _tableView ;
    CCLayerColor*                   _background;
    CCLayerColor*                   _layerColorWait;
    CCSprite*                       _spinner;
    CCLayerColor*                   _errorLayer;
    CCEditBox*                      _messageBox;
    std::map<int, GKChallengeCpp>   _challenges;
    std::vector<GKPlayerCpp>        _friends;
    GameTrackingObjCpp              _tracking;
    CCObject*                       _callbackListener;
    SEL_CallFuncN                   _callback;
    
public:
    
    static PopUpMainMenu* create();
    
    PopUpMainMenu();
    ~PopUpMainMenu();
    
    virtual void onEnter();
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    
    void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);
    
private:
    void drawStartMenu();
    
    //CCTableView Method
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    //Show other menu
    void showChallengeTable(CCObject * obj);
    void showMainMenu(CCObject *obj);
    
    
    // CallBack from Game Center
    void onChallengeLoaded(std::map<std::string, GKChallengeCpp> challenges);
    void onPlayerInfoReceived(std::vector<GKPlayerCpp> playerInfo);
    
    //Callback
    void onGameTrackingObtained(GameTrackingObjCpp obj);
    void onGameTrackingFailed();
    void callbackErrorLayer(CCObject *obj);
    void callbackPressed(cocos2d::CCObject *obj);
    
    
};

#endif /* defined(__FlyTony__PopUpMainMenu__) */
