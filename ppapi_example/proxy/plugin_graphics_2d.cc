// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_graphics_2d.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/generated/upcall_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
PP_Resource Create(PP_Module module,
                   const struct PP_Size* size,
                   bool is_always_opaque) {
  UNREFERENCED_PARAMETER(module);
  UNREFERENCED_PARAMETER(size);
  UNREFERENCED_PARAMETER(is_always_opaque);
  return kInvalidResourceId;
}

bool IsGraphics2D(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

bool Describe(PP_Resource device_context,
              struct PP_Size* size,
              bool* is_always_opaque) {
  UNREFERENCED_PARAMETER(device_context);
  UNREFERENCED_PARAMETER(size);
  UNREFERENCED_PARAMETER(is_always_opaque);
  return false;
}

void PaintImageData(PP_Resource device_context,
                    PP_Resource image,
                    const struct PP_Point* top_left,
                    const struct PP_Rect* src_rect) {
  UNREFERENCED_PARAMETER(device_context);
  UNREFERENCED_PARAMETER(image);
  UNREFERENCED_PARAMETER(top_left);
  UNREFERENCED_PARAMETER(src_rect);
}

void Scroll(PP_Resource device_context,
            const struct PP_Rect* clip_rect,
            const struct PP_Point* amount) {
  UNREFERENCED_PARAMETER(device_context);
  UNREFERENCED_PARAMETER(clip_rect);
  UNREFERENCED_PARAMETER(amount);
}

void ReplaceContents(PP_Resource device_context, PP_Resource image) {
  UNREFERENCED_PARAMETER(device_context);
  UNREFERENCED_PARAMETER(image);
}

int32_t Flush(PP_Resource device_context,
              struct PP_CompletionCallback callback) {
  UNREFERENCED_PARAMETER(device_context);
  UNREFERENCED_PARAMETER(callback);
  return PP_ERROR_BADRESOURCE;
}
}  // namespace

const PPB_Graphics2D* PluginGraphics2D::GetInterface() {
  static const PPB_Graphics2D intf = {
    Create,
    IsGraphics2D,
    Describe,
    PaintImageData,
    Scroll,
    ReplaceContents,
    Flush,
  };
  return &intf;
}

}  // namespace ppapi_proxy
