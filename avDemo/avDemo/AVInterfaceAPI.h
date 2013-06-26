//
//  AVInterfaceAPI.h
//  avDemo
//
//  Created by chenjianjun on 13-6-7.
//  Copyright (c) 2013年 free. All rights reserved.
//

#ifndef __avDemo__AVInterfaceAPI__
#define __avDemo__AVInterfaceAPI__

#include <iostream>

/*-------webrtc头文件包含-----start*/
#include "webrtc/video_engine/include/vie_base.h"
#include "webrtc/video_engine/include/vie_capture.h"
#include "webrtc/video_engine/include/vie_codec.h"
#include "webrtc/video_engine/include/vie_external_codec.h"
#include "webrtc/video_engine/include/vie_network.h"
#include "webrtc/video_engine/include/vie_render.h"
#include "webrtc/video_engine/include/vie_rtp_rtcp.h"

#include "webrtc/voice_engine/include/voe_base.h"
#include "webrtc/voice_engine/include/voe_audio_processing.h"
#include "webrtc/voice_engine/include/voe_codec.h"
#include "webrtc/voice_engine/include/voe_file.h"
#include "webrtc/voice_engine/include/voe_hardware.h"
#include "webrtc/voice_engine/include/voe_network.h"
#include "webrtc/voice_engine/include/voe_rtp_rtcp.h"
#include "webrtc/voice_engine/include/voe_volume_control.h"

#include "webrtc/common_types.h"
#include "webrtc/system_wrappers/interface/scoped_ptr.h"
/*-------webrtc头文件包含-----end  */

#include "UdpTransportAPI.h"

using namespace webrtc;

// VoiceEngine data struct
typedef struct stVoiceEngineData
{
    VoiceEngine* ve;
    VoEBase* base;
    VoECodec* codec;
    VoEFile* file;
    VoENetwork* netw;
    VoEAudioProcessing* apm;
    VoEVolumeControl* volume;
    VoEHardware* hardware;
    VoERTP_RTCP* rtp;
    
    scoped_ptr<VoiceTransport> transport;
    
    stVoiceEngineData()
    {
        ve = NULL;
        base = NULL;
        codec = NULL;
        file = NULL;
        netw = NULL;
        apm = NULL;
        volume = NULL;
        hardware = NULL;
        rtp = NULL;
    }
    
} VoiceEngineData;

// VideoEngine data struct
typedef struct stVideoEngineData
{
    VideoEngine* vie;
    ViEBase* base;
    ViECodec* codec;
    ViENetwork* netw;
    ViERTP_RTCP* rtp;
    ViERender* render;
    ViECapture* capture;
    ViEExternalCodec* externalCodec;
    
    scoped_ptr<VideoTransport> transport;
    
    stVideoEngineData()
    {
        vie = NULL;
        base = NULL;
        codec = NULL;
        netw = NULL;
        rtp = NULL;
        render = NULL;
        capture = NULL;
        externalCodec = NULL;
    }
    
} VideoEngineData;

// 操作系统类型
enum OsType
{
    OsT_WIN = 0,
    OsT_ANDRIOD = 1,
    OsT_IOS = 2
};

// 操作类型
enum OperatorType
{
    OpT_Speaker = 0,// 外放控制
    OpT_EC = 1,// 回音消除
    OpT_AGC = 2,// 自动增益
    Opt_NS = 3// 降噪
};

// 声音控制参数设置
typedef struct stVoEControlParameters
{
    OsType ostype;// 操作系统类型
    OperatorType optype;// 操作类型
    bool enable;// ture是开启 false是关闭
    int iVolume;// 操作类型是外放控制时，这个表示音量
}VoEControlParameters;

// 视频控制参数设置
typedef struct stViEControlParameters
{
    int width;// 摄像头采集的视频宽
    int height;// 摄像头采集的视频高
    int frameRate;// 摄像头采集的视频桢率
}ViEControlParameters;

class MyTracCallback: public TraceCallback {
public:
    virtual void Print(TraceLevel level, const char* message, int length)
    {
        NSLog(@"\n%s\n",message);
    };
    
public:
    virtual ~MyTracCallback() {}
    MyTracCallback() {}
};

