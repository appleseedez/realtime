/*
 * Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license
 * that can be found in the LICENSE file in the root of the source
 * tree. An additional intellectual property rights grant can be found
 * in the file PATENTS. All contributing project authors may
 * be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_DEVICE_INFO_IOS_OBJC_H_
#define WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_DEVICE_INFO_IOS_OBJC_H_

#import <Foundation/Foundation.h>
#include "device_info_ios.h"

@interface DeviceInfoIphoneObjC : NSObject
{
}
/**************************************************************************
 *
 * The following functions are called by DeviceInfoIphone class
 *
 ***************************************************************************/

- (NSNumber*)getCaptureDeviceCount;

- (NSNumber*)getDeviceNamesFromIndex:(uint32_t)index
                         DefaultName:(char*)deviceName
                          WithLength:(uint32_t)deviceNameLength
                         AndUniqueID:(char*)deviceUniqueID
                          WithLength:(uint32_t)deviceUniqueIDLength;

- (NSNumber*)displayCaptureSettingsDialogBoxWithDevice:(const uint8_t*)deviceUniqueIdUTF8
                                              AndTitle:(const uint8_t*)dialogTitleUTF8
                                       AndParentWindow:(void*) parentWindow
                                                   AtX:(uint32_t)positionX
                                                  AndY:(uint32_t) positionY;
@end

#endif // WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IOS_DEVICE_INFO_IOS_OBJC_H_
