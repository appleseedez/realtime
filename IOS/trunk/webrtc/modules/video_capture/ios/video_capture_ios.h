/*
 * Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license
 * that can be found in the LICENSE file in the root of the source
 * tree. An additional intellectual property rights grant can be found
 * in the file PATENTS. All contributing project authors may
 * be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_VIDEO_CAPTURE_IOS_H_
#define WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_VIDEO_CAPTURE_IOS_H_

#import <AVFoundation/AVFoundation.h>
#include <stdio.h>

#include "../video_capture_impl.h"
#include "../device_info_impl.h"
#include "video_capture_defines.h"

// Forward declaraion
@class VideoCaptureiPhoneObjC;
@class DeviceInfoIphoneObjC;

namespace webrtc {
    namespace videocapturemodule {
        class VideoCaptureiPhone: public VideoCaptureImpl {
        public:
            VideoCaptureiPhone(const int32_t id);
            virtual ~VideoCaptureiPhone();
            int32_t Init(const int32_t id, const char* deviceUniqueIdUTF8);
            virtual int32_t StartCapture(const VideoCaptureCapability& capability);
            virtual int32_t StopCapture();
            virtual bool CaptureStarted();
            virtual int32_t CaptureSettings(VideoCaptureCapability& settings);
            
        protected:
            // Help functions
            int32_t SetCameraOutput();
            
        private:
            VideoCaptureiPhoneObjC* _captureDevice;
            DeviceInfoIphoneObjC* _captureInfo;
            bool _isCapturing;
            int32_t _id;
            int32_t _captureWidth;
            int32_t _captureHeight;
            int32_t _captureFrameRate;
            int32_t _frameCount;
        };
    } // namespace videocapturemodule
} // namespace webrtc

#endif // WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_VIDEO_CAPTURE_IOS_H_
