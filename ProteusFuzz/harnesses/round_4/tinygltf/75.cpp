// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>
#include "tiny_gltf.h"

// Forward declarations of function pointer types to match tinygltf expectations
// Based on error messages and standard tinygltf usage:

// FileExistsFunction
typedef bool (*FileExistsFunction)(const std::string& filename, void* user_data);

// ExpandFilePathFunction
// Error indicates target is function<std::string(const std::string&, void*)>
typedef std::string (*ExpandFilePathFunction)(const std::string& filename, void* user_data);

// ReadWholeFileFunction
typedef bool (*ReadWholeFileFunction)(std::vector<unsigned char>* out, std::string* err, const std::string& filename, void* user_data);

// WriteWholeFileFunction
typedef bool (*WriteWholeFileFunction)(std::string* err, const std::string& filename, const std::vector<unsigned char>& contents, void* user_data);

// GetFileSizeInBytesFunction
// Error indicates target is function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>
typedef bool (*GetFileSizeInBytesFunction)(unsigned long* size, std::string* err, const std::string& filename, void* user_data);

// URIEncodeFunction
// Error indicates target is function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
typedef bool (*URIEncodeFunction)(const std::string& in, const std::string& /*unused*/, std::string* out, void* user_data);

// URIDecodeFunction
// Error indicates target is function<bool (const basic_string<char> &, basic_string<char> *, void *)>
// Note: The error message for decode specifically shows 2 input args (string, string*) + void*, not 3 strings.
// "no known conversion ... to 'const function<bool (const basic_string<char> &, basic_string<char> *, void *)>'"
// This means the signature is: bool(const std::string& in, std::string* out, void* user_data)
typedef bool (*URIDecodeFunction)(const std::string& in, std::string* out, void* user_data);

static bool FileExistsFunc(const std::string &filename, void *user_data) {
  (void)filename;
  (void)user_data;
  return false;
}

static std::string ExpandFilePathFunc(const std::string &filename, void *user_data) {
  (void)user_data;
  return filename;
}

static bool ReadWholeFileFunc(std::vector<unsigned char> *out,
                              std::string *err, const std::string &filename,
                              void *user_data) {
  (void)out;
  (void)err;
  (void)filename;
  (void)user_data;
  return false;
}

static bool WriteWholeFileFunc(std::string *err, const std::string &filename,
                               const std::vector<unsigned char> &contents,
                               void *user_data) {
  (void)err;
  (void)filename;
  (void)contents;
  (void)user_data;
  return false;
}

static bool GetFileSizeInBytesFunc(unsigned long *size, std::string *err,
                                   const std::string &filename,
                                   void *user_data) {
  (void)err;
  (void)filename;
  (void)user_data;
  if (size) *size = 0;
  return false;
}

static bool URIEncodeFunc(const std::string &in_str, const std::string & /*unused*/, std::string *out, void *user_data) {
  (void)user_data;
  *out = in_str;
  return true;
}

// Fixed signature to match the error message for URIDecodeFunction
static bool URIDecodeFunc(const std::string &in_str, std::string *out, void *user_data) {
  (void)user_data;
  *out = in_str;
  return true;
}

static tinygltf::FsCallbacks CreateDummyFsCallbacks() {
  tinygltf::FsCallbacks callbacks;
  callbacks.FileExists = FileExistsFunc;
  callbacks.ExpandFilePath = ExpandFilePathFunc;
  callbacks.ReadWholeFile = ReadWholeFileFunc;
  callbacks.WriteWholeFile = WriteWholeFileFunc;
  callbacks.GetFileSizeInBytes = GetFileSizeInBytesFunc;
  callbacks.user_data = nullptr;
  return callbacks;
}

static tinygltf::URICallbacks CreateDummyURICallbacks() {
  tinygltf::URICallbacks callbacks;
  callbacks.encode = URIEncodeFunc;
  callbacks.decode = URIDecodeFunc;
  callbacks.user_data = nullptr;
  return callbacks;
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Fuzz LoadBinaryFromMemory
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size),
                           "./dummy_file", tinygltf::REQUIRE_VERSION);

  // Fuzz SetFsCallbacks
  tinygltf::FsCallbacks fs_cb = CreateDummyFsCallbacks();
  ctx.SetFsCallbacks(fs_cb, &err);

  // Fuzz LoadBinaryFromFile (using dummy file, likely fails but tests path)
  ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file",
                         tinygltf::REQUIRE_VERSION);

  // Fuzz SetURICallbacks
  tinygltf::URICallbacks uri_cb = CreateDummyURICallbacks();
  ctx.SetURICallbacks(uri_cb, &err);

  // Fuzz LoadASCIIFromFile (using dummy file)
  ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file",
                        tinygltf::REQUIRE_VERSION);

  // Fuzz LoadASCIIFromString
  // Treat input as string data for ASCII parsing
  ctx.LoadASCIIFromString(&model, &err, &warn,
                          reinterpret_cast<const char*>(Data),
                          static_cast<unsigned int>(Size),
                          "./dummy_file", tinygltf::REQUIRE_VERSION);

  return 0;
}