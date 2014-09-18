//
//  GTODelegate.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 04/04/14.
//
//

#ifndef __FlyTony__GTODelegate__
#define __FlyTony__GTODelegate__

#include <iostream>
#include "GTOWrapperCpp.h"

class GTODelegate{
public:
    
    bool _debug;
    GTODelegate();
    ~GTODelegate();
    
    virtual void onGameTrackingObtained(GameTrackingObjCpp obj);
    virtual void onGameTrackingFailed();
};

#endif /* defined(__FlyTony__GTODelegate__) */
