// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_file_io.h"

#include <stdio.h>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/dev/ppb_file_io_dev.h"
#include "ppapi/c/dev/ppb_testing_dev.h"
#include "ppapi/cpp/dev/file_io_dev.h"
#include "ppapi/cpp/dev/file_ref_dev.h"
#include "ppapi/cpp/dev/file_system_dev.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/tests/testing_instance.h"

REGISTER_TEST_CASE(FileIO);

namespace {

const PPB_Testing_Dev* g_testing_interface;

class TestCompletionCallback {
 public:
  TestCompletionCallback() : result_(PP_ERROR_WOULDBLOCK) {
  }

  operator pp::CompletionCallback() const {
    return pp::CompletionCallback(&TestCompletionCallback::Handler,
                                  const_cast<TestCompletionCallback*>(this));
  }

  int32_t WaitForResult() {
    result_ = PP_ERROR_WOULDBLOCK;  // Reset
    g_testing_interface->RunMessageLoop();
    return result_;
  }

 private:
  static void Handler(void* user_data, int32_t result) {
    static_cast<TestCompletionCallback*>(user_data)->result_ = result;
    g_testing_interface->QuitMessageLoop();
  }

  int32_t result_;
};

std::string ReportError(const char* method, int32_t error) {
  char error_as_string[12];
  sprintf(error_as_string, "%d", error);
  std::string result = method + std::string(" failed with error: ") +
      error_as_string;
  if (error == PP_ERROR_NOSPACE)
    result += ". Did you run the test with --unlimited-quota-for-files?";
  return result;
}

std::string ReportMismatch(const std::string& method_name,
                           const std::string& returned_result,
                           const std::string& expected_result) {
  return method_name + " returned '" + returned_result + "'; '" +
      expected_result + "' expected.";
}

int32_t ReadEntireFile(pp::FileIO_Dev* file_io,
                       int32_t offset,
                       std::string* data) {
  TestCompletionCallback callback;
  char buf[256];
  int32_t read_offset = offset;

  for (;;) {
    int32_t rv = file_io->Read(read_offset, buf, sizeof(buf), callback);
    if (rv == PP_ERROR_WOULDBLOCK)
      rv = callback.WaitForResult();
    if (rv < 0)
      return rv;
    if (rv == 0)
      break;
    read_offset += rv;
    data->append(buf, rv);
  }

  return PP_OK;
}

int32_t WriteEntireBuffer(pp::FileIO_Dev* file_io,
                          int32_t offset,
                          const std::string& data) {
  TestCompletionCallback callback;
  int32_t write_offset = offset;
  const char* buf = data.c_str();
  int32_t size = data.size();

  while (write_offset < offset + size) {
    int32_t rv = file_io->Write(write_offset, &buf[write_offset - offset],
                                size - write_offset + offset, callback);
    if (rv == PP_ERROR_WOULDBLOCK)
      rv = callback.WaitForResult();
    if (rv < 0)
      return rv;
    if (rv == 0)
      return PP_ERROR_FAILED;
    write_offset += rv;
  }

  return PP_OK;
}

}  // namespace

bool TestFileIO::Init() {
  g_testing_interface = reinterpret_cast<PPB_Testing_Dev const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_TESTING_DEV_INTERFACE));
  if (!g_testing_interface) {
    // Give a more helpful error message for the testing interface being gone
    // since that needs special enabling in Chrome.
    instance_->AppendError("This test needs the testing interface, which is "
        "not currently available. In Chrome, use --enable-pepper-testing when "
        "launching.");
    return false;
  }

  // Make sure we're running over HTTP.
  pp::Var window = instance_->GetWindowObject();
  pp::Var location = window.GetProperty("location");
  pp::Var protocol = location.GetProperty("protocol");
  if (!protocol.is_string() || protocol.AsString() != "http:") {
    instance_->AppendError("This test needs to be run over HTTP.");
    return false;
  }

  return true;
}

void TestFileIO::RunTest() {
  RUN_TEST(Open);
  RUN_TEST(ReadWriteSetLength);
  RUN_TEST(TouchQuery);
}

