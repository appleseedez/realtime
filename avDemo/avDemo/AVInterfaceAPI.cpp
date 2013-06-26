//
//  AVInterfaceAPI.cpp
//  avDemo
//
//  Created by chenjianjun on 13-6-7.
//  Copyright (c) 2013年 free. All rights reserved.
//

#include "AVInterfaceAPI.h"

CAVInterfaceAPI::CAVInterfaceAPI()
{
}

CAVInterfaceAPI::~CAVInterfaceAPI()
{
}

bool CAVInterfaceAPI::VoeInit()
{
    if (!setUpAVEnv(0))
    {
        return false;
    }
    
    if (voeData.base->Init() != 0)
    {
        NSLog(@"voeData.base [Init] error.\n");
        
        return false;
    }
    
    return true;
}

bool CAVInterfaceAPI::setUpAVEnv(int type)
{
    switch (type)
    {
        case 0:
        {
            /**************************音频引擎创建************************/
            if (voeData.ve)
            {
                //NSLog(@"VoE already created");
                
                return false;
            }
            
            voeData.ve = VoiceEngine::Create();
            if (!voeData.ve)
            {
                //NSLog(@"Create VoE failed");
                
                return false;
            }
            
            VoiceEngine::SetTraceCallback(&mylog);
            VoiceEngine::SetTraceFilter(kTraceVideo | kTraceVideoCapture | kTraceVideoRenderer | kTraceVideoCoding);
            
            // Base
            voeData.base = VoEBase::GetInterface(voeData.ve);
            if (!voeData.base)
            {
                //NSLog(@"Get base sub-API failed");
                
                return false;
            }
            
            // Codec
            voeData.codec = VoECodec::GetInterface(voeData.ve);
            if (!voeData.codec)
            {
                //NSLog(@"Get codec sub-API failed");
                
                return false;
            }
            
            // File
            voeData.file = VoEFile::GetInterface(voeData.ve);
            if (!voeData.file)
            {
                //NSLog(@"Get file sub-API failed");
                
                return false;
            }
            
            // Network
            voeData.netw = VoENetwork::GetInterface(voeData.ve);
            if (!voeData.netw)
            {
                //NSLog(@"Get network sub-API failed");
                
                return false;
            }
            
            // audioprocessing
            voeData.apm = VoEAudioProcessing::GetInterface(voeData.ve);
            if (!voeData.apm)
            {
                //NSLog(@"Get VoEAudioProcessing sub-API failed");
                
                return false;
            }
            
            // Volume
            voeData.volume = VoEVolumeControl::GetInterface(voeData.ve);
            if (!voeData.volume)
            {
                //NSLog(@"Get volume sub-API failed");
                
                return false;
            }
            
            // Hardware
            voeData.hardware = VoEHardware::GetInterface(voeData.ve);
            if (!voeData.hardware)
            {
                //NSLog(@"Get hardware sub-API failed");
                
                return false;
            }
            
            // RTP
            voeData.rtp = VoERTP_RTCP::GetInterface(voeData.ve);
            if (!voeData.rtp)
            {
                //NSLog(@"Get rtp sub-API failed");
                
                return false;
            }
            
            break;
        }
        case 1:
        {
            /**************************视频引擎创建************************/
            if (vieData.vie)
            {
                //NSLog(@"VoE already created.\n");
                
                return false;
            }
            
            vieData.vie = VideoEngine::Create();
            if (!voeData.ve)
            {
                //NSLog(@"VoE create failed.\n");
                
                return false;
            }
            
            VideoEngine::SetTraceCallback(&mylog);
            VideoEngine::SetTraceFilter(kTraceVideo | kTraceVideoCapture | kTraceVideoRenderer | kTraceVideoCoding);
            
            vieData.base = ViEBase::GetInterface(vieData.vie);
            if (!vieData.base)
            {
                //NSLog(@"Get base sub-API failed");
                
                return false;
            }
            
            vieData.codec = ViECodec::GetInterface(vieData.vie);
            if (!vieData.codec)
            {
                //NSLog(@"Get codec sub-API failed");
                
                return false;
            }
            
            vieData.netw = ViENetwork::GetInterface(vieData.vie);
            if (!vieData.netw)
            {
                //NSLog(@"Get network sub-API failed");
                
                return false;
            }
            
            vieData.rtp = ViERTP_RTCP::GetInterface(vieData.vie);
            if (!vieData.rtp)
            {
                //NSLog(@"Get RTP sub-API failed");
                
                return false;
            }
            
            vieData.render = ViERender::GetInterface(vieData.vie);
            if (!vieData.render)
            {
                //NSLog(@"Get Render sub-API failed");
                
                return false;
            }
            
            vieData.capture = ViECapture::GetInterface(vieData.vie);
            if (!vieData.capture)
            {
                //NSLog(@"Get Capture sub-API failed");
                
                return false;
            }
            
            vieData.externalCodec = ViEExternalCodec::GetInterface(vieData.vie);
            if (!vieData.capture)
            {
                //NSLog(@"Get External Codec sub-API failed");
                
                return false;
            }
            
            break;
        }
            
        default:
            return false;
    }
    
    return true;
}

