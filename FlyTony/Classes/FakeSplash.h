//
//  FakeSplash.h
//  Kroms
//
//  Created by Giuseppe Lapenta on 23/11/13.
//
//

#ifndef __Kroms__FakeSplash__
#define __Kroms__FakeSplash__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FakeSplash : public CCLayer
{
    
    void showInitialMenu(CCNode* node);
    
public:
    
    FakeSplash();
    virtual ~FakeSplash();
    
    static CCScene* scene();
    CREATE_FUNC(FakeSplash);
    
    virtual bool init();
};

#endif /* defined(__Kroms__FakeSplash__) */
