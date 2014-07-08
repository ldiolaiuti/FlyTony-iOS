#include "AppDelegate.h"
#include "FakeSplash.h"
#include "SimpleAudioEngine.h"
#include "GKHWrapperCpp.h"
#include "ProtocolAnalytics.h"
#include "PluginManager.h"
#include "PluginProtocol.h"

// The app key of flurry
#define FLURRY_KEY_IOS          "36DNQ9MNW36CYV7ZKSY8"
#define FLURRY_KEY_ANDROID      "4T3BMRNBYQDZNZF88NST"

USING_NS_CC;

cocos2d::plugin::ProtocolAnalytics* g_pAnalytics = NULL;
std::string s_strAppKey = "";

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    pEGLView->setDesignResolutionSize(640, 960, kResolutionNoBorder);

    // turn on display FPS
    pDirector->setDisplayStats(false);

    CCSize screenSize = pEGLView->getFrameSize();

       std::vector<std::string> searchPath;

       if (screenSize.width > 640)
       {
           searchPath.push_back("hd");
           pDirector->setContentScaleFactor(2);
       }
       else
       {
           searchPath.push_back("sd");
           pDirector->setContentScaleFactor(1);
       }

    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = FakeSplash::scene();
    
    //log player in game center
    GKHWrapperCpp gkh;
    gkh.authenticateLocalPlayer();
    
    std::string flurryKey = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    flurryKey = FLURRY_KEY_IOS;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    flurryKey = FLURRY_KEY_ANDROID;
#endif
    
    cocos2d::plugin::PluginProtocol* pPlugin = cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsFlurry");
    s_strAppKey = flurryKey;

    g_pAnalytics = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(pPlugin);

    //Flurry analytics plugin
    g_pAnalytics->startSession(flurryKey.c_str());


    // run
    pDirector->runWithScene(pScene);

    return true;
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    g_pAnalytics->stopSession();

    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    
    std::string flurryKey = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    flurryKey = FLURRY_KEY_IOS;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    flurryKey = FLURRY_KEY_ANDROID;
#endif
    
    g_pAnalytics->startSession(flurryKey.c_str());
}
