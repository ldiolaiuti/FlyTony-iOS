//
//  ChallengeViewCell.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 22/04/14.
//
//

#ifndef __FlyTony__ChallengeViewCell__
#define __FlyTony__ChallengeViewCell__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChallengeViewCell : public cocos2d::extension::CCTableViewCell
{
private:
    bool                _selected;
    CCLayerColor*       _background;
    CCLayer*            _canvasLayer;
    CCSprite*           _messageBox;
    CCLayerColor*       _acceptedLayer;
    CCLabelBMFont*      _accept;
    
public:
    ChallengeViewCell(int width, int height, char* name, char* message);
    virtual ~ChallengeViewCell();
    
    virtual void onEnter();
    virtual void touched();
    
    virtual bool isSelected();
    virtual void setSelected(bool selected);
};

#endif /* defined(__FlyTony__ChallengeViewCell__) */
