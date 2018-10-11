// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_audio.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/dev/ppb_audio_config_dev.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
PP_Resource Create(PP_Instance instance,
                   PP_Resource config,
                   PPB_Audio_Callback audio_callback,
                   void* user_data) {
  UNREFERENCED_PARAMETER(instance);
  UNREFERENCED_PARAMETER(config);
  UNREFERENCED_PARAMETER(audio_callback);
  UNREFERENCED_PARAMETER(user_data);
  return kInvalidResourceId;
}

bool IsAudio(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

PP_Resource GetCurrentConfig(PP_Resource audio) {
  UNREFERENCED_PARAMETER(audio);
  return kInvalidResourceId;
}

bool StartPlayback(PP_Resource audio) {
  UNREFERENCED_PARAMETER(audio);
  return false;
}

bool StopPlayback(PP_Resource audio) {
  UNREFERENCED_PARAMETER(audio);
  return false;
}
}  // namespace

const PPB_Audio_Dev* PluginAudio::GetInterface() {
  static const PPB_Audio_Dev intf = {
    Create,
    IsAudio,
    GetCurrentConfig,
    StartPlayback,
    StopPlayback,
  };
  return &intf;
}

}  // namespace ppapi_proxy
