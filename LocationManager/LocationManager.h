//
//  LocationManager.h
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

#import <Foundation/Foundation.h>

/**
 *  强弱引用转换，用于解决代码块（block）与强引用self之间的循环引用问题
 *  调用方式: `@weakify_self`实现弱引用转换，`@strongify_self`实现强引用转换
 *
 *  示例：
 *  @weakify_self
 *  [obj block:^{
 *  @strongify_self
 *      self.property = something;
 *  }];
 */
#ifndef	weakify_self
    #if __has_feature(objc_arc)
        #define weakify_self autoreleasepool{} __weak __typeof__(self) weakSelf = self;
    #else
        #define weakify_self autoreleasepool{} __block __typeof__(self) blockSelf = self;
    #endif
#endif
#ifndef	strongify_self
    #if __has_feature(objc_arc)
        #define strongify_self try{} @finally{} __typeof__(weakSelf) self = weakSelf;
    #else
        #define strongify_self try{} @finally{} __typeof__(blockSelf) self = blockSelf;
    #endif
#endif

/**
 *  强弱引用转换，用于解决代码块（block）与强引用对象之间的循环引用问题
 *  调用方式: `@weakify(object)`实现弱引用转换，`@strongify(object)`实现强引用转换
 *
 *  示例：
 *  @weakify(object)
 *  [obj block:^{
 *      @strongify(object)
 *      strong_object = something;
 *  }];
 */
#ifndef	weakify
    #if __has_feature(objc_arc)
        #define weakify(object)	autoreleasepool{} __weak __typeof__(object) weak##_##object = object;
    #else
        #define weakify(object)	autoreleasepool{} __block __typeof__(object) block##_##object = object;
    #endif
#endif
#ifndef	strongify
    #if __has_feature(objc_arc)
        #define strongify(object) try{} @finally{} __typeof__(object) strong##_##object = weak##_##object;
    #else
        #define strongify(object) try{} @finally{} __typeof__(object) strong##_##object = block##_##object;
    #endif
#endif

@class LocationManager;

@protocol LocationManagerDelegate <NSObject>

@optional

/**
 *  @brief 获取地标信息
 *  @param placemarkName: 地名
 *  @param longitude: 经度
 *  @param latitude: 纬度
 *  @warning 由于系统自带的地理编码不完整（开发者可自行测试），对于给予的坐标不一定可以返回相应的地址，请谨慎使用
 */
- (void)locationManager:(LocationManager *)locationManager getPlacemarkName:(NSString *)placemarkName longitude:(float)longitude latitude:(float)latitude;

@end

@interface LocationManager : NSObject

///代理
@property (nonatomic, weak) id<LocationManagerDelegate> delegate;

/**
 *  @brief 开始位置请求（请求完成自动关闭）
 */
- (void)startUpdatingLocation;

/**
 *  @brief 获取地标信息
 *  @param placemarkName: 地名
 *  @param longitude: 经度
 *  @param latitude: 纬度
 *  @warning 由于系统自带的地理编码不完整（开发者可自行测试），对于给予的坐标不一定可以返回相应的地址，请谨慎使用
 */
- (void)getPlacemarkNameUsingBlock:(void (^)(NSString *placemarkName, float longitude, float latitude))block;

@end
