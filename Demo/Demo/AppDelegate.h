//
//  AppDelegate.h
//  Demo
//
//  Created by PFei_He on 14-12-17.
//  Copyright (c) 2014年 PF-Lib. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LocationManager.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

///定位
@property (strong, nonatomic) LocationManager *locationManager;

@end
