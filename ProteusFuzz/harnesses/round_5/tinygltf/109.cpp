// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <fstream>
#include <functional>
#include "tiny_gltf.h"

// Dummy Filesystem Callbacks with correct signatures matching tinygltf.h
// Signature: bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data)
static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *user_data) {
  (void)out;
  (void)err;
  (void)filename;
  (void)user_data;
  return false;
}

// Signature: bool WriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data)
static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents,
                                void *user_data) {
  (void)err;
  (void)filename;
  (void)contents;
  (void)user_data;
  return false;
}

// Signature: bool GetFileSizeInBytes(size_t *size, const std::string &filename, void *user_data)
// Note: tinygltf.h defines this as: bool GetFileSizeInBytes(size_t *size, const std::string &filename, void *user_data)
// But the error suggests the expected signature might differ slightly in the std::function wrapper.
// Let's ensure the signature matches exactly what tinygltf expects.
// Based on the error: function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>
// It seems the library expects: bool(size_t*, std::string*, const std::string&, void*)
// Let's adjust the dummy function to match the likely expected signature if the header is different,
// or assume the header definition in the prompt was simplified and the actual library expects a pointer for the second string.
// However, looking at the error: "no known conversion ... to 'function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'"
// This implies the library expects: bool(size_t*, std::string*, const std::string&, void*)
// Wait, the error says: function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>
// That means: bool(size_t*, std::string*, const std::string&, void*)
// My dummy function was: bool(size_t*, const std::string&, void*)
// I need to add the std::string* parameter.
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) {
  (void)size;
  (void)err;
  (void)filename;
  (void)user_data;
  return false;
}

// Dummy URI Callbacks with correct signatures
// Error suggests: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// This implies: bool(const std::string&, const std::string&, std::string*, void*)
// Wait, the error for encode: "function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>"
// This looks like: bool(const std::string& input, const std::string& ???, std::string* out, void* user_data)
// Actually, let's look at the standard tinygltf URI callback signatures.
// Usually: bool URIDecode(std::string *out, const std::string &url, void *user_data)
// And: bool URIEncode(std::string *out, const std::string &url, void *user_data)
// The error message for encode: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// This is weird. It has two const strings and one output string.
// Maybe the signature in this specific version of tinygltf is different?
// Or maybe the error message is parsing the arguments in a confusing order.
// Let's try to match the error message exactly.
// Error: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// Arguments: (const std::string&, const std::string&, std::string*, void*)
// This doesn't match the standard tinygltf API.
// However, looking at the error for decode: function<bool (const basic_string<char> &, basic_string<char> *, void *)>
// Arguments: (const std::string&, std::string*, void*)
// This matches the standard: bool(const std::string& url, std::string* out, void* user_data)
// So decode is: bool(const std::string&, std::string*, void*)
// But encode error says: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// Maybe the first const string is the url, the second is something else? Or maybe it's a typo in the error message?
// Let's assume the standard tinygltf signatures are correct and the error message might be misleading or the library version is specific.
// Standard tinygltf:
// typedef bool (*URIDecodeFunction)(std::string *out, const std::string &url, void *user_data);
// typedef bool (*URIEncodeFunction)(std::string *out, const std::string &url, void *user_data);
// But the error for decode says: function<bool (const basic_string<char> &, basic_string<char> *, void *)>
// This is (const std::string&, std::string*, void*).
// This is different from the standard (std::string*, const std::string&, void*).
// The order of arguments is swapped!
// Let's adjust the dummy functions to match the error message's expected signature.
// Decode: bool(const std::string& url, std::string* out, void* user_data)
// Encode: bool(const std::string& url, const std::string& ???, std::string* out, void* user_data) -- This is still weird.
// Let's re-read the encode error: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// Maybe it's (const std::string& url, const std::string& encoding, std::string* out, void* user_data)?
// Or maybe the error message is just confusing.
// Let's try to match the decode signature first as it seems clearer.
// Decode: bool(const std::string&, std::string*, void*)
static bool DummyURIDecode(const std::string &url, std::string *out, void *user_data) {
  (void)url;
  (void)out;
  (void)user_data;
  return false;
}

// For encode, let's assume the second const string is an extra parameter or the error message is slightly off.
// But to be safe, let's try to match the signature exactly as seen in the error.
// function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// bool(const std::string&, const std::string&, std::string*, void*)
static bool DummyURIEncode(const std::string &url, const std::string &encoding, std::string *out, void *user_data) {
  (void)url;
  (void)encoding;
  (void)out;
  (void)user_data;
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // 1. Test LoadBinaryFromMemory
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "",
                           tinygltf::REQUIRE_VERSION);

  // 2. Test LoadASCIIFromString
  const char *str_data = reinterpret_cast<const char *>(Data);
  ctx.LoadASCIIFromString(&model, &err, &warn, str_data, Size, "",
                          tinygltf::REQUIRE_VERSION);

  // 3. Test LoadASCIIFromFile and LoadBinaryFromFile
  std::string filename = "./dummy_file";
  {
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open()) {
      ofs.write(reinterpret_cast<const char *>(Data), Size);
      ofs.close();
    }
  }

  ctx.LoadASCIIFromFile(&model, &err, &warn, filename,
                        tinygltf::REQUIRE_VERSION);

  ctx.LoadBinaryFromFile(&model, &err, &warn, filename,
                         tinygltf::REQUIRE_VERSION);

  // 4. Test SetFsCallbacks
  tinygltf::FsCallbacks fs_callbacks;
  fs_callbacks.FileExists = nullptr;
  fs_callbacks.ExpandFilePath = nullptr;
  fs_callbacks.ReadWholeFile = &DummyReadWholeFile;
  fs_callbacks.WriteWholeFile = &DummyWriteWholeFile;
  // Adjusted signature to match error: bool(size_t*, std::string*, const std::string&, void*)
  fs_callbacks.GetFileSizeInBytes = &DummyGetFileSizeInBytes;
  fs_callbacks.user_data = nullptr;

  ctx.SetFsCallbacks(fs_callbacks, &err);

  // 5. Test SetURICallbacks
  tinygltf::URICallbacks uri_callbacks;
  // Adjusted signatures to match error messages
  // Encode: bool(const std::string&, const std::string&, std::string*, void*)
  uri_callbacks.encode = &DummyURIEncode;
  // Decode: bool(const std::string&, std::string*, void*)
  uri_callbacks.decode = &DummyURIDecode;
  uri_callbacks.user_data = nullptr;

  ctx.SetURICallbacks(uri_callbacks, &err);

  // 6. Explore more states with different check_sections flags
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "",
                           tinygltf::REQUIRE_VERSION);
  ctx.LoadASCIIFromString(&model, &err, &warn, str_data, Size, "",
                          tinygltf::REQUIRE_VERSION);

  return 0;
}