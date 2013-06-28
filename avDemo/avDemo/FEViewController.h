//
//  FEViewController.h
//  avDemo
//
//  Created by chenjianjun on 13-6-7.
//  Copyright (c) 2013年 free. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#include "webrtc/modules/video_render/ios/render_view.h"

@interface FEViewController : UIViewController
{
}

@property (strong,nonatomic) RenderView* view_remote;
@property (retain, nonatomic) IBOutlet UIView *visionView;

@end
