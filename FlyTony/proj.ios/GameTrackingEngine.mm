//
//  GameTrackingEngine.m
//  FlyTony
//
//  Created by Luca Diolaiuti on 27/03/14.
//
//

#import "GameTrackingEngine.h"

#define kSeedKey @"SeedArray"
#define kTapIteration @"TapArray"
#define kScore @"ScoreKey"

@implementation GameTrackingEngine

static GameTrackingEngine *istanceOfGameTrackingEngine;

+(GameTrackingEngine *)sharedClient {
    static GameTrackingEngine *sharedClient = nil;
    static dispatch_once_t onceToken; dispatch_once(&onceToken, ^{
        sharedClient = [[GameTrackingEngine alloc] initWithBaseURL:[NSURL
                                                                    URLWithString: BASE_URL]];
    });
    return sharedClient;
}

- (id) initWithBaseURL:(NSURL *)url {
    self = [super initWithBaseURL:url];
    
    if (self) {
        self.responseSerializer = [AFJSONResponseSerializer serializer];
        self.requestSerializer = [AFJSONRequestSerializer serializer];
    }
    
    return self;
}


- (void) retrieveGameTrackingDetailsForKey:(int64_t) key
                                 onSuccess:(GameTrackingObjResponse) responceBlock
                                 onFailure:(GameTrackingObjError) errorBlock {
    
    NSDictionary *params = @{@"challengeId": [NSNumber numberWithInt:key]};
    [self GET:@"ChallengesAPI.php"
       parameters:params
          success:^(AFHTTPRequestOperation *operation, id responceObject) {
              
              if (responceBlock != nil &&
                  responceObject != nil &&
                  [responceObject isKindOfClass:[NSDictionary class]]) {
                  
                  GameTrackingObj *gameTrackingObj = [[GameTrackingObj alloc] init];
                  NSArray *array =responceObject[kTapIteration];
                  gameTrackingObj.tapIteration = [array mutableCopy];
                  array=(NSMutableArray *)responceObject[kSeedKey];
                  gameTrackingObj.seed = [array mutableCopy];
                  gameTrackingObj.score = [responceObject[kScore] intValue];
                  
                  responceBlock(gameTrackingObj);
              }
          } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
              if (errorBlock != nil) {
                  errorBlock(error);
              }
          }];
}
- (void) sendGameTrackingInfo:(GameTrackingObj*) gameTrackingObj
                  challengeId:(NSNumber*) challengeId
                    onSuccess:(GameTrackingObjSentSuccessfully) successBlock
                    onFailure:(GameTrackingObjError) errorBlock {

    
    NSDictionary *params =
    @{kTapIteration:
          gameTrackingObj.tapIteration,
      kSeedKey:
          gameTrackingObj.seed,
      kScore:
          [NSNumber numberWithInt:gameTrackingObj.score]};
    
    NSLog(@"\nChallengeID: %d", challengeId.intValue);
    
    
    NSString *postPath = [NSString stringWithFormat:@"ChallengesAPI.php?challengeId=%d", challengeId.intValue];
    [self POST:postPath
        parameters:params
        success:^(AFHTTPRequestOperation *operation, id responceObject){
               if (successBlock != nil) {
                   successBlock();
               }
           }
        failure:^(AFHTTPRequestOperation *operation, NSError *error){
               if (errorBlock != nil) {
                   errorBlock(error);
               }
           }];
}

- (void) sendGameTrackingInfo:(GameTrackingObj*) gameTrackingObj
                  challengeId:(NSNumber*) challengeId
                     playerId:(NSString *)playerId
                    onSuccess:(GameTrackingObjSentSuccessfully)successBlock
                    onFailure:(GameTrackingObjError)errorBlock{
    
    NSDictionary *params =
    @{kTapIteration:
          gameTrackingObj.tapIteration,
      kSeedKey:
          gameTrackingObj.seed,
      kScore:
          [NSNumber numberWithInt:gameTrackingObj.score]};
    
    
    NSString *postPath = [NSString stringWithFormat:@"ChallengesAPI.php?challengeId=%d-playerID=%s", challengeId.intValue, [playerId UTF8String]];
    [self POST:postPath
    parameters:params
       success:^(AFHTTPRequestOperation *operation, id responceObject){
           if (successBlock != nil) {
               successBlock();
           }
       } failure:^(AFHTTPRequestOperation *operation, NSError *error){
           if (errorBlock != nil) {
               errorBlock(error);
           }
       }];
}

@end