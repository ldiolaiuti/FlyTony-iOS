//
//  GameTrackingEngine.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 27/03/14.
//
//

#import "AFNetworking.h"
#import "AFURLRequestSerialization.h"
#include "GameTrackingObj.h"


@class GameTrackingObj;
typedef void (^GameTrackingObjResponse) (GameTrackingObj *gameTrackingObj);
typedef void (^GameTrackingObjSentSuccessfully)();
typedef void (^GameTrackingObjError)(NSError *error);

@interface GameTrackingEngine : AFHTTPRequestOperationManager

+ (GameTrackingEngine *)sharedClient;
- (void) retrieveGameTrackingDetailsForKey:(int64_t) key onSuccess:(GameTrackingObjResponse) responceBlock onFailure:(GameTrackingObjError) errorBlock;
- (void) sendGameTrackingInfo:(GameTrackingObj*) gameTrackingObj challengeId:(NSNumber*) challengeId onSuccess:(GameTrackingObjSentSuccessfully) successBlock onFailure:(GameTrackingObjError) errorBlock;
- (void) sendGameTrackingInfo:(GameTrackingObj*) gameTrackingObj challengeId:(NSNumber*) challengeId playerId:(NSString*) playerId onSuccess:(GameTrackingObjSentSuccessfully) successBlock onFailure:(GameTrackingObjError) errorBlock;

@end
