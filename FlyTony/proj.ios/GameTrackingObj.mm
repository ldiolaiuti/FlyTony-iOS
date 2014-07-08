//
//  GameTrackingObj.m
//  FlyTony
//
//  Created by Luca Diolaiuti on 27/03/14.
//
//

#import "GameTrackingObj.h"

@implementation GameTrackingObj


static GameTrackingObj *instanceOfGameTrackingObj;

#pragma mark Singleton stuff
+(id) alloc
{
	@synchronized(self)
	{
		NSAssert(instanceOfGameTrackingObj == nil, @"Attempted to allocate a second instance of the singleton: GameKitHelper");
		instanceOfGameTrackingObj = [[super alloc] retain];
		return instanceOfGameTrackingObj;
	}
	
	// to avoid compiler warning
	return nil;
}

+(GameTrackingObj*) sharedGameTrackingObj
{
	@synchronized(self)
	{
		if (instanceOfGameTrackingObj == nil)
		{
			[[GameTrackingObj alloc] init];
		}
		
		return instanceOfGameTrackingObj;
	}
	
	// to avoid compiler warning
	return nil;
}


- (id) init {
    self = [super init];
    self.tapIteration = [NSMutableArray array];
    self.seed = [NSMutableArray array];
    self.score = 0;
    return self;
    
}

- (void) addTapIteration:(int32_t) iteration{
    if (self.tapIteration == nil) {
        self.tapIteration = [NSMutableArray array];
    }
    [self.tapIteration addObject:@(iteration)];
}


- (void) addSeed:(double) seme{
    if (self.seed == nil) {
        self.seed = [NSMutableArray array];
    }
    [self.seed addObject:@(seme)];
}


-(void) addScore:(int32_t)score{
    self.score=score;
}


- (void) clear {
    [self.tapIteration removeAllObjects];
    [self.seed removeAllObjects];
}

@end