int CAVInterfaceAPI::CreateVoeChannel()
{
    int voechannel = voeData.base->CreateChannel();
    
    if (voechannel < 0)
    {
        return -1;
    }
    
    // 设置编解码
    CodecInst voiceCodec;
    strcpy(voiceCodec.plname, "ILBC");
    voiceCodec.plfreq = 8000;// 宽带模式
    voiceCodec.pltype = 102;// 默认动态负载类型
    voiceCodec.pacsize = 240;//
    voiceCodec.channels = 1;// 单声道
    voiceCodec.rate = 13300;// 信道自适应
    int numCodecs = voeData.codec->NumOfCodecs();
    
    for (int i = 0; i < numCodecs; i++)
    {
        if (voeData.codec->GetCodec(i, voiceCodec) != -1)
        {
            if (strncmp(voiceCodec.plname, "ILBC", 4) == 0)
            {
                break;
            }
        }
    }
    
    if (voeData.codec->SetSendCodec(voechannel, voiceCodec) != 0)
    {
        return -1;
    }
    
    return voechannel;
}

int CAVInterfaceAPI::SetVoeCommunicationParameters(int voechannel,
                                                   int localListenPort,
                                                   int remoteRecvPort,
                                                   std::string strRemoteIP)
{
    voeData.transport.reset(new VoiceTransport(voeData.netw, voechannel));
    
    voeData.transport->SetLocalReceiver(localListenPort);
    voeData.transport->SetSendDestination(strRemoteIP.c_str(), remoteRecvPort);
    
    return 0;
}

int CAVInterfaceAPI::OpenVoeWork(int voechannel)
{
    if (voeData.base->StartSend(voechannel) != 0)
    {
        return -1;
    }
    
    if (voeData.base->StartReceive(voechannel) != 0)
    {
        return -1;
    }
    
    if (voeData.base->StartPlayout(voechannel) != 0)
    {
        return -1;
    }
    
    return 0;
}

void CAVInterfaceAPI::CloseVoeWork(int channel)
{
    voeData.base->StopSend(channel);
    voeData.base->StartReceive(channel);
    voeData.base->StopPlayout(channel);
    voeData.transport.reset(NULL);
    voeData.base->DeleteChannel(channel);
    voeData.base->Terminate();
    VoiceEngine::SetTraceCallback(NULL);
    // Delete Voe
    VoiceEngine::Delete(voeData.ve);
    memset(&voeData, 0, sizeof(voeData));
}

