// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_BUFFER_H_
#define PPAPI_PROXY_PLUGIN_BUFFER_H_

#include "native_client/src/include/nacl_macros.h"
#include "ppapi/c/dev/ppb_buffer_dev.h"

namespace ppapi_proxy {

// Implements the plugin (i.e., .nexe) side of the PPB_Buffer interface.
class PluginBuffer {
 public:
  static const PPB_Buffer_Dev* GetInterface();

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PluginBuffer);
};

}  // namespace ppapi_proxy

#endif  // PPAPI_PROXY_PLUGIN_BUFFER_H_
