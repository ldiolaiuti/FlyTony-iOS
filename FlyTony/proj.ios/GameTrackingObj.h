//
//  GameTrackingObj.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 27/03/14.
//
//

#include "GTODelegate.h"
#include "GTOWrapperCpp.h"
#include <Foundation/Foundation.h>

@interface GameTrackingObj : NSObject

@property (nonatomic, strong) NSMutableArray *tapIteration;
@property (nonatomic, readwrite) double seed;
@property (nonatomic, readwrite) NSInteger score;

+ (GameTrackingObj*) sharedGameTrackingObj;

- (void) addTapIteration:(int32_t) iteration;
- (void) addSeed:(double) seme;
- (void) addScore:(int32_t) score;
- (void) clear;

@end
