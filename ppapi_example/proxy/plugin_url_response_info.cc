// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_url_response_info.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/dev/ppb_url_response_info_dev.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
bool IsURLResponseInfo(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

PP_Var GetProperty(PP_Resource response,
                   PP_URLResponseProperty_Dev property) {
  UNREFERENCED_PARAMETER(response);
  UNREFERENCED_PARAMETER(property);
  return PP_MakeUndefined();
}

PP_Resource GetBody(PP_Resource response) {
  UNREFERENCED_PARAMETER(response);
  return kInvalidResourceId;
}
}  // namespace

const PPB_URLResponseInfo_Dev* PluginURLResponseInfo::GetInterface() {
  static const PPB_URLResponseInfo_Dev intf = {
    IsURLResponseInfo,
    GetProperty,
    GetBody,
  };
  return &intf;
}

}  // namespace ppapi_proxy