class CAVInterfaceAPI
{
public:
    CAVInterfaceAPI();
    ~CAVInterfaceAPI();
    
public:
    /*********************************音频api接口********************************/
    //////////////////////////////////////////////////////////////////////////
    ///@brief 音频引擎初期化
    ///
    ///@param[]
    ///
    ///@return true false
    //////////////////////////////////////////////////////////////////////////
    bool VoeInit();
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 创建一个音频channel
    ///
    ///@param[]
    ///
    ///@return 音频channel 小于0:失败
    //////////////////////////////////////////////////////////////////////////
    int CreateVoeChannel();
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 设置网络通信参数
    ///
    ///@param[int] 音频channel
    ///@param[int] 本地接收数据监听端口
    ///@param[int] 远端接收数据端口
    ///@param[string] 远端接收ip
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    int SetVoeCommunicationParameters(int voechannel,
                                      int localListenPort,
                                      int remoteRecvPort,
                                      std::string strRemoteIP);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 打开音频工作引擎
    ///
    ///@param[int] 音频channel
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    int OpenVoeWork(int voechannel);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 音频控制参数接口
    ///
    ///@param[int] 音频channel
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    int SetVoEControlParameters(VoEControlParameters& param);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 关闭音频工作引擎
    ///
    ///@param[int] 音频channel
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    void CloseVoeWork(int voechannel);
    
    
    
    
    
    /*********************************视频api接口********************************/
    //////////////////////////////////////////////////////////////////////////
    ///@brief 视频引擎的初期化
    ///
    ///@param[bool] 音视频同步标志，默认是做音视频同步，如果做话音频的初期化工作因该放前面
    ///
    ///@return true false
    //////////////////////////////////////////////////////////////////////////
    bool VieInit(bool voeSyncFlg = true);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 设置网络通信参数
    ///
    ///@param[int] 视频channel
    ///@param[int] 本地接收数据监听端口
    ///@param[int] 远端接收数据端口
    ///@param[string] 远端接收ip
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    int SetVieCommunicationParameters(int viechannel,
                                      int localListenPort,
                                      int remoteRecvPort,
                                      std::string strRemoteIP);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 创建视频和摄像头通道
    ///
    ///@param[in] 摄像头采集的视频宽
    ///@param[in] 摄像头采集的视频高
    ///@param[in] 音频通道，主要用于音视频绑定
    ///
    ///@return 视频channel 小于0是失败
    //////////////////////////////////////////////////////////////////////////
    int CreateVieChannel(int width,int height,int voiceChannel);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 开启摄像头通道
    ///
    ///@param[in] 摄像头采集的视频宽
    ///@param[in] 摄像头采集的视频高
    ///@param[in] 摄像头采集的视频桢率
    ///@param[in] 摄像头序号
    ///@param[out] 视频channel
    ///
    ///@return 摄像头通道 小于0是失败
    //////////////////////////////////////////////////////////////////////////
    int StartCamera(int width,
                    int height,
                    int frameRate,
                    int cameraNum,
                    int viechannel);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 开启视频引擎
    ///
    ///@param[in] 视频通道
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    int OpenVieWork(int viechannel);
    
    //////////////////////////////////////////////////////////////////////////
    ///@brief 停止音频引擎
    ///
    ///@param[in] 视频通道
    ///@param[in] 摄像头通道
    ///
    ///@return 0是成功 其他是失败
    //////////////////////////////////////////////////////////////////////////
    void CloseVieWork(int viechannel, int cameraId);
    
    // 设置远程图像显示窗口
    int VieAddRemoteRenderer(int viechannel, void* view);
    // 设置本地图像显示窗口
    int VieAddLocalRenderer(int captureId, void* view);
    
    // 获取摄像头（0:前置摄像头 1:后置摄像头）
    int VieGetCameraOrientation(int cameraNum);
    // 设置摄像头方向
    int VieSetRotation(int captureId, int degrees);
    
private:
    // 初期化
    // type:0(音频) 1（视频）
    bool setUpAVEnv(int type);
    // 跟踪日志类
    MyTracCallback mylog;
    VoiceEngineData voeData;
    VideoEngineData vieData;
};

#endif /* defined(__avDemo__AVInterfaceAPI__) */
