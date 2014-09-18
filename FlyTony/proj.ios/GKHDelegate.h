//
//  GKHDelegate.h
//  HelloCpp
//
//  Created by Steve Barnegren on 29/11/2013.
//
//

#ifndef __HelloCpp__GKHDelegate__
#define __HelloCpp__GKHDelegate__

#include <iostream>
#include <vector>
#include <map>

//forward declare structs
struct GKPlayerCpp;
struct GKScoreCpp;
struct GKAchievementCpp;
struct GKChallengeCpp;

class GKHDelegate {
    
public:
    
    GKHDelegate();
    bool _debug;
    void setDebug(bool debug){_debug = debug;};
    
    // Players
    virtual void onLocalPlayerAuthenticationChanged();
    virtual void onFriendsListRecieved(std::vector<std::string> friends);
    virtual void onPlayerInfoReceived(std::vector<GKPlayerCpp> playerInfo);
    
    // Scores
    virtual void onScoresSubmitted(bool success);
    virtual void onScoresReceived(std::vector<GKScoreCpp> scores);
    
    // Achievements
    virtual void onAchievementReported(GKAchievementCpp achievement);
    virtual void onResetAchievements(bool success);
    virtual void onAchievementsLoaded(std::map<std::string, GKAchievementCpp> achievements);

    // Game Centre Views
    virtual void onLeaderboardViewDismissed();
    virtual void onAchievementsViewDismissed();
    
    //MatchMaking
    virtual void onChallengeLoaded(std::map<std::string, GKChallengeCpp> challenges);
    virtual void onChallengeCompleted(GKChallengeCpp challengeCpp);
    virtual void onChallengeSended();
    virtual void onSendingChallengeError();
    
};

#endif /* defined(__HelloCpp__GKHDelegate__) */
