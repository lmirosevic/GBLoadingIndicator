//
//  GBLoadingIndicator.m
//  Russia
//
//  Created by Luka Mirosevic on 14/07/2013.
//  Copyright (c) 2013 Goonbee. All rights reserved.
//

#import "GBLoadingIndicator.h"

@interface GBLoadingIndicatorManager : NSObject
@end

@interface GBLoadingIndicatorManager ()

@property (strong, nonatomic) NSMutableDictionary                           *callers;

@end

@implementation GBLoadingIndicatorManager

#pragma mark - CA

+(GBLoadingIndicatorManager *)sharedManager {
    static GBLoadingIndicatorManager *_sharedManager;
    @synchronized(self) {
        if (!_sharedManager) {
            _sharedManager = [self new];
        }
    }
    
    return _sharedManager;
}

- (id)init {
    if (self = [super init]) {
        self.callers = [NSMutableDictionary new];
    }
    return self;
}

#pragma mark - API

GBLoadingIndicatorConcrete * GBLoadingIndicator(id caller) {
    return [[GBLoadingIndicatorManager sharedManager] _concreteLoadingIndicatorForCaller:caller];
}

#pragma mark - util

-(GBLoadingIndicatorConcrete *)_concreteLoadingIndicatorForCaller:(id)caller {
    NSString *pointer = [self.class _object2String:caller];
    
    id existingConcreteLoadingIndicator = self.callers[pointer];
    
    if (!existingConcreteLoadingIndicator) {
        existingConcreteLoadingIndicator = [GBLoadingIndicatorConcrete new];
        self.callers[pointer] = existingConcreteLoadingIndicator;
    }
    
    return existingConcreteLoadingIndicator;
}

+(NSString *)_object2String:(id)object {
    return [NSString stringWithFormat:@"%p", object];
}

@end
