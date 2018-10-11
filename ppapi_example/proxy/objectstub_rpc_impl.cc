// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_scoped_ptr.h"
#include "native_client/src/include/portability.h"
#include "native_client/src/include/portability_process.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/proxy/generated/ppb_rpc_server.h"
//#include "ppapi/proxy/generated/ppp_rpc_server.h"
#include "ppapi/proxy/object_capability.h"
#include "ppapi/proxy/object_serialize.h"
#include "ppapi/proxy/utility.h"

#ifdef __native_client__
#include "ppapi/proxy/plugin_globals.h"
#else
#include "ppapi/proxy/browser_globals.h"
#endif  // __native_client__

//
// These methods provide dispatching to the implementation of the object stubs.
//

using ppapi_proxy::DebugPrintf;
using ppapi_proxy::ObjectCapability;
using ppapi_proxy::DeserializeTo;
using ppapi_proxy::SerializeTo;
using ppapi_proxy::VarInterface;

namespace {

PP_Var LookupCapability(const ObjectCapability* capability) {
  if (capability->pid() != GETPID()) {
    DebugPrintf("WHOA! PIDS DON'T MATCH\n");
  }
  PP_Var var;
  var.type = PP_VARTYPE_OBJECT;
  var.value.as_id = capability->object_id();
  return var;
}

}  // namespace

