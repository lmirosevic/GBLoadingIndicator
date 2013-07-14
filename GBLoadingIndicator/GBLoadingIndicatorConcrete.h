//
//  GBLoadingIndicatorConcrete.h
//  Russia
//
//  Created by Luka Mirosevic on 14/07/2013.
//  Copyright (c) 2013 Goonbee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "GBLoadingIndicator_Types.h"

@interface GBLoadingIndicatorConcrete : NSObject

@property (copy, nonatomic, readonly) NSArray                               *registeredViews;

@property (copy, nonatomic) GBLoadingIndicatorStartedLoadingBlock           startedLoadingHandler;
@property (copy, nonatomic) GBLoadingIndicatorFinishedLoadingBlock          finishedLoadingHandler;
@property (copy, nonatomic) GBLoadingIndicatorFailedLoadingBlock            failedLoadingHandler;

-(void)registerView:(UIView *)view;
-(void)registerViews:(NSArray *)views;
-(void)unregisterView:(UIView *)view;
-(void)unregisterAllViews;

//all in one combo
-(void)registerViews:(NSArray *)views started:(GBLoadingIndicatorStartedLoadingBlock)started finished:(GBLoadingIndicatorFinishedLoadingBlock)finished failed:(GBLoadingIndicatorFailedLoadingBlock)failed;

//call these when things happen
-(void)didStartLoad;
-(void)didFinishLoad;
-(void)didFailLoad;

@end
