//
//  FEViewController.m
//  avDemo
//
//  Created by chenjianjun on 13-6-7.
//  Copyright (c) 2013年 free. All rights reserved.
//
#define IOS

#import "FEViewController.h"
#include "AVInterfaceAPI.h"

UIImageView* _pview_local;

@interface FEViewController ()
{
    CAVInterfaceAPI* pInterfaceApi;
    NSInteger voiceChannel;// 音频通道
    NSInteger videoChannel;// 视频通道
    NSInteger currentCameraOrientation;
    NSInteger cameraId;
}

@end

//#define IP "192.168.1.100"
#define IP "127.0.0.1"

@implementation FEViewController

//@synthesize view_local;
@synthesize view_remote;


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    CGRect view1_frame = CGRectMake(self.view.bounds.size.width - 144,
                                    self.view.bounds.size.height - 192 - 60,
                                    144,
                                    192);
    _pview_local = [[UIImageView alloc] initWithFrame:view1_frame];
    [_pview_local setBackgroundColor:[UIColor greenColor]];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)Start:(id)sender
{
    //CGRect view1_frame = CGRectMake(self.view.bounds.size.width - 144,
    //                                self.view.bounds.size.height - 192 - 60,
    //                                144,
    //                                192);
    //self.view_local = [[RenderView alloc] initWithFrame:view1_frame];
    //[self.view_local setBackgroundColor:[UIColor greenColor]];
    
    CGRect view2_frame = CGRectMake(0,
                                    0,
                                    self.view.bounds.size.width,
                                    self.view.bounds.size.height - 60);
    self.view_remote = [[RenderView alloc] initWithFrame:view2_frame];
    [self.view_remote setBackgroundColor:[UIColor blueColor]];
    
    [self.view addSubview:_pview_local];
    [self.view addSubview:self.view_remote];
    
    [self.view sendSubviewToBack:_pview_local];
    [self.view sendSubviewToBack:self.view_remote];
    

    pInterfaceApi = new CAVInterfaceAPI();
    if (!pInterfaceApi)
    {
        return;
    }
    
    if (!pInterfaceApi->VoeInit())
    {
        return;
    }
    
    voiceChannel = pInterfaceApi->CreateVoeChannel();
    if (voiceChannel < 0)
    {
        return;
    }
    
    pInterfaceApi->SetVoeCommunicationParameters(voiceChannel, 11113, 11113, IP);
    
    VoEControlParameters param;
    
    param.ostype = OsT_IOS;
    param.optype = OpT_Speaker;
    param.enable = true;
    param.iVolume = 240;    
    pInterfaceApi->SetVoEControlParameters(param);
    
    param.optype = OpT_EC;
    param.enable = true;
    pInterfaceApi->SetVoEControlParameters(param);
    
    param.optype = OpT_AGC;
    param.enable = true;
    pInterfaceApi->SetVoEControlParameters(param);
    
    param.optype = Opt_NS;
    param.enable = true;
    pInterfaceApi->SetVoEControlParameters(param);
 
    pInterfaceApi->OpenVoeWork(voiceChannel);

    
    /********************************************************************/
    // 初期化
    if (!pInterfaceApi->VieInit(true)) {
        return;
    }

    // 创建通道
    videoChannel = pInterfaceApi->CreateVieChannel(144, 192, voiceChannel);
    if (videoChannel < 0)
    {
        return;
    }
    
    // 设置网络参数
    pInterfaceApi->SetVieCommunicationParameters(videoChannel, 11111, 11111, IP);
    
    // 开启摄像头
    cameraId = pInterfaceApi->StartCamera(144, 192, 25, 1, videoChannel);
    if (cameraId >= 0) {
        // 摆正摄像头位置
        pInterfaceApi->VieSetRotation(cameraId, [self getCameraOrientation:pInterfaceApi->VieGetCameraOrientation(0)]);
        
        //pInterfaceApi->VieAddLocalRenderer(cameraId, self.view_local);
    }

    pInterfaceApi->VieAddRemoteRenderer(videoChannel, self.view_remote);

    pInterfaceApi->OpenVieWork(videoChannel);
}

- (IBAction)Stop:(id)sender
{
    if (pInterfaceApi)
    {
        pInterfaceApi->CloseVoeWork(voiceChannel);
        pInterfaceApi->CloseVieWork(videoChannel, cameraId);
    }
    
    delete pInterfaceApi;
    pInterfaceApi = NULL;
    
    [_pview_local removeFromSuperview];
    [self.view_remote removeFromSuperview];
}


-(NSInteger) getCameraOrientation:(NSInteger) cameraOrientation
{
    UIInterfaceOrientation displatyRotation = [[UIApplication sharedApplication] statusBarOrientation];
    NSInteger degrees = 0;
    switch (displatyRotation) {
        case UIInterfaceOrientationPortrait: degrees = 0; break;
        case UIInterfaceOrientationLandscapeLeft: degrees = 90; break;
        case UIInterfaceOrientationPortraitUpsideDown: degrees = 180; break;
        case UIInterfaceOrientationLandscapeRight: degrees = 270; break;
    }
    
    NSInteger result = 0;
    if (cameraOrientation > 180) {
        result = (cameraOrientation + degrees) % 360;
    } else {
        result = (cameraOrientation - degrees + 360) % 360;
    }
    
    return result;
}

@end
