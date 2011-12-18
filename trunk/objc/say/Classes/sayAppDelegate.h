//
//  sayAppDelegate.h
//  say
//
//  Created by zuohaitao on 11-12-10.
//  Copyright 2011 zuohaitao@doaob. All rights reserved.
//

#import <UIKit/UIKit.h>

@class sayViewController;

@interface sayAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    sayViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet sayViewController *viewController;

@end

