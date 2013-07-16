//
//  GBLoadingIndicator_Types.h
//  Russia
//
//  Created by Luka Mirosevic on 14/07/2013.
//  Copyright (c) 2013 Goonbee. All rights reserved.
//

typedef void(^GBLoadingIndicatorStartedLoadingBlock)(NSArray *registeredViews);
typedef void(^GBLoadingIndicatorFinishedLoadingBlock)(NSArray *registeredViews, BOOL success);
