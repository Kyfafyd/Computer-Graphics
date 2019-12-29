//
//  AppDelegate.h
//  HelloOpenGL
//
//  Created by Randy McLain on 2/15/17.
//  Copyright © 2017 Randy McLain. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OpenGLView.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
 OpenGLView* _glView;
}

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic, retain) IBOutlet OpenGLView * glView;
@end


