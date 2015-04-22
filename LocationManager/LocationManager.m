//
//  LocationManager.m
//  LocationManager
//
//  Created by PFei_He on 15/4/22.
//  Copyright (c) 2015年 PF-Lib. All rights reserved.
//
//  https://github.com/PFei-He/LocationManager
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_7_1
    #define kRequestAlwaysAuthorization(locationManager) [locationManager requestAlwaysAuthorization];
    #define kRequestWhenInUseAuthorization(locationManager) [locationManager requestWhenInUseAuthorization];
#else
    #define kRequestAlwaysAuthorization(locationManager) ((void)0)
    #define kRequestWhenInUseAuthorization(locationManager) ((void)0)
#endif

#import "LocationManager.h"
#import <CoreLocation/CoreLocation.h>
#import <MapKit/MapKit.h>

typedef void(^nameBlock)(NSString *, float, float);

@interface LocationManager () <CLLocationManagerDelegate>

///定位
@property (nonatomic, strong) CLLocationManager *locationManager;

///地名
@property (nonatomic, copy) nameBlock nameBlock;

@end

@implementation LocationManager

#pragma mark - Initialization Methods

- (instancetype)init
{
    self = [super init];
    if (self) {
        if (!_locationManager) _locationManager = [[CLLocationManager alloc] init];
    }
    return self;
}

#pragma mark - Views Methods

- (void)setupAlertViewWithTitle:(NSString *)title message:(NSString *)message
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
}

#pragma mark - Public Methods

- (void)startUpdatingLocation;
{
    if ([CLLocationManager locationServicesEnabled]) {
        _locationManager  = [[CLLocationManager alloc] init];
        _locationManager.delegate = self;
        _locationManager.desiredAccuracy = kCLLocationAccuracyBest;
        _locationManager.distanceFilter = 1000.f;
        kRequestAlwaysAuthorization(_locationManager)
        [_locationManager startUpdatingLocation];
    } else {
        [self setupAlertViewWithTitle:@"提示" message:@"请打开位置服务"];
    }
}

#pragma mark -

//获取地名
- (void)getPlacemarkNameUsingBlock:(void (^)(NSString *, float, float))block
{
    _nameBlock = block;
}

#pragma mark - CLLocationManagerDelegate Methods

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
    //通过地理编码获取地址
    CLGeocoder *geocoder = [[CLGeocoder alloc] init];
    CLGeocodeCompletionHandler handle = ^(NSArray *placemarks, NSError *error)
    {
        for (CLPlacemark *placemark in placemarks) {
            if ([_delegate respondsToSelector:@selector(locationManager:getPlacemarkName:longitude:latitude:)]) {
                [_delegate locationManager:self getPlacemarkName:placemark.name longitude:newLocation.coordinate.longitude latitude:newLocation.coordinate.latitude];
            } else if (_nameBlock) {
                _nameBlock(placemark.name, newLocation.coordinate.longitude, newLocation.coordinate.latitude);
            }
        }
    };
    [geocoder reverseGeocodeLocation:newLocation completionHandler:handle];
    [manager stopUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    [self setupAlertViewWithTitle:error.localizedDescription message:error.localizedFailureReason];
}

@end
