/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_RENDER_MAIN_SOURCE_ANDROID_VIDEO_RENDER_OPENGLES20_H_
#define WEBRTC_MODULES_VIDEO_RENDER_MAIN_SOURCE_ANDROID_VIDEO_RENDER_OPENGLES20_H_

#include "video_render_defines.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

namespace webrtc
{

class VideoRenderDelegeteGLES_2_0 {
 public:
  VideoRenderDelegeteGLES_2_0(int32_t id);
  ~VideoRenderDelegeteGLES_2_0();

  int32_t Setup(int32_t widht, int32_t height);
  int32_t Render(const I420VideoFrame& frameToRender);
  int32_t SetCoordinates(int32_t zOrder, const float left, const float top,
                         const float right, const float bottom);

 private:
  void printGLString(const char *name, GLenum s);
  void checkGlError(const char* op);
  GLuint loadShader(GLenum shaderType, const char* pSource);
  GLuint createProgram(const char* pVertexSource,
                       const char* pFragmentSource);
  void SetupTextures(const I420VideoFrame& frameToRender);
  void UpdateTextures(const I420VideoFrame& frameToRender);

  int32_t _id;
  GLuint _textureIds[3]; // Texture id of Y,U and V texture.
  GLuint _program;
  GLsizei _textureWidth;
  GLsizei _textureHeight;

  GLfloat _vertices[20];
  static const char g_indices[];

  static const char g_vertextShader[];
  static const char g_fragmentShader[];

};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_VIDEO_RENDER_MAIN_SOURCE_ANDROID_VIDEO_RENDER_OPENGLES20_H_
