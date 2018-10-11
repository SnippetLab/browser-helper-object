// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_url_request_info.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/dev/ppb_url_request_info_dev.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
PP_Resource Create(PP_Module module) {
  UNREFERENCED_PARAMETER(module);
  return kInvalidResourceId;
}

bool IsURLRequestInfo(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

bool SetProperty(PP_Resource request,
                 PP_URLRequestProperty_Dev property,
                 struct PP_Var value) {
  UNREFERENCED_PARAMETER(request);
  UNREFERENCED_PARAMETER(property);
  UNREFERENCED_PARAMETER(value);
  return false;
}

bool AppendDataToBody(PP_Resource request, const char* data, uint32_t len) {
  UNREFERENCED_PARAMETER(request);
  UNREFERENCED_PARAMETER(data);
  UNREFERENCED_PARAMETER(len);
  return false;
}

bool AppendFileToBody(PP_Resource request,
                      PP_Resource file_ref,
                      int64_t start_offset,
                      int64_t number_of_bytes,
                      PP_Time expected_last_modified_time) {
  UNREFERENCED_PARAMETER(request);
  UNREFERENCED_PARAMETER(file_ref);
  UNREFERENCED_PARAMETER(start_offset);
  UNREFERENCED_PARAMETER(number_of_bytes);
  UNREFERENCED_PARAMETER(expected_last_modified_time);
  return false;
}
}  // namespace

const PPB_URLRequestInfo_Dev* PluginURLRequestInfo::GetInterface() {
  static const PPB_URLRequestInfo_Dev intf = {
    Create,
    IsURLRequestInfo,
    SetProperty,
    AppendDataToBody,
    AppendFileToBody,
  };
  return &intf;
}

}  // namespace ppapi_proxy
