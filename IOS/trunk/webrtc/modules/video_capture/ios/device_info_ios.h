/*
 * Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license
 * that can be found in the LICENSE file in the root of the source
 * tree. An additional intellectual property rights grant can be found
 * in the file PATENTS. All contributing project authors may
 * be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_DEVICE_INFO_IOS_H_
#define WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_DEVICE_INFO_IOS_H_

#include "../video_capture_impl.h"
#include "../device_info_impl.h"

#include "map_wrapper.h"

@class DeviceInfoIphoneObjC;

namespace webrtc {
    namespace videocapturemodule {
        class DeviceInfoIphone: public DeviceInfoImpl {
        public:
            DeviceInfoIphone(const int32_t id);
            virtual ~DeviceInfoIphone();
            int32_t Init();
            virtual uint32_t NumberOfDevices();
            virtual int32_t GetDeviceName(uint32_t deviceNumber,
                                          char* deviceNameUTF8,
                                          uint32_t deviceNameLength,
                                          char* deviceUniqueIdUTF8,
                                          uint32_t deviceUniqueIdUTF8Length,
                                          char* productUniqueIdUTF8 = 0,
                                          uint32_t productUniqueIdUTF8Length = 0);
            virtual int32_t NumberOfCapabilities(const char* deviceUniqueIdUTF8);
            virtual int32_t GetCapability(const char* deviceUniqueIdUTF8,
                                          const uint32_t deviceCapabilityNumber,
                                          VideoCaptureCapability& capability);
            virtual int32_t GetBestMatchedCapability(const char* deviceUniqueIdUTF8,
                                                     const VideoCaptureCapability& requested,
                                                     VideoCaptureCapability& resulting);
            virtual int32_t DisplayCaptureSettingsDialogBox(const char* deviceUniqueIdUTF8,
                                                            const char* dialogTitleUTF8,
                                                            void* parentWindow,
                                                            uint32_t positionX,
                                                            uint32_t positionY);
            virtual int32_t GetOrientation(const char* deviceUniqueIdUTF8,
                                           VideoCaptureRotation& orientation);
            
        protected:
            virtual int32_t CreateCapabilityMap(const char* deviceUniqueIdUTF8);
            DeviceInfoIphoneObjC* _captureInfo;
        };
    } // namespace videocapturemodule
} // namespace webrtc

#endif // WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_DEVICE_INFO_IOS_H_
