//
//  GBLoadingIndicator_StandardHandlers.h
//  Russia
//
//  Created by Luka Mirosevic on 14/07/2013.
//  Copyright (c) 2013 Goonbee. All rights reserved.
//

#import "GBLoadingIndicator.h"

#import <UIKit/UIKit.h>
#import <objc/runtime.h>

static NSTimeInterval const kGBLoadingIndicatorFadeViewsAnimationDuration = 0.2;
static char bigWhiteSpinnerKey;

#pragma mark - StartedLoadingBlock

static GBLoadingIndicatorStartedLoadingBlock const kGBLoadingIndicatorBlockInstantlyHide = ^(NSArray *registeredViews) {
    for (UIView *view in registeredViews) {
        if ([view isKindOfClass:UIView.class]) {
            view.hidden = YES;//we can't just set alpha to 0 because users might still be able to touch the view
        }
    }
};

#define kGBLoadingIndicatorBlockAddSpinnerTo(view) GBLoadingIndicatorBlockFactoryAddSpinner(self, view)
static inline GBLoadingIndicatorStartedLoadingBlock GBLoadingIndicatorBlockFactoryAddSpinner(id caller, UIView *view) {
    __weak id weakCaller = caller;
    return ^(NSArray *registeredViews) {
        if ([weakCaller isKindOfClass:UIViewController.class]) {
            //get the caller's view if he's a UIViewController
//            UIView *targetView = ((UIViewController *)weakCaller).view;
            
            //foo decide what im doing
            UIView *targetView = view;
            
            //creater a spinner for him
            UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
            spinner.userInteractionEnabled = NO;
            spinner.hidesWhenStopped = YES;
            [spinner startAnimating];
            
            //add the spinner to the center of the view
            spinner.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
            spinner.center = targetView.center;
            [targetView addSubview:spinner];
            
            //remember the spinner
            objc_setAssociatedObject(weakCaller, &bigWhiteSpinnerKey, spinner, OBJC_ASSOCIATION_RETAIN_NONATOMIC);            
        }
    };
};

#define kGBLoadingIndicatorBlockHideAndShowSpinner(view) ^(NSArray *registeredViews) { \
    GBLoadingIndicatorBlockFactoryAddSpinner(self, view)(registeredViews); \
    kGBLoadingIndicatorBlockInstantlyHide(registeredViews); \
}

#pragma mark - FinishedLoadingBlock

static GBLoadingIndicatorFinishedLoadingBlock const kGBLoadingIndicatorBlockFadeInViews = ^(NSArray *registeredViews, BOOL success) {
    if (success) {
        for (UIView *view in registeredViews) {
            if ([view isKindOfClass:UIView.class]) {
                view.alpha = 0;
                view.hidden = NO;
                
                [UIView animateWithDuration:kGBLoadingIndicatorFadeViewsAnimationDuration animations:^{
                    view.alpha = 1;
                }];
            }
        }
    }
};

#define kGBLoadingIndicatorBlockRemoveSpinner GBLoadingIndicatorBlockFactoryRemoveSpinner(self)
static inline GBLoadingIndicatorFinishedLoadingBlock GBLoadingIndicatorBlockFactoryRemoveSpinner(id caller) {
    __weak id weakCaller = caller;
    return ^(NSArray *registeredViews, BOOL success) {
        if ([weakCaller isKindOfClass:UIViewController.class]) {
            //get the associated object
            UIView *targetView = ((UIViewController *)weakCaller).view;
            UIActivityIndicatorView *spinner = objc_getAssociatedObject(weakCaller, &bigWhiteSpinnerKey);
            
            //remove it from the view
            [spinner stopAnimating];
            [spinner removeFromSuperview];
            
            //destroy the spinner
            objc_setAssociatedObject(targetView, &bigWhiteSpinnerKey, nil, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
        }
    };
};

#define kGBLoadingIndicatorBlockShowAndRemoveSpinner ^(NSArray *registeredViews, BOOL success) { \
    kGBLoadingIndicatorBlockRemoveSpinner(registeredViews, success); \
    kGBLoadingIndicatorBlockFadeInViews(registeredViews, success); \
}
