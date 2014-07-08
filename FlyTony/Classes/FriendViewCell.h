//
//  CustomViewCell.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 26/03/14.
//
//

#ifndef FlyTony_FriendViewCell_h
#define FlyTony_FriendViewCell_h


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class FriendViewCell : public cocos2d::extension::CCTableViewCell
{
private:
    bool            _selected;
    CCLayerColor*   _background;
    CCLayerColor*        _canvasLayer;
    CCSprite*       _radioUnselected;
    CCSprite*       _radioSelected;
    
public:
    FriendViewCell(int width, int height);
    virtual ~FriendViewCell();
    
    virtual void onEnter();
    virtual void touched();
    
    virtual bool isSelected();
    virtual void setSelected(bool selected);
};


#endif
