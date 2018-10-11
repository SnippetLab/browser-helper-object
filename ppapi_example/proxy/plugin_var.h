// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_VAR_H_
#define PPAPI_PROXY_PLUGIN_VAR_H_

#include <string>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/portability.h"
#include "ppapi/c/dev/ppb_var_deprecated.h"
#include "ppapi/c/dev/ppp_class_deprecated.h"
#include "ppapi/c/pp_var.h"

namespace ppapi_proxy {

// Implements the plugin side of the PPB_Var_Deprecated interface.
// This implementation also determines how PP_Vars are represented internally
// in the proxied implementation.
class PluginVar {
 public:
  // Returns an interface pointer suitable to the PPAPI client.
  static const PPB_Var_Deprecated* GetInterface();

  // Print, etc.
  static void Print(PP_Var var);
  static std::string VarToString(PP_Var var);

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PluginVar);
};

}  // namespace ppapi_proxy

#endif  // PPAPI_PROXY_PLUGIN_VAR_H_
