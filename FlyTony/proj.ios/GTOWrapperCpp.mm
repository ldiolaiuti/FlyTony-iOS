//
//  GTOWrapperCpp.m
//  FlyTony
//
//  Created by Luca Diolaiuti on 04/04/14.
//
//

#import "GTOWrapperCpp.h"
#import "GameKitHelper.h"
#import "GameTrackingEngine.h"



static GTODelegate* _delegate=NULL;

GTOWrapperCpp::GTOWrapperCpp(){
    [GameTrackingObj sharedGameTrackingObj];
}

void GTOWrapperCpp::setDelegate(GTODelegate* delegate){
    _delegate=delegate;
}


#pragma mark Record Movement

void GTOWrapperCpp::addTapIteration(int iteration){
    [[GameTrackingObj sharedGameTrackingObj] addTapIteration:iteration];
}

void GTOWrapperCpp::addSeed(double seed){
    [[GameTrackingObj sharedGameTrackingObj] addSeed:seed];
}

void GTOWrapperCpp::addScore(int score){
    [[GameTrackingObj sharedGameTrackingObj] addScore:score];
}

#pragma mark Clear

void GTOWrapperCpp::clear(){
    [[GameTrackingObj sharedGameTrackingObj] clear];
}

#pragma mark Challenge

void GTOWrapperCpp::sendScoreChallengeToPlayers(std::vector<std::string> players, int score, char* message){
    
    GameKitHelper *gameKitHelper = [GameKitHelper sharedGameKitHelper];
    GameTrackingObj *gameTrackingObj = [GameTrackingObj sharedGameTrackingObj];
    
    
    NSString* mes =[[NSString alloc] initWithUTF8String:message];
    
    NSMutableArray *array = [[[NSMutableArray alloc]init]autorelease];
    
    for (std::vector<std::string>::iterator it = players.begin(); it != players.end(); it++) {
        
        NSString *string = [NSString stringWithUTF8String:it->c_str()];
        [array addObject:string];
        
    }
    
    [gameKitHelper sendScoreChallengeToPlayers:array withScore:score message:mes withGameTrackingObj:gameTrackingObj];
}

void GTOWrapperCpp::retriveChallenge(uint64_t context){
    
    __block GameTrackingObjCpp gtoCpp;
    if (_delegate!=NULL){
        gtoCpp.tapIteration = new std::vector<int>();
        gtoCpp.seed = new std::vector<double>();
    
        [[GameTrackingEngine sharedClient]
         retrieveGameTrackingDetailsForKey:context onSuccess:^(GameTrackingObj *gameTrackingObj)
         {
             double seed;
             for (int i=0; i<[gameTrackingObj.seed count]; i++){
                 seed =[[gameTrackingObj.seed objectAtIndex:i] doubleValue];
                 NSLog(@"%f",seed);
                 gtoCpp.seed->push_back(seed);
             }
         
             int tap;
             for (int i=0; i<[gameTrackingObj.tapIteration count]; i++) {
                 tap =[[gameTrackingObj.tapIteration objectAtIndex:i] integerValue];
                 NSLog(@"%d",tap);
                 gtoCpp.tapIteration->push_back(tap);
             }
             gtoCpp.score=gameTrackingObj.score;
             _delegate->onGameTrackingObtained(gtoCpp);
         }
         onFailure:^(NSError* error){
             NSLog(@"FAIL");
             _delegate->onGameTrackingFailed();
         }];
    }
}

