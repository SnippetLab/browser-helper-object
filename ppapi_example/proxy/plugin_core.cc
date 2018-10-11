// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_core.h"

#include <stdio.h>
#include <map>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/portability.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

using ppapi_proxy::DebugPrintf;

// All of the methods here are invoked from the plugin's main (UI) thread,
// so no locking is done.

namespace {

std::map<PP_Resource, uint32_t>* local_ref_count;

// Increment the reference count for a specified resource.  This is done with
// a plugin-side cache, maintained by this helper function.  Only the first time
// a resource has an AddRef call requires an RPC to the browser.
void AddRefResource(PP_Resource resource) {
  DebugPrintf("PluginCore::AddRefResource: resource=%"NACL_PRIu64"\n",
              resource);
  if (local_ref_count == NULL) {
    local_ref_count = new std::map<PP_Resource, uint32_t>;
  }
  if (local_ref_count->find(resource) == local_ref_count->end()) {
    // The first time we create a local ref count object we need to increment
    // the reference count in the browser.
    NaClSrpcChannel* channel = ppapi_proxy::GetMainSrpcChannel();
    (void) PpbCoreRpcClient::PPB_Core_AddRefResource(channel, resource);
    // And add a local (cached) reference count object.
    (*local_ref_count)[resource] = 1;
    return;
  } else {
    // Otherwise, just update the local reference count.
    (*local_ref_count)[resource] = (*local_ref_count)[resource] + 1;
  }
}

void ReleaseResource(PP_Resource resource) {
  DebugPrintf("PluginCore::ReleaseResource: resource=%"NACL_PRIu64"\n",
              resource);
  if (local_ref_count == NULL ||
      local_ref_count->find(resource) == local_ref_count->end()) {
    // ERROR: How can we decrement if there is no local (cached) reference
    // count for the specified resource?
    return;
  }
  if ((*local_ref_count)[resource] > 1) {
    // Just update the local (cached) reference count.
    (*local_ref_count)[resource] = (*local_ref_count)[resource] - 1;
    return;
  } else {
    // When the local reference count goes to zero we decrement the reference
    // count in the browser.
    NaClSrpcChannel* channel = ppapi_proxy::GetMainSrpcChannel();
    (void) PpbCoreRpcClient::PPB_Core_ReleaseResource(channel, resource);
    // Then remove the local reference count for the resource.
    local_ref_count->erase(resource);
    if (local_ref_count->size() == 0) {
      // There are no locally reference counted objects, free the map.
      delete local_ref_count;
      local_ref_count = NULL;
    }
  }
}

void* MemAlloc(size_t num_bytes) {
  DebugPrintf("PluginCore::MemAlloc: num_bytes=%"NACL_PRIuS"\n", num_bytes);
  return malloc(num_bytes);
}

void MemFree(void* ptr) {
  DebugPrintf("PluginCore::MemFree: ptr=%p\n", ptr);
  free(ptr);
}

PP_Time GetTime() {
  DebugPrintf("PluginCore::GetTime\n");
  NaClSrpcChannel* channel = ppapi_proxy::GetMainSrpcChannel();
  double time;
  NaClSrpcError retval = PpbCoreRpcClient::PPB_Core_GetTime(channel, &time);
  if (retval != NACL_SRPC_RESULT_OK) {
    return static_cast<PP_Time>(-1.0);
  } else {
    return static_cast<PP_Time>(time);
  }
}

PP_TimeTicks GetTimeTicks() {
  DebugPrintf("PluginCore::GetTimeTicks\n");
  // TODO(brettw) implement this function properly (being the same as GetTime
  // is the same as we have in the browser now).
  return GetTime();
}


static void CallOnMainThread(int32_t delay_in_milliseconds,
                             PP_CompletionCallback callback,
                             int32_t result) {
  UNREFERENCED_PARAMETER(callback);
  DebugPrintf("PluginCore::CallOnMainThread: delay=%" NACL_PRIu32
              ", result=%" NACL_PRIu32 "\n",
              delay_in_milliseconds,
              result);
  NACL_UNIMPLEMENTED();
  // See how NPN_PluginThreadAsyncCall is implemented in npruntime.
}

static bool IsMainThread() {
  DebugPrintf("PluginCore::IsMainThread\n");
  NACL_UNIMPLEMENTED();
}

}  // namespace

namespace ppapi_proxy {

const void* PluginCore::GetInterface() {
  static const PPB_Core intf = {
    AddRefResource,
    ReleaseResource,
    MemAlloc,
    MemFree,
    GetTime,
    GetTimeTicks,
    CallOnMainThread,
    IsMainThread
  };
  return reinterpret_cast<const void*>(&intf);
}

}  // namespace ppapi_proxy