int CAVInterfaceAPI::SetVoEControlParameters(VoEControlParameters& param)
{
    switch (param.optype) {
        case OpT_Speaker:// 外放控制
        {
            if (voeData.hardware->SetLoudspeakerStatus(param.enable) != 0)
            {
                return -1;
            }
            
            if (voeData.volume->SetSpeakerVolume(param.iVolume) != 0)
            {
                return -1;
            }
            
            break;
        }
        case OpT_EC:// 回音消除
        {
            if (param.ostype == OsT_IOS || param.ostype == OsT_ANDRIOD)
            {
                if (voeData.apm->SetEcStatus(param.enable, kEcAecm) < 0)
                {
                    return -1;
                }
                
                if (voeData.apm->SetAecmMode(kAecmSpeakerphone, false) != 0)
                {
                    return -1;
                }
            }
            else
            {
                if (voeData.apm->SetEcStatus(param.enable, kEcAec) < 0)
                {
                    return -1;
                }
            }
            
            break;
        }
        case OpT_AGC:// 自动增益
        {
            if (param.ostype == OsT_IOS || param.ostype == OsT_ANDRIOD)
            {
                if (voeData.apm->SetAgcStatus(param.enable, kAgcFixedDigital) < 0)
                {
                    return -1;
                }
                
                webrtc::AgcConfig config;
                // The following settings are by default, explicitly set here.
                config.targetLeveldBOv = 3;
                config.digitalCompressionGaindB = 9;
                config.limiterEnable = true;
                if (voeData.apm->SetAgcConfig(config) != 0)
                {
                    return -1;
                }
            }
            else
            {
                if (voeData.apm->SetAgcStatus(param.enable, kAgcAdaptiveDigital) < 0)
                {
                    return -1;
                }
            }
            
            break;
        }
        case Opt_NS:// 降噪
        {
            if (param.ostype == OsT_IOS || param.ostype == OsT_ANDRIOD)
            {
                if (voeData.apm->SetNsStatus(param.enable, kNsModerateSuppression) < 0)
                {
                    return -1;
                }
            }
            else
            {
                if (voeData.apm->SetNsStatus(param.enable, kNsVeryHighSuppression) < 0)
                {
                    return -1;
                }
            }
        }
        default:
            return -1;
    }
    
    return 0;
}


bool CAVInterfaceAPI::VieInit(bool voeSyncFlg)
{
    if (!setUpAVEnv(1))
    {
        return false;
    }
    
    if (vieData.base->Init() != 0)
    {
        return false;
    }
    
    // 判断用户是否需要做音视频同步
    if (!voeSyncFlg)
    {
        return true;
    }
    
    if (!voeData.ve)
    {
        return false;
    }
    
    // 把音频引擎加入到视频引擎中去
    if (0 != vieData.base->SetVoiceEngine(voeData.ve))
    {
        return false;
    }
    
    return true;
}

int CAVInterfaceAPI::CreateVieChannel(int width,int height,int voiceChannel)
{
    if (!vieData.base || !vieData.codec)
    {
        return -1;
    }
    
    int viechannel;
    
    if (vieData.base->CreateChannel(viechannel) != 0)
    {
        return -1;
    }
    
    if (voiceChannel >= 0)
    {
        vieData.base->ConnectAudioChannel(viechannel, voiceChannel);
    }
    
    // 设置编解码
    VideoCodec codec;
    int num_codec = vieData.codec->NumberOfCodecs();
    for (int codecNum = 0; codecNum < num_codec; ++codecNum)
    {
        if (vieData.codec->GetCodec(codecNum, codec) != -1)
        {
            if (codec.codecType == kVideoCodecVP8)
            {
                break;
            }
        }
    }
    
    codec.startBitrate = 500;
    codec.maxBitrate = 600;
    codec.width = width;
    codec.height = height;
    codec.maxFramerate = 15;
    
    if (vieData.codec->SetReceiveCodec(viechannel, codec) != 0)
    {
        return -1;
    }
    
    if (vieData.codec->SetSendCodec(viechannel, codec) != 0)
    {
        return -1;
    }
    
    return viechannel;
}

int CAVInterfaceAPI::StartCamera(int width,
                                 int height,
                                 int frameRate,
                                 int cameraNum,
                                 int viechannel)
{
    if (!vieData.capture )
    {
        return -1;
    }
    
    // 尝试打开摄像头
    char deviceName[128];
    char deviceUniqueName[128];
    int cameraId;
    if (cameraNum > vieData.capture->NumberOfCaptureDevices())
    {
        return -1;
    }
    
    if (vieData.capture->GetCaptureDevice(cameraNum,
                                          deviceName,
                                          sizeof(deviceName),
                                          deviceUniqueName,
                                          sizeof(deviceUniqueName)) < 0)
    {
        return -1;
    }
    
    vieData.capture->AllocateCaptureDevice(deviceUniqueName,
                                           sizeof(deviceUniqueName),
                                           cameraId);
    
    if (cameraId >= 0)
    {
        vieData.capture->ConnectCaptureDevice(cameraId, viechannel);
        
        CaptureCapability capability;
        capability.height = height;
        capability.width = width;
        capability.maxFPS = frameRate;
        capability.rawType = kVideoI420;
        capability.codecType = kVideoCodecUnknown;
        
        vieData.capture->StartCapture(cameraId, capability);
    }
    
    return cameraId;
}

