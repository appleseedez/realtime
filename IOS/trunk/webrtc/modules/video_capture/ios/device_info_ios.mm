/*
 * Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license
 * that can be found in the LICENSE file in the root of the source
 * tree. An additional intellectual property rights grant can be found
 * in the file PATENTS. All contributing project authors may
 * be found in the AUTHORS file in the root of the source tree.
 */
#include "device_info_ios.h"
#include "trace.h"
#include "../video_capture_config.h"
#include "device_info_ios_objc.h"

#include "video_capture.h"

namespace webrtc
{
    namespace videocapturemodule
    {
        VideoCaptureModule::DeviceInfo*
        VideoCaptureImpl::CreateDeviceInfo(const int32_t id)
        {
            WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, id, "CreateDeviceInfo %d", id);
            
            DeviceInfoIphone* newCaptureInfoModule =
            new videocapturemodule::DeviceInfoIphone(id);
            
            if (!newCaptureInfoModule || newCaptureInfoModule->Init() != 0)
            {
                //DestroyDeviceInfo(newCaptureInfoModule);
                videocapturemodule::DeviceInfoIphone* captureDeviceInfo =
                static_cast<videocapturemodule::DeviceInfoIphone*>
                (newCaptureInfoModule);
                
                delete captureDeviceInfo;
                newCaptureInfoModule = NULL;
                WEBRTC_TRACE(webrtc::kTraceInfo, kTraceVideoCapture, id,
                             "Failed to Init newCaptureInfoModule created with id %d"\
                             "and device \"\" ", id);
                return NULL;
            }
            WEBRTC_TRACE(webrtc::kTraceInfo, kTraceVideoCapture, id,
                         + "VideoCaptureModule created for id", id);
            return newCaptureInfoModule;
        }
        
        /*
         void VideoCaptureImpl::DestroyDeviceInfo(VideoCaptureDeviceInfo* deviceInfo)
         {
         WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, 0,
         "%s:%d", __FUNCTION__, __LINE__);
         videocapturemodule::DeviceInfoIphone* captureDeviceInfo =
         static_cast<videocapturemodule::DeviceInfoIphone*> (deviceInfo);
         delete captureDeviceInfo;
         captureDeviceInfo = NULL;
         }
         */
        
        DeviceInfoIphone::DeviceInfoIphone(const int32_t id)
        : DeviceInfoImpl(id)
        {
            WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, 0,
                         + "%s:%d", __FUNCTION__, __LINE__);
            _captureInfo = [[DeviceInfoIphoneObjC alloc] init];
        }
        
        DeviceInfoIphone::~DeviceInfoIphone()
        {
            WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, 0,
                         + "%s:%d", __FUNCTION__, __LINE__);
        }
        
        int32_t DeviceInfoIphone::Init()
        {
            WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, 0,
                         + "%s:%d", __FUNCTION__, __LINE__);
            return 0;
        }
        
        uint32_t DeviceInfoIphone::NumberOfDevices()
        {
            WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, 0, "%s:%d",
                         + __FUNCTION__, __LINE__);
            uint32_t captureDeviceCount =
            [[_captureInfo getCaptureDeviceCount]intValue];
            return captureDeviceCount;
            
        }
        
        int32_t DeviceInfoIphone::GetDeviceName(
                                                uint32_t deviceNumber,
                                                char* deviceNameUTF8,
                                                uint32_t deviceNameLength,
                                                char* deviceUniqueIdUTF8,
                                                uint32_t deviceUniqueIdUTF8Length,
                                                char* productUniqueIdUTF8,
                                                uint32_t productUniqueIdUTF8Length)
        {
            WEBRTC_TRACE(kTraceModuleCall, kTraceVideoCapture, 0, "%s:%d", __FUNCTION__, __LINE__);
            int errNum = [[_captureInfo getDeviceNamesFromIndex:deviceNumber
                                                    DefaultName:deviceNameUTF8 WithLength:deviceNameLength
                                                    AndUniqueID:deviceUniqueIdUTF8
                                                     WithLength:deviceUniqueIdUTF8Length]intValue];
            return errNum;
        }
        
        int32_t DeviceInfoIphone::NumberOfCapabilities(const char* deviceUniqueIdUTF8)
        {
            WEBRTC_TRACE(webrtc::kTraceError, kTraceVideoCapture, _id,
                         "NumberOfCapabilities is not supported on IPhone platform.");
            return -1;
        }
        
        
        int32_t DeviceInfoIphone::GetCapability(const char* deviceUniqueIdUTF8,
                                                const uint32_t deviceCapabilityNumber,
                                                VideoCaptureCapability& capability)
        {
            WEBRTC_TRACE(webrtc::kTraceError, kTraceVideoCapture, _id,
                         "NumberOfCapabilities is not supported"\
                         " on the IPhone platform.");
            return -1;
        }
        
        
        int32_t DeviceInfoIphone::GetBestMatchedCapability(const char* deviceUniqueIdUTF8,
                                                           const VideoCaptureCapability& requested,
                                                           VideoCaptureCapability& resulting)
        {
            WEBRTC_TRACE(webrtc::kTraceInfo, kTraceVideoCapture, _id,
                         "NumberOfCapabilities is not supported"\
                         " on the Iphone platform.");
            return -1;
        }
        
        int32_t DeviceInfoIphone::DisplayCaptureSettingsDialogBox(const char* deviceUniqueIdUTF8,
                                                                  const char* dialogTitleUTF8,
                                                                  void* parentWindow,
                                                                  uint32_t positionX,
                                                                  uint32_t positionY)
        {
            return -1;
        }
        
        
        int32_t DeviceInfoIphone::CreateCapabilityMap (const char* deviceUniqueIdUTF8)
        {
            WEBRTC_TRACE(webrtc::kTraceInfo, kTraceVideoCapture, _id,
                         "NumberOfCapabilities is not supported"\
                         " on the IPhone platform.");
            return -1;
        }
        
        int32_t DeviceInfoIphone::GetOrientation(const char* deviceUniqueIdUTF8,
                                                 VideoCaptureRotation& orientation)
        {
            if(strcmp((char*)deviceUniqueIdUTF8,"Front Camera")==0)
            {
                orientation=kCameraRotate0;
            }
            else
            {
                orientation=kCameraRotate90;
            }
            
            return orientation;
        }
        
    } // namespace videocapturemodule
} // namespace webrtc
