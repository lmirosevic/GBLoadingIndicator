//
//  GBLoadingIndicatorConcrete.m
//  Russia
//
//  Created by Luka Mirosevic on 14/07/2013.
//  Copyright (c) 2013 Goonbee. All rights reserved.
//

#import "GBLoadingIndicatorConcrete.h"

@interface GBLoadingIndicatorConcrete ()

@property (strong, nonatomic) NSMutableArray                                *internalRegisteredViews;

@end

@implementation GBLoadingIndicatorConcrete

#pragma mark - ca

-(NSMutableArray *)internalRegisteredViews {
    if (!_internalRegisteredViews) {
        _internalRegisteredViews = [NSMutableArray new];
    }
    
    return _internalRegisteredViews;
}

-(NSArray *)registeredViews {
    return [self.internalRegisteredViews copy];
}

#pragma mark - API

-(void)registerViews:(NSArray *)views started:(GBLoadingIndicatorStartedLoadingBlock)started finished:(GBLoadingIndicatorFinishedLoadingBlock)finished failed:(GBLoadingIndicatorFailedLoadingBlock)failed {
    [self registerViews:views];
    self.startedLoadingHandler = started;
    self.finishedLoadingHandler = finished;
    self.failedLoadingHandler = failed;
}

-(void)registerView:(UIView *)view {
    if ([view isKindOfClass:UIView.class]) {
        [self.internalRegisteredViews addObject:view];
    }
}

-(void)registerViews:(NSArray *)views {
    for (id view in views) {
        [self registerView:view];
    }
}

-(void)unregisterView:(UIView *)view {
    [self.internalRegisteredViews removeObject:view];
}

-(void)unregisterAllViews {
    //it's safe because on subsequent access it will be lazily recreated if necessary
    self.internalRegisteredViews = nil;
}

-(void)startedLoad {
    if (self.startedLoadingHandler) self.startedLoadingHandler(self.registeredViews);
}

-(void)finishedLoad {
    if (self.finishedLoadingHandler) self.finishedLoadingHandler(self.registeredViews, YES);
}

-(void)failedLoad {
    if (self.finishedLoadingHandler) self.finishedLoadingHandler(self.registeredViews, NO);
    if (self.failedLoadingHandler) self.failedLoadingHandler(self.registeredViews);
}

@end