int CAVInterfaceAPI::OpenVieWork(int viechannel)
{
    if (!vieData.base)
    {
        return -1;
    }
    
    if (vieData.base->StartSend(viechannel) != 0)
    {
        return -1;
    }
    
    if (vieData.base->StartReceive(viechannel) != 0)
    {
        return -1;
    }
    
    return 0;
}

int CAVInterfaceAPI::SetVieCommunicationParameters(int viechannel,
                                                   int localListenPort,
                                                   int remoteRecvPort,
                                                   std::string strRemoteIP)
{
    if (!vieData.rtp)
    {
        return -1;
    }
    
    // 外挂传输
    vieData.transport.reset(new VideoTransport(vieData.netw, viechannel));
    // 设置丢包重传
    vieData.rtp->SetNACKStatus(viechannel, true);
    // 无数据时发送关键桢
    vieData.rtp->SetKeyFrameRequestMethod(viechannel, kViEKeyFrameRequestPliRtcp);
    
    if (vieData.transport->SetLocalReceiver(localListenPort) != 0)
    {
        return -1;
    }
    
    if (vieData.transport->SetSendDestination(strRemoteIP.c_str(), remoteRecvPort) != 0)
    {
        return -1;
    }
    
    return 0;
}

int CAVInterfaceAPI::VieAddRemoteRenderer(int viechannel, void* view)
{
    if (!vieData.render)
    {
        return -1;
    }
    
    vieData.render->AddRenderer(viechannel, view, 1, 0.0, 0.0, 1.0, 1.0);
    if (vieData.render->StartRender(viechannel) != 0)
    {
        return -1;
    }
    
    return 0;
}

int CAVInterfaceAPI::VieAddLocalRenderer(int _captureId, void* view)
{
    if (!vieData.render)
    {
        return -1;
    }
    
    vieData.render->AddRenderer(_captureId, view, 0, 0.0, 0.0, 1.0, 1.0);
    if (vieData.render->StartRender(_captureId) != 0)
    {
        return -1;
    }
    
    return 0;
}

int CAVInterfaceAPI::VieGetCameraOrientation(int cameraNum)
{
    if (!vieData.capture)
    {
        return -1;
    }
    
    char deviceName[128];
    char deviceUniqueName[128];
    
    if (vieData.capture->GetCaptureDevice(cameraNum,
                                          deviceName,
                                          sizeof(deviceName),
                                          deviceUniqueName,
                                          sizeof(deviceUniqueName)) != 0)
    {
        return -1;
    }
    
    RotateCapturedFrame orientation;
    vieData.capture->GetOrientation(deviceUniqueName, orientation);
    
    return (int)orientation;
}

int CAVInterfaceAPI::VieSetRotation(int captureId, int degrees)
{
    if (!vieData.capture)
    {
        return -1;
    }
    
    RotateCapturedFrame rotation = RotateCapturedFrame_0;
    if (degrees == 90)
        rotation = RotateCapturedFrame_90;
    else if (degrees == 180)
        rotation = RotateCapturedFrame_180;
    else if (degrees == 270)
        rotation = RotateCapturedFrame_270;
    
    return vieData.capture->SetRotateCapturedFrames(captureId, rotation);
}

void CAVInterfaceAPI::CloseVieWork(int viechannel, int cameraId)
{
    if (!vieData.base)
    {
        vieData.base->StopReceive(viechannel);
        vieData.base->StopSend(viechannel);
        vieData.base->DeleteChannel(viechannel);
        vieData.base->Release();
    }
    if (vieData.render)
    {
        vieData.render->StopRender(viechannel);
        vieData.render->RemoveRenderer(viechannel);
        vieData.render->RemoveRenderer(cameraId);
        vieData.render->Release();
    }
    if (!vieData.capture)
    {
        vieData.capture->StopCapture(cameraId);
        vieData.capture->ReleaseCaptureDevice(cameraId);
        vieData.capture->Release();
    }
    if (!vieData.netw)
    {
        vieData.transport.reset(NULL);
        vieData.netw->Release();
    }
    if (!vieData.rtp) {
        vieData.rtp->Release();
    }
    if (!vieData.codec)
    {
        vieData.codec->Release();
    }
    if (!vieData.externalCodec)
    {
        vieData.externalCodec->Release();
    }
    
    VideoEngine::SetTraceCallback(NULL);
    // Delete Vie
    VideoEngine::Delete(vieData.vie);
    
    memset(&vieData, 0, sizeof(vieData));
}
