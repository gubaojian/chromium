// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PPB_VIDEO_CAPTURE_PROXY_H_
#define PPAPI_PPB_VIDEO_CAPTURE_PROXY_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/proxy/interface_proxy.h"
#include "ppapi/proxy/serialized_structs.h"

struct PPB_VideoCapture_Dev;
struct PPP_VideoCapture_Dev;
struct PP_VideoCaptureDeviceInfo_Dev;

namespace ppapi {
class HostResource;
}

namespace pp {
namespace proxy {

class PPB_VideoCapture_Proxy : public InterfaceProxy {
 public:
  PPB_VideoCapture_Proxy(Dispatcher* dispatcher, const void* target_interface);
  virtual ~PPB_VideoCapture_Proxy();

  static const Info* GetInfo();

  static PP_Resource CreateProxyResource(PP_Instance instance);

  const PPB_VideoCapture_Dev* ppb_video_capture_target() const {
    return static_cast<const PPB_VideoCapture_Dev*>(target_interface());
  }

  // InterfaceProxy implementation.
  virtual bool OnMessageReceived(const IPC::Message& msg);

 private:
  // Message handlers.
  void OnMsgCreate(PP_Instance instance, ppapi::HostResource* result_resource);
  void OnMsgStartCapture(const ppapi::HostResource& resource,
                         const PP_VideoCaptureDeviceInfo_Dev& info,
                         uint32_t buffers);
  void OnMsgReuseBuffer(const ppapi::HostResource& resource,
                        uint32_t buffer);
  void OnMsgStopCapture(const ppapi::HostResource& resource);
};

class PPP_VideoCapture_Proxy : public InterfaceProxy {
 public:
  PPP_VideoCapture_Proxy(Dispatcher* dispatcher, const void* target_interface);
  virtual ~PPP_VideoCapture_Proxy();

  static const Info* GetInfo();

  const PPP_VideoCapture_Dev* ppp_video_capture_target() const {
    return static_cast<const PPP_VideoCapture_Dev*>(target_interface());
  }

  // InterfaceProxy implementation.
  virtual bool OnMessageReceived(const IPC::Message& msg);

 private:
  // Message handlers.
  void OnMsgOnDeviceInfo(const ppapi::HostResource& video_capture,
                         const PP_VideoCaptureDeviceInfo_Dev& info,
                         const std::vector<PPPVideoCapture_Buffer>& buffers);
  void OnMsgOnStatus(const ppapi::HostResource& video_capture,
                     uint32_t status);
  void OnMsgOnError(const ppapi::HostResource& video_capture,
                    uint32_t error_code);
  void OnMsgOnBufferReady(const ppapi::HostResource& video_capture,
                          uint32_t buffer);
};

}  // namespace proxy
}  // namespace pp

#endif  // PPAPI_PPB_VIDEO_CAPTURE_PROXY_H_
