//
//  GTOWrapperCpp.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 04/04/14.
//
//

#ifndef __GTOWrapperCpp__GameTracking__
#define __GTOWrapperCpp__GameTracking__

#include <iostream>
#include <vector>

class GTODelegate;


struct GameTrackingObjCpp{
public:
    std::vector<int> *tapIteration;
    double seed;
    int score;
};


class GTOWrapperCpp{
    
public:
    GTOWrapperCpp();
    
    void setDelegate(GTODelegate *delegate);
    
    //register movement
    void addTapIteration(int iteration);
    void addSeed(double seed);
    void addScore(int score);
    
    //clear vector
    void clear();
    
    //Challenge
    void sendScoreChallengeToPlayers(std::vector<std::string> players, int score, char* message);
    void retriveChallenge(uint64_t context);
    
};
#endif