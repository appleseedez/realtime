/*
 * Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license
 * that can be found in the LICENSE file in the root of the source
 * tree. An additional intellectual property rights grant can be found
 * in the file PATENTS. All contributing project authors may
 * be found in the AUTHORS file in the root of the source tree.
 */

#include "engine_configurations.h"

#import "render_view.h"

#include "video_render_ios_impl.h"
#include "critical_section_wrapper.h"
#include "video_render_gles20.h"
#include "trace.h"
#include "video_render_defines.h"

namespace webrtc {
    
    VideoRenderIPhoneImpl::VideoRenderIPhoneImpl(
                                                 const int32_t id,
                                                 const VideoRenderType videoRenderType,
                                                 void* window,
                                                 const bool fullscreen) :
    _id(id),
    _renderIPhoneCritsect(*CriticalSectionWrapper::CreateCriticalSection()),
    _fullScreen(fullscreen),
    _ptrWindow(window)
    {
        WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "Constructor %s:%d",
                     __FUNCTION__, __LINE__);
    }
    
    VideoRenderIPhoneImpl::~VideoRenderIPhoneImpl()
    {
        WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id,
                     "Destructor %s:%d", __FUNCTION__, __LINE__);
        delete &_renderIPhoneCritsect;
        if (_ptrIPhoneRender)
        {
            delete _ptrIPhoneRender;
            _ptrIPhoneRender = NULL;
        }
    }
    
    int32_t
    VideoRenderIPhoneImpl::Init()
    {
        CriticalSectionScoped cs(&_renderIPhoneCritsect);
        WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id,
                     "%s:%d", __FUNCTION__, __LINE__);
        
        _ptrIPhoneRender =
        new VideoRenderGLES_2_0((RenderView*)_ptrWindow, _fullScreen, _id);
        if (!_ptrWindow)
        {
            WEBRTC_TRACE(kTraceWarning, kTraceVideoRenderer, _id,
                         "Constructor %s:%d", __FUNCTION__, __LINE__);
            return -1;
        }
        int retVal = _ptrIPhoneRender->Init();
        if (retVal == -1)
        {
            WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id,
                         "Failed to init %s:%d", __FUNCTION__, __LINE__);
            return -1;
        }
        return 0;
    }
    
    int32_t
    VideoRenderIPhoneImpl::ChangeUniqueId(const int32_t id)
    {
        CriticalSectionScoped cs(&_renderIPhoneCritsect);
        WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "%s", __FUNCTION__);
        _id = id;
        
        if(_ptrIPhoneRender)
        {
            _ptrIPhoneRender->ChangeUniqueID(_id);
        }
        
        return 0;
    }
    
    int32_t
    VideoRenderIPhoneImpl::ChangeWindow(void* window)
    {
        CriticalSectionScoped cs(&_renderIPhoneCritsect);
        WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id,
                     "%s changing ID to ", __FUNCTION__, window);
        
        if (window == NULL)
        {
            return -1;
        }
        _ptrWindow = window;
        
        WEBRTC_TRACE(kTraceModuleCall, kTraceVideoRenderer, _id,
                     "%s:%d", __FUNCTION__, __LINE__);
        
        _ptrWindow = window;
        _ptrIPhoneRender->ChangeWindow((RenderView*)_ptrWindow);
        
        return 0;
    }
    
    VideoRenderCallback*
    VideoRenderIPhoneImpl::AddIncomingRenderStream(const uint32_t streamId,
                                                   const uint32_t zOrder,
                                                   const float left,
                                                   const float top,
                                                   const float right,
                                                   const float bottom)
    {
        CriticalSectionScoped cs(&_renderIPhoneCritsect);
        WEBRTC_TRACE(kTraceDebug, kTraceVideoRenderer, _id, "%s", __FUNCTION__);
        if(!_ptrWindow)
        {
            return NULL;
        }
        
        VideoChannelGLES_2_0* nsOpenGLChannel =
        _ptrIPhoneRender->CreateEAGLChannel(streamId, zOrder, left,
                                            top, right, bottom);
        
        if(!nsOpenGLChannel)
        {
            WEBRTC_TRACE(kTraceError, kTraceVideoRenderer, _id,
                         "%s Failed to create NSGL channel", __FUNCTION__);
            return NULL;
        }
        
        return nsOpenGLChannel;
    }
    
    int32_t
    VideoRenderIPhoneImpl::DeleteIncomingRenderStream(const uint32_t streamId)
    {
        WEBRTC_TRACE(kTraceDebug, kTraceVideoRenderer, _id,
                     "Constructor %s:%d", __FUNCTION__, __LINE__);
        CriticalSectionScoped cs(&_renderIPhoneCritsect);
        _ptrIPhoneRender->DeleteEAGLChannel(streamId);
        
        return 0;
    }
    
    int32_t
    VideoRenderIPhoneImpl::GetIncomingRenderStreamProperties(
                                                             const uint32_t streamId,
                                                             uint32_t& zOrder,
                                                             float& left,
                                                             float& top,
                                                             float& right,
                                                             float& bottom) const
    {
        return _ptrIPhoneRender->GetChannelProperties(streamId, zOrder, left,
                                                      top, right, bottom);
    }
    
    int32_t
    VideoRenderIPhoneImpl::StartRender()
    {
        return _ptrIPhoneRender->StartRender();
    }
    
    int32_t
    VideoRenderIPhoneImpl::StopRender()
    {
        return _ptrIPhoneRender->StopRender();
    }
    
    VideoRenderType
    VideoRenderIPhoneImpl::RenderType()
    {
        return kRenderiPhone;
    }
    
    RawVideoType
    VideoRenderIPhoneImpl::PerferedVideoType()
    {
        return kVideoI420;
    }
    
    bool
    VideoRenderIPhoneImpl::FullScreen()
    {
        WEBRTC_TRACE(kTraceError, kTraceVideoRenderer, _id,
                     "%s - not supported on iPhone", __FUNCTION__);
        return -1;
    }
    
    
    int32_t
    VideoRenderIPhoneImpl::GetGraphicsMemory(
                                             uint64_t& totalGraphicsMemory,
                                             uint64_t& availableGraphicsMemory) const
    {
        WEBRTC_TRACE(kTraceError, kTraceVideoRenderer, _id,
                     "%s - not supported on iPhone", __FUNCTION__);
        return -1;
    }
    
    int32_t
    VideoRenderIPhoneImpl::GetScreenResolution(uint32_t& screenWidth,
                                               uint32_t& screenHeight) const
    {
        return _ptrIPhoneRender->GetScreenResolution(screenWidth, screenHeight);
    }
    
    uint32_t
    VideoRenderIPhoneImpl::RenderFrameRate(const uint32_t streamId)
    {
        WEBRTC_TRACE(kTraceError, kTraceVideoRenderer, _id,
                     "%s - not supported on iPhone", __FUNCTION__);
        return -1;
    }
    
    int32_t
    VideoRenderIPhoneImpl::SetStreamCropping(
                                             const uint32_t streamId,
                                             const float left,
                                             const float top,
                                             const float right,
                                             const float bottom)
    {
        return _ptrIPhoneRender->SetStreamCropping(streamId, left, top,
                                                   right, bottom);
    }
    
    
    int32_t VideoRenderIPhoneImpl::ConfigureRenderer(
                                                     const uint32_t streamId,
                                                     const unsigned int zOrder,
                                                     const float left,
                                                     const float top,
                                                     const float right,
                                                     const float bottom)
    {
        // TODO
        return 0;
    }
    
    
    int32_t
    VideoRenderIPhoneImpl::SetTransparentBackground(const bool enable)
    {
        // TODO
        return 0;
    }
    
    int32_t VideoRenderIPhoneImpl::SetText(
                                           const uint8_t textId,
                                           const uint8_t* text,
                                           const int32_t textLength,
                                           const uint32_t textColorRef,
                                           const uint32_t backgroundColorRef,
                                           const float left,
                                           const float top,
                                           const float right,
                                           const float bottom)
    {
        // return _ptrIPhoneRender->SetText(textId, text, textLength,
        // textColorRef,backgroundColorRef,
        // left, top, right, bottom);
        return -1;
    }
    
    int32_t VideoRenderIPhoneImpl::SetBitmap(
                                             const void* bitMap,
                                             const uint8_t pictureId,
                                             const void* colorKey,
                                             const float left,
                                             const float top,
                                             const float right,
                                             const float bottom)
    {
        // TODO
        return -1;
    }
    
    int32_t VideoRenderIPhoneImpl::FullScreenRender(void* window,
                                                    const bool enable)
    {
        // TODO
        return -1;
    }
} // namspace webrtc
