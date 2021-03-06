//
//  GBLoadingIndicator_StandardHandlers.h
//  Russia
//
//  Created by Luka Mirosevic on 14/07/2013.
//  Copyright (c) 2013 Goonbee. All rights reserved.
//

#import "GBLoadingIndicator.h"

#import "GBLoadingIndicator_Types.h"

#import <UIKit/UIKit.h>
#import <objc/runtime.h>

static NSTimeInterval const kGBLoadingIndicatorFadeViewsAnimationDuration = 0.2;
static char bigWhiteSpinnerKey;
static char errorViewKey;

#pragma mark - StartedLoadingBlock

static GBLoadingIndicatorStartedLoadingBlock const kGBLoadingIndicatorBlockInstantlyHideViews = ^(NSArray *registeredViews) {
    for (UIView *view in registeredViews) {
        if ([view isKindOfClass:UIView.class]) {
            view.hidden = YES;//we can't just set alpha to 0 because users might still be able to touch the view
        }
    }
};

#define kGBLoadingIndicatorBlockAddSpinner GBLoadingIndicatorBlockFactoryAddSpinner(self)
static inline GBLoadingIndicatorStartedLoadingBlock GBLoadingIndicatorBlockFactoryAddSpinner(id caller) {
    __weak id weakCaller = caller;
    return ^(NSArray *registeredViews) {
        if ([weakCaller isKindOfClass:UIViewController.class]) {
            //get the caller's view if he's a UIViewController
            UIView *targetView = ((UIViewController *)weakCaller).view;
            
            //creater a spinner for him
            UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
            spinner.userInteractionEnabled = NO;
            spinner.hidesWhenStopped = YES;
            [spinner startAnimating];
            
            //add the spinner to the center of the view
            spinner.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
            
            spinner.frame = CGRectMake(targetView.bounds.origin.x + (targetView.bounds.size.width - spinner.frame.size.width)/2,
                                       targetView.bounds.origin.y + (targetView.bounds.size.height - spinner.frame.size.height)/2,
                                       spinner.frame.size.width,
                                       spinner.frame.size.height);
            
            [targetView addSubview:spinner];
            [targetView bringSubviewToFront:spinner];
            
            //remember the spinner
            objc_setAssociatedObject(weakCaller, &bigWhiteSpinnerKey, spinner, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
        }
    };
};

#define kGBLoadingIndicatorBlockRemoveErrorView GBLoadingIndicatorBlockFactoryRemoveErrorView(self)
static inline GBLoadingIndicatorStartedLoadingBlock GBLoadingIndicatorBlockFactoryRemoveErrorView(id caller) {
    __weak id weakCaller = caller;
    return ^(NSArray *registeredViews) {
        if ([weakCaller isKindOfClass:UIViewController.class]) {
            
            //get the associated object
            UIView *targetView = ((UIViewController *)weakCaller).view;
            UIView *errorView = objc_getAssociatedObject(weakCaller, &errorViewKey);

            //remove it from the view
            [errorView removeFromSuperview];

            //destroy the view
            objc_setAssociatedObject(targetView, &errorViewKey, nil, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
        }
    };
};

#define kGBLoadingIndicatorBlockShowSpinnerRemoveErrorHideViews ^(NSArray *registeredViews) { \
    kGBLoadingIndicatorBlockRemoveErrorView(registeredViews); \
    kGBLoadingIndicatorBlockAddSpinner(registeredViews); \
    kGBLoadingIndicatorBlockInstantlyHideViews(registeredViews); \
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

#define kGBLoadingIndicatorBlockAddErrorView(errorView) GBLoadingIndicatorBlockFactoryAddErrorView(self, errorView)
static inline GBLoadingIndicatorFinishedLoadingBlock GBLoadingIndicatorBlockFactoryAddErrorView(id caller, UIView *errorView) {
    __weak id weakCaller = caller;
    return ^(NSArray *registeredViews, BOOL success) {
        if (!success) {
            if ([weakCaller isKindOfClass:UIViewController.class]) {
                //get the caller's view if he's a UIViewController
                UIView *targetView = ((UIViewController *)weakCaller).view;
                
                errorView.frame = CGRectMake(targetView.bounds.origin.x + (targetView.bounds.size.width - errorView.frame.size.width)/2,
                                             targetView.bounds.origin.y + (targetView.bounds.size.height - errorView.frame.size.height)/2,
                                             errorView.frame.size.width,
                                             errorView.frame.size.height);

                [targetView addSubview:errorView];
                [targetView bringSubviewToFront:errorView];

                //attach it to the VC
                objc_setAssociatedObject(weakCaller, &errorViewKey, errorView, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
            }
        }
    };
};

#define kGBLoadingIndicatorBlockRemoveSpinnerAndShowViewsOrErrorView(errorView) ^(NSArray *registeredViews, BOOL success) { \
    kGBLoadingIndicatorBlockRemoveSpinner(registeredViews, success); \
    kGBLoadingIndicatorBlockFadeInViews(registeredViews, success); \
    kGBLoadingIndicatorBlockAddErrorView(errorView)(registeredViews, success); \
}

#define kGBLoadingIndicatorBlockRemoveSpinnerAndShowViews ^(NSArray *registeredViews, BOOL success) { \
    kGBLoadingIndicatorBlockRemoveSpinner(registeredViews, success); \
    kGBLoadingIndicatorBlockFadeInViews(registeredViews, success); \
}

