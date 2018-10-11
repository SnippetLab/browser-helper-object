// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_CORE_H_
#define PPAPI_PROXY_PLUGIN_CORE_H_

#include "native_client/src/include/nacl_macros.h"

namespace ppapi_proxy {

// Implements the untrusted side of the PPB_Core interface.
// We will also need an rpc service to implement the trusted side, which is a
// very thin wrapper around the PPB_Core interface returned from the browser.
class PluginCore {
 public:
  // Return an interface pointer usable by PPAPI plugins.
  static const void* GetInterface();

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PluginCore);
};

}  // namespace ppapi_proxy

#endif  // PPAPI_PROXY_PLUGIN_CORE_H_
