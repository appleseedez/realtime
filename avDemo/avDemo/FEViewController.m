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
    NSString* _ip;

}

@end

//#define IP "192.168.1.100"

@implementation FEViewController

//@synthesize view_local;
@synthesize view_remote;


- (void)viewDidLoad
{
    [super viewDidLoad];
    NSUserDefaults* userDefauts = [NSUserDefaults standardUserDefaults];
    _ip = [userDefauts objectForKey:@"ip"];
    if (!_ip) {
        _ip = @"127.0.0.1";
    }
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
 
    
}

- (IBAction)Start:(id)sender
{
    _ip = [[NSUserDefaults standardUserDefaults] objectForKey:@"ip"];
    if (!_ip) {
        _ip = @"127.0.0.1";
    }
   [[ [UIAlertView alloc] initWithTitle:@"ip" message:[NSString stringWithFormat:@"ip is : %@",_ip] delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:nil, nil] show];
    CGRect view2_frame = CGRectMake(0,
                                    0,
                                    self.visionView.bounds.size.width,
                                    self.visionView.bounds.size.height);
    self.view_remote = [[RenderView alloc] initWithFrame:view2_frame];
    [self.view_remote setBackgroundColor:[UIColor blueColor]];
   
    CGPoint previewFrameAncor = [self.visionView convertPoint: CGPointMake(self.visionView.bounds.size.width - 150, self.visionView.bounds.size.height- 200) toView:self.view];
    CGRect previewFrame = CGRectMake(previewFrameAncor.x, previewFrameAncor.y, 144, 192);
    _pview_local = [[UIImageView alloc] initWithFrame:previewFrame];
    [_pview_local setBackgroundColor:[UIColor greenColor]];
    
    [self.visionView addSubview:self.view_remote];
    [self.view sendSubviewToBack:self.visionView];
    [self.view addSubview:_pview_local];
    

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
    
    pInterfaceApi->SetVoeCommunicationParameters(voiceChannel, 11113, 11113, [_ip UTF8String]);
    
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
    pInterfaceApi->SetVieCommunicationParameters(videoChannel, 11111, 11111, [_ip UTF8String]);
    
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

- (void)dealloc {
    [_visionView release];
    [super dealloc];
}
@end