std::string TestFileIO::TestOpen() {
  TestCompletionCallback callback;

  pp::FileSystem_Dev file_system(instance_, PP_FILESYSTEMTYPE_LOCALTEMPORARY);
  pp::FileRef_Dev file_ref(file_system, "/file_open");
  int32_t rv = file_system.Open(1024, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileSystem::Open", rv);

  pp::FileIO_Dev file_io;
  rv = file_io.Open(file_ref, PP_FILEOPENFLAG_CREATE, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileIO::Open", rv);

  // Try opening a file that doesn't exist.
  pp::FileRef_Dev nonexistent_file_ref(file_system, "/nonexistent_file");
  pp::FileIO_Dev nonexistent_file_io;
  rv = nonexistent_file_io.Open(
      nonexistent_file_ref, PP_FILEOPENFLAG_READ, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_ERROR_FILENOTFOUND)
    return ReportError("FileIO::Open", rv);

  return "";
}

std::string TestFileIO::TestReadWriteSetLength() {
  TestCompletionCallback callback;

  pp::FileSystem_Dev file_system(instance_, PP_FILESYSTEMTYPE_LOCALTEMPORARY);
  pp::FileRef_Dev file_ref(file_system, "/file_read_write_setlength");
  int32_t rv = file_system.Open(1024, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileSystem::Open", rv);

  pp::FileIO_Dev file_io;
  rv = file_io.Open(file_ref,
                    PP_FILEOPENFLAG_CREATE |
                    PP_FILEOPENFLAG_READ |
                    PP_FILEOPENFLAG_WRITE,
                    callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileIO::Open", rv);

  // Write something to the file.
  rv = WriteEntireBuffer(&file_io, 0, "test_test");
  if (rv != PP_OK)
    return ReportError("FileIO::Write", rv);

  // Read the entire file.
  std::string read_buffer;
  rv = ReadEntireFile(&file_io, 0, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (read_buffer != "test_test")
    return ReportMismatch("FileIO::Read", read_buffer, "test_test");

  // Truncate the file.
  rv = file_io.SetLength(4, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileIO::SetLength", rv);

  // Check the file contents.
  read_buffer.clear();
  rv = ReadEntireFile(&file_io, 0, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (read_buffer != "test")
    return ReportMismatch("FileIO::Read", read_buffer, "test");

  // Try to read past the end of the file.
  read_buffer.clear();
  rv = ReadEntireFile(&file_io, 100, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (!read_buffer.empty())
    return ReportMismatch("FileIO::Read", read_buffer, "<empty string>");

  // Write past the end of the file. The file should be zero-padded.
  rv = WriteEntireBuffer(&file_io, 8, "test");
  if (rv != PP_OK)
    return ReportError("FileIO::Write", rv);

  // Check the contents of the file.
  read_buffer.clear();
  rv = ReadEntireFile(&file_io, 0, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (read_buffer != std::string("test\0\0\0\0test", 12))
    return ReportMismatch("FileIO::Read", read_buffer,
                          std::string("test\0\0\0\0test", 12));

  // Extend the file.
  rv = file_io.SetLength(16, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileIO::SetLength", rv);

  // Check the contents of the file.
  read_buffer.clear();
  rv = ReadEntireFile(&file_io, 0, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (read_buffer != std::string("test\0\0\0\0test\0\0\0\0", 16))
    return ReportMismatch("FileIO::Read", read_buffer,
                          std::string("test\0\0\0\0test\0\0\0\0", 16));

  // Write in the middle of the file.
  rv = WriteEntireBuffer(&file_io, 4, "test");
  if (rv != PP_OK)
    return ReportError("FileIO::Write", rv);

  // Check the contents of the file.
  read_buffer.clear();
  rv = ReadEntireFile(&file_io, 0, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (read_buffer != std::string("testtesttest\0\0\0\0", 16))
    return ReportMismatch("FileIO::Read", read_buffer,
                          std::string("testtesttest\0\0\0\0", 16));

  // Read from the middle of the file.
  read_buffer.clear();
  rv = ReadEntireFile(&file_io, 4, &read_buffer);
  if (rv != PP_OK)
    return ReportError("FileIO::Read", rv);
  if (read_buffer != std::string("testtest\0\0\0\0", 12))
    return ReportMismatch("FileIO::Read", read_buffer,
                          std::string("testtest\0\0\0\0", 12));

  return "";
}

std::string TestFileIO::TestTouchQuery() {
  TestCompletionCallback callback;

  pp::FileSystem_Dev file_system(instance_, PP_FILESYSTEMTYPE_LOCALTEMPORARY);
  int32_t rv = file_system.Open(1024, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileSystem::Open", rv);

  pp::FileRef_Dev file_ref(file_system, "/file_touch");
  pp::FileIO_Dev file_io;
  rv = file_io.Open(file_ref,
                    PP_FILEOPENFLAG_CREATE | PP_FILEOPENFLAG_WRITE,
                    callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileIO::Open", rv);

  // Write some data to have a non-zero file size.
  rv = file_io.Write(0, "test", 4, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != 4)
    return ReportError("FileIO::Write", rv);

  // last_access_time's granularity is 1 day
  // last_modified_time's granularity is 2 seconds
  const PP_Time last_access_time = 123 * 24 * 3600.0;
  const PP_Time last_modified_time = 246.0;
  rv = file_io.Touch(last_access_time, last_modified_time, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileSystem::Touch", rv);

  PP_FileInfo_Dev info;
  rv = file_io.Query(&info, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileSystem::Query", rv);

  if ((info.size != 4) ||
      (info.type != PP_FILETYPE_REGULAR) ||
      (info.system_type != PP_FILESYSTEMTYPE_LOCALTEMPORARY) ||
      (info.last_access_time != last_access_time) ||
      (info.last_modified_time != last_modified_time))
    return "FileSystem::Query() has returned bad data.";

  return "";
}
