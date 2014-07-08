//
//  PopUpRestartMenu.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 23/04/14.
//
//

#ifndef __FlyTony__PopUpRestartMenu__
#define __FlyTony__PopUpRestartMenu__

#include <iostream>

#endif /* defined(__FlyTony__PopUpRestartMenu__) */

#include <iostream>
#include "cocos2d.h"
#include "CCTableView.h"
#include "cocos-ext.h"
#include "NDKHelper/NDKHelper.h"
#include "GKHDelegate.h"
#include "GTOWrapperCpp.h"
#include "GTODelegate.h"
#include "FriendViewCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PopUpRestartMenu: public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate, public GKHDelegate, public GTODelegate{
    
    CCMenu*                         _menu;
    CCMenuItem*                     _item=NULL;
    CCTableView*                    _tableView ;
    CCLayerColor*                   _background;
    CCLayerColor*                   _errorLayer;
    CCLayerColor*                   _layerColorWait;
    CCSprite*                       _spinner;
    CCEditBox*                      _messageBox;
    CCSprite*                       _recordRays;
    CCLabelBMFont*                  _challengeWonLabel;
    CCAnimation*                    _medalAnimation=NULL;
    std::map<int, GKChallengeCpp>   _challenges;
    std::vector<GKPlayerCpp>        _friends;
    std::vector<bool>               _selected;
    GameTrackingObjCpp              _tracking;
    
    int                 _score;
    bool                _isNewRecord=false;
    bool                _isChallenge;
    bool                _win;
    CCObject*           _callbackListener;
    SEL_CallFuncN       _callback;
    
public:
    
    static PopUpRestartMenu* create(int score, bool isNewRecord);
    static PopUpRestartMenu* createWithChallenge(int score, bool isNewRecord, bool win);
    
    PopUpRestartMenu(int score, bool isNewRecord);
    PopUpRestartMenu(int score, bool isNewRecord, bool win);
    ~PopUpRestartMenu();
    
    virtual void onEnter();
    virtual bool init();
    
    //CCTableView Method
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);

    
private:
    void drawRestartMenu();
    
    void showFriendTable(CCObject * obj);
    void showMainMenu(CCObject *obj);
    void showLeaderBoard(CCObject * obj);
    
    
    // CallBack from Game Center
    void onFriendsListRecieved(std::vector<std::string> friends);
    void onPlayerInfoReceived(std::vector<GKPlayerCpp> playerInfo);
    void sendChallengeToFriends(CCObject* obj);
    
    void onChallengeSended();
    void onSendingChallengeError();
    void callbackErrorLayer();
    
    void callbackPressed(cocos2d::CCObject *obj);
    
    
};
