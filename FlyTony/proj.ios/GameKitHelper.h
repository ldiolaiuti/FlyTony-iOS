//
//  GameKitHelper.h
//
//  Created by Steffen Itterheim on 05.10.10.
//  Copyright 2010 Steffen Itterheim. All rights reserved.
//

//#import "cocos2d.h"
#include <iostream>
#include <vector>
#import <GameKit/GameKit.h>
#import "GKHDelegate.h"
#import "GKHWrapperCpp.h"
#import "GameTrackingObj.h"

#define kHighScoreLeaderboardCategory @"eu.balzo.tonyfly.leaderboard"
#define kAchievementsFileName @"Achievements.plist"
#define kAchievementsResourceName @"Achievements"


@protocol GameKitHelperProtocol

-(void) onLocalPlayerAuthenticationChanged;

-(void) onFriendListReceived:(NSArray*)friends;
-(void) onPlayerInfoReceived:(NSArray*)players;

-(void) onScoresSubmitted:(bool)success;
-(void) onScoresReceived:(NSArray*)scores;
-(void) onScoresOfFriendToChallengeListRiceived:(NSArray*) scores;
-(void) onPayerInfoRiceived:(NSArray*) players;

-(void) onAchievementReported:(GKAchievement*)achievement;
-(void) onAchievementsLoaded:(NSDictionary*)achievements;
-(void) onResetAchievements:(bool)success;

-(void) onMatchFound:(GKMatch*)match;
-(void) onPlayersAddedToMatch:(bool)success;
-(void) onReceivedMatchmakingActivity:(NSInteger)activity;

-(void) onPlayerConnected:(NSString*)playerID;
-(void) onPlayerDisconnected:(NSString*)playerID;
-(void) onStartMatch;
-(void) onReceivedData:(NSData*)data fromPlayer:(NSString*)playerID;

-(void) onMatchmakingViewDismissed;
-(void) onMatchmakingViewError;
-(void) onLeaderboardViewDismissed;
-(void) onAchievementsViewDismissed;



@end


@interface GameKitHelper : NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKMatchmakerViewControllerDelegate, GKMatchDelegate, GKChallengeEventHandlerDelegate>
{
	id<GameKitHelperProtocol> delegate;
	bool isGameCenterAvailable;
	NSError* lastError;
	
	NSMutableDictionary* achievements;
	NSMutableDictionary* cachedAchievements;
	
	GKMatch* currentMatch;
	bool matchStarted;
}

//@property (nonatomic, retain) id<GameKitHelperProtocol> delegate;
@property (nonatomic, readonly) bool isGameCenterAvailable;
@property (nonatomic, readonly) NSError* lastError;
@property (nonatomic, readonly) NSMutableDictionary* achievements;
@property (nonatomic, readonly) GKMatch* currentMatch;
@property (nonatomic, readonly) bool matchStarted;
@property (nonatomic, readwrite) BOOL includeLocalPlayerScore;

/** returns the singleton object, like this: [GameKitHelper sharedGameKitHelper] */
+(GameKitHelper*) sharedGameKitHelper;

// Player authentication, info
-(void) authenticateLocalPlayer;
-(void) getLocalPlayerFriends;
-(void) getPlayerInfo:(NSArray*)players;
-(BOOL) isLocalPlayerAuthenticated;

// Scores
-(void) submitScore:(int64_t)score category:(NSString*)category;

-(void) retrieveScoresForPlayers:(NSArray*)players
						category:(NSString*)category
						   range:(NSRange)range
					 playerScope:(GKLeaderboardPlayerScope)playerScope
					   timeScope:(GKLeaderboardTimeScope)timeScope;
-(void) retrieveTopTenAllTimeGlobalScoresForCatagory:(NSString*)catagory;
-(void) onScoresOfFriendToChallengeListRiceived:(NSArray*) scores;
-(void) onPayerInfoRiceived:(NSArray*) players;

// Achievements
-(GKAchievement*) getAchievementByID:(NSString*)identifier;
-(void) reportAchievementWithID:(NSString*)identifier percentComplete:(float)percent showCompletionBanner:(BOOL)showBanner;
-(void) resetAchievements;
-(void) reportCachedAchievements;
-(void) saveCachedAchievements;

// Matchmaking
-(void) disconnectCurrentMatch;
-(void) findMatchForRequest:(GKMatchRequest*)request;
-(void) addPlayersToMatch:(GKMatchRequest*)request;
-(void) cancelMatchmakingRequest;
-(void) queryMatchmakingActivity;
-(void) loadChallenges;

// Game Center Views
-(void) showLeaderboard;
-(void) showAchievements;
-(void) showMatchmakerWithInvite:(GKInvite*)invite;
-(void) showMatchmakerWithRequest:(GKMatchRequest*)request;
-(void) sendDataToAllPlayers:(void*)data length:(NSUInteger)length;

//delegate
-(void)setDelegate:(GKHDelegate*)gkhDelegate;


//Challenge
-(void)findScoresOfFriendToChallenge;
-(void)sendScoreChallengeToPlayers:(NSArray *) players withScore:(int64_t)score message:(NSString *)message;
-(void)sendScoreChallengeToPlayers:(NSArray *) players score:(int64_t)score context:(int64_t)context message:(NSString *)message;
-(void)sendScoreChallengeToPlayers:(NSArray *)players
                         withScore:(int64_t)score
                           message:(NSString *)message
               withGameTrackingObj:(GameTrackingObj*)gameTrackingObj;
-(void)localPlayerDidCompleteChallenge:(GKChallenge *)challenge;
-(void)localPlayerDidReceiveChallenge:(GKChallenge *)challenge;
-(void)declineChallengeByContext:(int64_t) context;



//C++/Objective-C type converters

-(std::vector<std::string>)convertNSArrayOfStringToCppVector:(NSArray*)array;
-(NSArray*)convertCppVectorOfStringToNSArray:(std::vector<std::string>)vector;
-(GKPlayerCpp)convertGKPLayerToGKPLayerStruct:(GKPlayer*)player;
-(std::vector<GKPlayerCpp>)convertNSArrayOfGKPlayersToCppVector:(NSArray*)array;
-(GKScoreCpp)convertGKScoreToGKScoreStruct:(GKScore*)gkScore;
-(std::vector<GKScoreCpp>)convertNSArrayOfGKScoresToCppVector:(NSArray*)array;
-(GKAchievementCpp)convertGKAchievementToGKAchievementStruct:(GKAchievement*)a;
-(std::map<std::string, GKAchievementCpp>)convertNSDictionaryOfGKAchievementTostdMap:(NSDictionary*)dict;
-(std::map<std::string, GKChallengeCpp>)convertNSArrayOfGKChallengeTostdMap:(NSArray*)array;
-(GKChallengeCpp)convertGKChallengeToGKChallengeCppStruct:(GKChallenge *)challenge;




@end
