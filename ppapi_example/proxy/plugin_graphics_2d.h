// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_GRAPHICS_2D_H_
#define PPAPI_PROXY_PLUGIN_GRAPHICS_2D_H_

#include "native_client/src/include/nacl_macros.h"
#include "ppapi/c/ppb_graphics_2d.h"

namespace ppapi_proxy {

// Implements the plugin (i.e., .nexe) side of the PPB_Graphics2D interface.
class PluginGraphics2D {
 public:
  static const PPB_Graphics2D* GetInterface();

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PluginGraphics2D);
};

}  // namespace ppapi_proxy

#endif  // PPAPI_PROXY_PLUGIN_GRAPHICS_2D_H_
