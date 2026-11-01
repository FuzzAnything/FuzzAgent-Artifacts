// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <functional>
#include "tiny_gltf.h"

// Define function pointer types matching tinygltf's expected signatures exactly
// Based on the error messages, the expected signatures are:
// URIEncodeFunction: bool(const std::string&, const std::string&, std::string*, void*)
// URIDecodeFunction: bool(const std::string&, std::string*, void*)
// GetFileSizeFunction: bool(size_t*, const std::string&, const std::string&, void*)
// Note: The error messages suggest specific signatures that might differ from standard docs.
// We will define them to match what the library expects based on the compilation errors.

// Re-defining based on error analysis:
// URIEncodeFunction seems to be: bool(const std::string&, const std::string&, std::string*, void*)
// URIDecodeFunction seems to be: bool(const std::string&, std::string*, void*)
// GetFileSizeInBytes seems to be: bool(size_t*, const std::string&, const std::string&, void*)
// However, looking at standard tinygltf, the signatures are usually:
// URIEncode: std::string(const std::string&, void*)
// URIDecode: std::string(const std::string&, void*)
// GetFileSize: bool(size_t*, const std::string&, void*)
// The error message shows:
// "no known conversion ... to 'const function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'"
// This implies the library expects a bool return for URIEncode.
// Let's try to match the error message's expectation strictly.

// Actually, looking at the error:
// "candidate function not viable: no known conversion from 'std::string (const std::string &, void *)' ... to 'const function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'"
// This means the struct expects: bool(const string&, const string&, string*, void*) for encode.
// And for decode: "to 'const function<bool (const basic_string<char> &, basic_string<char> *, void *)>'"
// This means the struct expects: bool(const string&, string*, void*) for decode.
// And for GetFileSize: "to 'const function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'"
// This means the struct expects: bool(size_t*, string*, const string&, void*) for GetFileSize.

// Let's define the static functions to match these specific signatures.

static bool StaticURIEncode(const std::string &str, const std::string &unused, std::string *out, void *user_data) {
  (void)user_data;
  (void)unused;
  if (out) *out = str;
  return true;
}

static bool StaticURIDecode(const std::string &str, std::string *out, void *user_data) {
  (void)user_data;
  if (out) *out = str;
  return true;
}

static bool StaticFileExists(const std::string &abs_path, void *user_data) {
  (void)abs_path;
  (void)user_data;
  return true;
}

static std::string StaticExpandFilePath(const std::string &filepath, void *user_data) {
  (void)filepath;
  (void)user_data;
  return filepath;
}

static bool StaticReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                                const std::string &filepath, void *user_data) {
  (void)user_data;
  std::ifstream f(filepath, std::ios::binary);
  if (!f) {
    if (err) *err = "File not found: " + filepath;
    return false;
  }
  out->assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
  return true;
}

static bool StaticWriteWholeFile(std::string *err, const std::string &filepath,
                                 const std::vector<unsigned char> &contents, void *user_data) {
  (void)user_data;
  std::ofstream f(filepath, std::ios::binary);
  if (!f) {
    if (err) *err = "Cannot write file: " + filepath;
    return false;
  }
  f.write(reinterpret_cast<const char*>(contents.data()), contents.size());
  return true;
}

// Matching the error signature: bool(size_t*, const std::string&, const std::string&, void*)
// The error message says: "bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)"
// Wait, the error says: "basic_string<char> *" as the second argument?
// Let's re-read: "to 'const function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'"
// So: bool(size_t*, std::string*, const std::string&, void*)
static bool StaticGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *user_data) {
  (void)user_data;
  std::ifstream f(filepath, std::ios::binary | std::ios::ate);
  if (!f) {
    if (err) *err = "Cannot open file";
    return false;
  }
  *size = static_cast<size_t>(f.tellg());
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Write fuzzing data to dummy files
  const std::string dummy_glb = "./dummy_file.glb";
  const std::string dummy_gltf = "./dummy_file.gltf";

  {
    std::ofstream f_glb(dummy_glb, std::ios::binary);
    if (f_glb) {
      f_glb.write(reinterpret_cast<const char*>(Data), Size);
    }
  }

  {
    std::ofstream f_gltf(dummy_gltf, std::ios::binary);
    if (f_gltf) {
      f_gltf.write(reinterpret_cast<const char*>(Data), Size);
    }
  }

  // 1. Test SetURICallbacks with valid callbacks
  tinygltf::URICallbacks uri_callbacks;
  uri_callbacks.encode = StaticURIEncode;
  uri_callbacks.decode = StaticURIDecode;
  uri_callbacks.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks, &err);

  // 2. Test SetURICallbacks with null callbacks (should return false)
  tinygltf::URICallbacks uri_callbacks_null;
  uri_callbacks_null.encode = nullptr;
  uri_callbacks_null.decode = nullptr;
  uri_callbacks_null.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks_null, &err);

  // 3. Test SetFsCallbacks with valid callbacks
  tinygltf::FsCallbacks fs_callbacks;
  fs_callbacks.FileExists = StaticFileExists;
  fs_callbacks.ExpandFilePath = StaticExpandFilePath;
  fs_callbacks.ReadWholeFile = StaticReadWholeFile;
  fs_callbacks.WriteWholeFile = StaticWriteWholeFile;
  fs_callbacks.GetFileSizeInBytes = StaticGetFileSizeInBytes;
  fs_callbacks.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks, &err);

  // 4. Test SetFsCallbacks with null callbacks (should return false)
  tinygltf::FsCallbacks fs_callbacks_null;
  fs_callbacks_null.FileExists = nullptr;
  fs_callbacks_null.ExpandFilePath = nullptr;
  fs_callbacks_null.ReadWholeFile = nullptr;
  fs_callbacks_null.WriteWholeFile = nullptr;
  fs_callbacks_null.GetFileSizeInBytes = nullptr;
  fs_callbacks_null.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks_null, &err);

  // 5. Test LoadBinaryFromFile
  ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_glb, tinygltf::REQUIRE_VERSION);

  // 6. Test LoadASCIIFromFile
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_gltf, tinygltf::REQUIRE_VERSION);

  // 7. Test LoadASCIIFromString
  std::string str(reinterpret_cast<const char*>(Data), Size);
  ctx.LoadASCIIFromString(&model, &err, &warn, str.c_str(), str.length(), "", tinygltf::REQUIRE_VERSION);

  // 8. Test RemoveImageLoader
  ctx.RemoveImageLoader();

  return 0;
}