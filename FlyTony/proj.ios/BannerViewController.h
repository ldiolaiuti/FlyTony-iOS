//
//  BannerViewController.h
//  FlyTony
//
//  Created by Luca Diolaiuti on 19/03/14.
//
//


#import <UIKit/UIKit.h>

extern NSString * const BannerViewActionWillBegin;
extern NSString * const BannerViewActionDidFinish;

@interface BannerViewController : UIViewController

- (instancetype)initWithContentViewController:(UIViewController *)contentController;

@end