// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"
#include <fstream>

// Dummy callback implementations for FsCallbacks
// Based on the error messages, the expected signatures are:
// FileExists: bool(const std::string &, void *)
// ExpandFilePath: std::string(const std::string &, void *)
// ReadWholeFile: bool(std::vector<unsigned char> *, std::string *, const std::string &, void *)
// WriteWholeFile: bool(std::string *, const std::string &, const std::vector<unsigned char> &, void *)
// GetFileSizeInBytes: bool(uint64_t *, std::string *, const std::string &, void *)
// Note: The error for GetFileSizeInBytes shows it expects: bool(unsigned long *, basic_string<char> *, const basic_string<char> &, void *)
// This means it takes a pointer to write the size, a pointer to write error, filename, and user_data.

static bool DummyFileExists(const std::string &filename, void *user_data) {
  (void)filename;
  (void)user_data;
  return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *user_data) {
  (void)filepath;
  (void)user_data;
  return filepath;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *user_data) {
  (void)out;
  (void)err;
  (void)filename;
  (void)user_data;
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *user_data) {
  (void)err;
  (void)filename;
  (void)contents;
  (void)user_data;
  return false;
}

// Corrected signature based on error: bool(uint64_t *, std::string *, const std::string &, void *)
static bool DummyGetFileSizeInBytes(uint64_t *size, std::string *err, const std::string &filename, void *user_data) {
  (void)size;
  (void)err;
  (void)filename;
  (void)user_data;
  return false;
}

// Dummy callback implementations for URICallbacks
// Based on error: encode expects bool(const string &, const string &, string *, void *)
// This is unusual. Let's re-read the error carefully.
// "no known conversion from 'std::string (*)(const std::string &, void *)' to 'const function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'"
// This implies the library expects: bool(const std::string &, const std::string &, std::string *, void *)
// This looks like a specific version of tinygltf or a misunderstanding of the types.
// However, standard tinygltf URIEncodeFunction is: std::string(const std::string &, void *)
// Let's assume the error message is correct for this specific build and match it.
// But wait, the error for decode was: "no viable overloaded '=' ... to 'const function<bool (const basic_string<char> &, basic_string<char> *, void *)>'"
// This matches the standard decode signature: bool(const string &, string *, void *).
// The encode error is the confusing one.
// Let's look at the tinygltf source code if possible, or assume the error message is right.
// If the error says `bool(const string &, const string &, string *, void *)`, maybe it's:
// bool(const std::string &uri, const std::string &original, std::string *out, void *user_data)?
// Or maybe the error message is garbled.
// Let's try to match the standard signature first, but the error explicitly says the target type is `function<bool (const string &, const string &, string *, void *)>`.
// This is very specific. Let's define the dummy function to match this exact signature.
// Signature: bool(const std::string &, const std::string &, std::string *, void *)
static bool DummyURIEncode(const std::string &uri, const std::string &original, std::string *out, void *user_data) {
  (void)uri;
  (void)original;
  (void)out;
  (void)user_data;
  return true;
}

// Decode signature from error: bool(const string &, string *, void *)
static bool DummyURIDecode(const std::string &uri, std::string *out, void *user_data) {
  (void)uri;
  (void)out;
  (void)user_data;
  if (out) *out = uri;
  return true;
}

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
  std::ofstream file(filename, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char *>(data), size);
    file.close();
  }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Fuzz SetURICallbacks
  tinygltf::URICallbacks uri_callbacks;
  // Match the signature inferred from the error: bool(const string &, const string &, string *, void *)
  uri_callbacks.encode = &DummyURIEncode;
  uri_callbacks.decode = &DummyURIDecode;
  uri_callbacks.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks, &err);

  // Invalid case (null pointers)
  tinygltf::URICallbacks uri_callbacks_invalid;
  uri_callbacks_invalid.encode = nullptr;
  uri_callbacks_invalid.decode = nullptr;
  uri_callbacks_invalid.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks_invalid, &err);

  // Fuzz SetFsCallbacks
  tinygltf::FsCallbacks fs_callbacks;
  fs_callbacks.FileExists = &DummyFileExists;
  fs_callbacks.ExpandFilePath = &DummyExpandFilePath;
  fs_callbacks.ReadWholeFile = &DummyReadWholeFile;
  fs_callbacks.WriteWholeFile = &DummyWriteWholeFile;
  // Match the signature inferred from the error: bool(uint64_t *, std::string *, const std::string &, void *)
  fs_callbacks.GetFileSizeInBytes = &DummyGetFileSizeInBytes;
  fs_callbacks.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks, &err);

  // Invalid FsCallbacks (null pointers)
  tinygltf::FsCallbacks fs_callbacks_invalid;
  fs_callbacks_invalid.FileExists = nullptr;
  fs_callbacks_invalid.ExpandFilePath = nullptr;
  fs_callbacks_invalid.ReadWholeFile = nullptr;
  fs_callbacks_invalid.WriteWholeFile = nullptr;
  fs_callbacks_invalid.GetFileSizeInBytes = nullptr;
  fs_callbacks_invalid.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks_invalid, &err);

  // Fuzz LoadBinaryFromMemory
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 1);
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 2);
  // Edge case: empty size
  ctx.LoadBinaryFromMemory(&model, &err, &warn, nullptr, 0, "", 0);

  // Fuzz LoadASCIIFromString
  ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char *>(Data), Size, "", 0);
  ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char *>(Data), Size, "", 1);
  ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char *>(Data), Size, "", 2);
  ctx.LoadASCIIFromString(&model, &err, &warn, "", 0, "", 0);

  // Fuzz LoadBinaryFromFile and LoadASCIIFromFile
  const std::string dummy_filename = "./dummy_file";
  WriteDummyFile(dummy_filename, Data, Size);

  ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 0);
  ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 1);
  ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 2);

  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 0);
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 1);
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 2);

  // Cleanup
  std::remove(dummy_filename.c_str());

  return 0;
}