NaClSrpcError ObjectStubRpcServer::HasProperty(NaClSrpcChannel* channel,
                                               uint32_t capability_length,
                                               char* capability_bytes,
                                               uint32_t name_length,
                                               char* name_bytes,
                                               uint32_t ex_in_length,
                                               char* ex_in_bytes,
                                               int32_t* success,
                                               uint32_t* exception_length,
                                               char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::HasProperty\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the name PP_Var.
  PP_Var name;
  if (!DeserializeTo(channel, name_bytes, name_length, 1, &name)) {
    // Deserialization of name failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  *success = VarInterface()->HasProperty(var, name, &exception);
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::HasMethod(NaClSrpcChannel* channel,
                                             uint32_t capability_length,
                                             char* capability_bytes,
                                             uint32_t name_length,
                                             char* name_bytes,
                                             uint32_t ex_in_length,
                                             char* ex_in_bytes,
                                             int32_t* success,
                                             uint32_t* exception_length,
                                             char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::HasMethod\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the name PP_Var.
  PP_Var name;
  if (!DeserializeTo(channel, name_bytes, name_length, 1, &name)) {
    // Deserialization of name failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  *success = VarInterface()->HasMethod(var, name, &exception);
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::GetProperty(NaClSrpcChannel* channel,
                                               uint32_t capability_length,
                                               char* capability_bytes,
                                               uint32_t name_length,
                                               char* name_bytes,
                                               uint32_t ex_in_length,
                                               char* ex_in_bytes,
                                               uint32_t* value_length,
                                               char* value_bytes,
                                               uint32_t* exception_length,
                                               char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::GetProperty\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the name PP_Var.
  PP_Var name;
  if (!DeserializeTo(channel, name_bytes, name_length, 1, &name)) {
    // Deserialization of name failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  PP_Var value = VarInterface()->GetProperty(var, name, &exception);
  // Return the value PP_Var.
  if (!SerializeTo(&value, value_bytes, value_length)) {
    // Serialization of value failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::GetAllPropertyNames(
    NaClSrpcChannel* channel,
    uint32_t capability_length,
    char* capability_bytes,
    uint32_t ex_in_length,
    char* ex_in_bytes,
    int32_t* property_count,
    uint32_t* properties_length,
    char* properties_bytes,
    uint32_t* exception_length,
    char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::GetAllPropertyNames\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  //PP_Var var =
  //    LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  UNREFERENCED_PARAMETER(property_count);
  UNREFERENCED_PARAMETER(properties_length);
  UNREFERENCED_PARAMETER(properties_bytes);
  // Invoke the method.
  // TODO(sehr): implement GetAllPropertyNames.
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::SetProperty(NaClSrpcChannel* channel,
                                               uint32_t capability_length,
                                               char* capability_bytes,
                                               uint32_t name_length,
                                               char* name_bytes,
                                               uint32_t value_length,
                                               char* value_bytes,
                                               uint32_t ex_in_length,
                                               char* ex_in_bytes,
                                               uint32_t* exception_length,
                                               char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::SetProperty\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the name PP_Var.
  PP_Var name;
  if (!DeserializeTo(channel, name_bytes, name_length, 1, &name)) {
    // Deserialization of name failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the value PP_Var.
  PP_Var value;
  if (!DeserializeTo(channel, value_bytes, value_length, 1, &value)) {
    // Deserialization of value failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the previous exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  VarInterface()->SetProperty(var, name, value, &exception);
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::RemoveProperty(NaClSrpcChannel* channel,
                                                  uint32_t capability_length,
                                                  char* capability_bytes,
                                                  uint32_t name_length,
                                                  char* name_bytes,
                                                  uint32_t ex_in_length,
                                                  char* ex_in_bytes,
                                                  uint32_t* exception_length,
                                                  char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::RemoveProperty\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the name PP_Var.
  PP_Var name;
  if (!DeserializeTo(channel, name_bytes, name_length, 1, &name)) {
    // Deserialization of name failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  VarInterface()->RemoveProperty(var, name, &exception);
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::Call(NaClSrpcChannel* channel,
                                        uint32_t capability_length,
                                        char* capability_bytes,
                                        uint32_t name_length,
                                        char* name_bytes,
                                        int32_t argc,
                                        uint32_t argv_length,
                                        char* argv_bytes,
                                        uint32_t ex_in_length,
                                        char* ex_in_bytes,
                                        uint32_t* ret_length,
                                        char* ret_bytes,
                                        uint32_t* exception_length,
                                        char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::Call\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the name PP_Var.
  PP_Var name;
  if (!DeserializeTo(channel, name_bytes, name_length, 1, &name)) {
    // Deserialization of name failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Get the parameters.
  nacl::scoped_array<PP_Var> argv(new PP_Var[argc]);
  if (!DeserializeTo(channel, argv_bytes, argv_length, argc, argv.get())) {
    // Deserialization of argv failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  PP_Var ret = VarInterface()->Call(var,
                                    name,
                                    static_cast<uint32_t>(argc),
                                    argv.get(),
                                    &exception);
  // Return ret.
  if (!SerializeTo(&ret, ret_bytes, ret_length)) {
    // Serialization of ret failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}

NaClSrpcError ObjectStubRpcServer::Construct(NaClSrpcChannel* channel,
                                             uint32_t capability_length,
                                             char* capability_bytes,
                                             int32_t argc,
                                             uint32_t argv_length,
                                             char* argv_bytes,
                                             uint32_t ex_in_length,
                                             char* ex_in_bytes,
                                             uint32_t* ret_length,
                                             char* ret_bytes,
                                             uint32_t* exception_length,
                                             char* exception_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::Construct\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  PP_Var var =
      LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Get the previous value of the exception PP_Var.
  PP_Var exception;
  if (!DeserializeTo(channel, ex_in_bytes, ex_in_length, 1, &exception)) {
    // Deserialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  nacl::scoped_array<PP_Var> argv(new PP_Var[argc]);
  if (!DeserializeTo(channel, argv_bytes, argv_length, argc, argv.get())) {
    // Deserialization of argv failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Invoke the method.
  PP_Var ret = VarInterface()->Construct(var,
                                         static_cast<uint32_t>(argc),
                                         argv.get(),
                                         &exception);
  // Return ret.
  if (!SerializeTo(&ret, ret_bytes, ret_length)) {
    // Serialization of ret failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // Return the final value of the exception PP_Var.
  if (!SerializeTo(&exception, exception_bytes, exception_length)) {
    // Serialization of exception failed.
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  return NACL_SRPC_RESULT_OK;
}


NaClSrpcError ObjectStubRpcServer::Deallocate(NaClSrpcChannel* channel,
                                              uint32_t capability_length,
                                              char* capability_bytes) {
  UNREFERENCED_PARAMETER(channel);
  DebugPrintf("ObjectStubRpcServer::Deallocate\n");
  // Get the receiver object.
  if (capability_length != sizeof(ObjectCapability)) {
    return NACL_SRPC_RESULT_APP_ERROR;
  }
  // PP_Var var =
  //   LookupCapability(reinterpret_cast<ObjectCapability*>(capability_bytes));
  // Invoke the method.
  return NACL_SRPC_RESULT_OK;
}
