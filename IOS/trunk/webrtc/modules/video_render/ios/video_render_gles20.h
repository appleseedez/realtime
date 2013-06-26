/*
 * Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license
 * that can be found in the LICENSE file in the root of the source
 * tree. An additional intellectual property rights grant can be found
 * in the file PATENTS. All contributing project authors may
 * be found in the AUTHORS file in the root of the source tree.
 */

// Todo: Change this when file name changes
#ifndef __VIDEO_RENDER_GLES_2_0_H
#define __VIDEO_RENDER_GLES_2_0_H

#define COLOR_DEPTH_32 1

#pragma mark * Standard imports
#import <UIKit/UIKit.h>
#import <AudioUnit/AUComponent.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/EAGLDrawable.h>

#pragma mark * GIPS imports
#import "render_view.h"

#pragma mark * Standard includes
#include <list>
#include <map>

#pragma mark * GIPS includes
#include "engine_configurations.h"
#include "critical_section_wrapper.h"
#include "event_wrapper.h"
#include "trace.h"
#include "thread_wrapper.h"
#include "video_render_defines.h"
#include "video_channel_gles20.h"

namespace webrtc {
    
#pragma mark * Forward declares
    class Trace;
    class CriticalSectionWrapper;
    class ThreadWrapper;
    class Event;
    
#pragma mark * VideoRenderGLES_2_0 class declaration
    class VideoRenderGLES_2_0
    {
    public:
        VideoRenderGLES_2_0(RenderView* gipsView,
                            bool fullscreen, int32_t iId);
        virtual ~VideoRenderGLES_2_0();
        
        virtual int Init();
        virtual VideoChannelGLES_2_0* CreateEAGLChannel(int channel,
                                                        int zOrder,
                                                        float startWidth,
                                                        float startHeight,
                                                        float stopWidth,
                                                        float stopHeight);
        virtual VideoChannelGLES_2_0* ConfigureEAGLChannel(int channel,
                                                           int zOrder,
                                                           float startWidth,
                                                           float startHeight,
                                                           float stopWidth,
                                                           float stopHeight);
        virtual int DeleteEAGLChannel(int channel);
        virtual int DeleteAllEAGLChannels();
        virtual int StopThread();
        virtual bool IsFullScreen();
        virtual bool HasChannels();
        virtual bool HasChannel(int channel);
        virtual int GetChannels(std::list<int>& channelList);
        virtual bool ScreenUpdateProcess();
        virtual int GetWindowRect(Rect& rect);
        virtual int PauseRendering();
        virtual int ResumeRendering();
        
        int32_t GetScreenResolution(uint32_t& screenWidth,
                                    uint32_t& screenHeight);
        int32_t SetStreamCropping(const uint32_t streamId,
                                  const float left,
                                  const float top,
                                  const float right,
                                  const float bottom);
        
        int ChangeWindow(void* newWindowRef);
        int32_t ChangeUniqueID(int32_t id);
        int32_t StartRender();
        int32_t StopRender();
        int32_t DeleteAGLChannel(const uint32_t streamID);
        int32_t GetChannelProperties(const uint16_t streamId,
                                     uint32_t& zOrder,
                                     float& left,
                                     float& top,
                                     float& right,
                                     float& bottom);
        virtual int LockCritSec();
        virtual int UnlockCritSec();
        
    protected:
        static bool ScreenUpdateThreadProc(void* obj);
        
    private:
        int RenderOffScreenBuffers();
        int SwapAndDisplayBuffers();
        
        CriticalSectionWrapper& _glesCritSec;
        ThreadWrapper* _screenUpdateThread;
        EventWrapper* _screenUpdateEvent;
        
        RenderView* _view;
        Rect _windowRect;
        int _windowWidth;
        int _windowHeight;
        bool _fullScreen;
        GLint _backingWidth;
        GLint _backingHeight;
        GLuint _viewRenderbuffer, _viewFramebuffer, _depthRenderbuffer;
        std::map<int, VideoChannelGLES_2_0*> _aglChannels;
        std::multimap<int, int> _zOrderToChannel;
        EAGLContext* _glesContext;
        bool _isRendering;
        int32_t _id;
    };
} // namespace webrtc

#endif // __VIDEO_RENDER_GLES_2_0_H
