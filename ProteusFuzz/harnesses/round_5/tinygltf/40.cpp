// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
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

// Callbacks matching tinygltf expected signatures
// Note: Signatures must match exactly what tiny_gltf.h expects.
// Based on common tinygltf implementations:
// WriteWholeFile: bool WriteWholeFile(const std::string &filename, const std::vector<unsigned char> *contents, void *user_data)
// However, the error suggests the library expects:
// bool WriteWholeFile(std::string *filename, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data)
// Or similar. Let's look at the error again.
// Error: 'no known conversion from 'bool (const std::string &, const std::vector<unsigned char> *, void *)' to 'const function<bool (basic_string<char> *, const basic_string<char> &, const vector<unsigned char> &, void *)>'
// This implies the expected signature is: bool(std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// This is unusual. Let's assume the library defines WriteWholeFileFunction as:
// typedef bool (*WriteWholeFileFunction)(const std::string &filename, const std::vector<unsigned char> *contents, void *user_data);
// Wait, the error message says the target type is: function<bool (basic_string<char> *, const basic_string<char> &, const vector<unsigned char> &, void *)>
// This looks like a specific version of tinygltf or a mismatch in my assumption.
// Let's re-read the error carefully.
// The error says: 'no known conversion from 'bool (const std::string &, const std::vector<unsigned char> *, void *)' ... to 'function<bool (basic_string<char> *, const basic_string<char> &, const vector<unsigned char> &, void *)>'
// This means the struct FsCallbacks expects a function with signature:
// bool (std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// This is very strange for a write function. Usually it's (const std::string&, const std::vector<unsigned char>*, void*).
// Perhaps the library version used here has a different signature.
// Let's try to match the signature in the error message exactly.
// Target: bool (std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// My function: bool (const std::string&, const std::vector<unsigned char>*, void*)
// The error suggests the library expects a pointer to std::string as the first argument.
// Let's adjust the callback to match the error's expected signature.

static bool FileExistsCallback(const std::string &filename, void *user_data) {
  (void)user_data;
  return (filename == "./dummy_file");
}

static std::string ExpandFilePathCallback(const std::string &filepath,
                                          void *user_data) {
  (void)user_data;
  return filepath;
}

static bool ReadWholeFileCallback(std::vector<unsigned char> *out,
                                  std::string *err,
                                  const std::string &filename,
                                  void *user_data) {
  (void)user_data;
  if (filename == "./dummy_file") {
    out->resize(12);
    std::memcpy(out->data(), "glTF", 4);
    out->data()[4] = 2; 
    out->data()[5] = 0;
    out->data()[6] = 0;
    out->data()[7] = 0;
    out->data()[8] = 12; 
    out->data()[9] = 0;
    out->data()[10] = 0;
    out->data()[11] = 0;
    return true;
  }
  if (err) *err = "File not found: " + filename;
  return false;
}

// Adjusted signature to match the error: bool(std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// This is highly unusual, but we must match the compiler's expectation.
// Wait, looking at the error again:
// 'no known conversion from 'bool (const std::string &, const std::vector<unsigned char> *, void *)' ... to 'function<bool (basic_string<char> *, const basic_string<char> &, const vector<unsigned char> &, void *)>'
// The target type is: function<bool (std::string*, const std::string&, const std::vector<unsigned char>&, void*)>
// This implies the first argument is a pointer to string (maybe for error output?), second is filename, third is contents, fourth is user_data.
// Let's define the callback to match this.
static bool WriteWholeFileCallback(std::string *err,
                                   const std::string &filename,
                                   const std::vector<unsigned char> &contents,
                                   void *user_data) {
  (void)err;
  (void)filename;
  (void)contents;
  (void)user_data;
  return true;
}

static bool GetFileSizeInBytesCallback(size_t *size,
                                       std::string *err,
                                       const std::string &filename,
                                       void *user_data) {
  (void)err;
  (void)user_data;
  if (filename == "./dummy_file") {
    *size = 12;
    return true;
  }
  return false;
}

// URI Callbacks
// Error for URIEncodeCallback:
// 'no known conversion from 'std::string (const std::string &, void *)' ... to 'function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'
// Target: function<bool (const std::string&, const std::string&, std::string*, void*)>
// This implies the signature is: bool(const std::string& input, const std::string& something, std::string* output, void* user_data)
// This is also unusual. Standard URI encode usually takes (const std::string&, void*) -> std::string.
// Let's adjust to match the error's expected signature.
// The error says: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// So: bool(const std::string&, const std::string&, std::string*, void*)
static bool URIEncodeCallback(const std::string &input,
                              const std::string &unused,
                              std::string *output,
                              void *user_data) {
  (void)unused;
  (void)user_data;
  if (output) *output = input; // Simple passthrough
  return true;
}

// Error for URIDecodeCallback:
// 'no known conversion from 'std::string (const std::string &, void *)' ... to 'function<bool (const basic_string<char> &, basic_string<char> *, void *)>'
// Target: function<bool (const std::string&, std::string*, void*)>
// So: bool(const std::string& input, std::string* output, void* user_data)
static bool URIDecodeCallback(const std::string &input,
                              std::string *output,
                              void *user_data) {
  (void)user_data;
  if (output) *output = input; // Simple passthrough
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Write fuzz data to dummy file to test file loading
  std::ofstream dummyFile("./dummy_file", std::ios::binary);
  if (dummyFile.is_open()) {
    dummyFile.write(reinterpret_cast<const char*>(Data), Size);
    dummyFile.close();
  }

  tinygltf::TinyGLTF loader;

  // 1. LoadASCIIFromFile
  loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file",
                           tinygltf::REQUIRE_VERSION);
  loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file",
                           tinygltf::REQUIRE_VERSION);
  loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file",
                           tinygltf::REQUIRE_VERSION);

  // 2. LoadBinaryFromFile
  loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file",
                            tinygltf::REQUIRE_VERSION);
  loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file",
                            tinygltf::REQUIRE_VERSION);

  // 3. LoadBinaryFromMemory
  loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, ".",
                              tinygltf::REQUIRE_VERSION);
  loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, ".",
                              tinygltf::REQUIRE_VERSION);

  // 4. LoadASCIIFromString
  loader.LoadASCIIFromString(&model, &err, &warn,
                             reinterpret_cast<const char*>(Data), Size, ".",
                             tinygltf::REQUIRE_VERSION);
  loader.LoadASCIIFromString(&model, &err, &warn,
                             reinterpret_cast<const char*>(Data), Size, ".",
                             tinygltf::REQUIRE_VERSION);

  // 5. SetFsCallbacks
  tinygltf::FsCallbacks fsCallbacks;
  fsCallbacks.FileExists = FileExistsCallback;
  fsCallbacks.ExpandFilePath = ExpandFilePathCallback;
  fsCallbacks.ReadWholeFile = ReadWholeFileCallback;
  // Adjusted signature to match the library's expectation based on compiler error
  fsCallbacks.WriteWholeFile = WriteWholeFileCallback;
  fsCallbacks.GetFileSizeInBytes = GetFileSizeInBytesCallback;
  fsCallbacks.user_data = nullptr;

  loader.SetFsCallbacks(fsCallbacks, &err);

  // 6. SetURICallbacks
  tinygltf::URICallbacks uriCallbacks;
  // Adjusted signatures to match the library's expectation based on compiler error
  uriCallbacks.encode = URIEncodeCallback;
  uriCallbacks.decode = URIDecodeCallback;
  uriCallbacks.user_data = nullptr;

  loader.SetURICallbacks(uriCallbacks, &err);

  // Test with invalid callbacks (null pointers) to trigger error paths
  tinygltf::URICallbacks invalidUriCallbacks;
  invalidUriCallbacks.encode = nullptr;
  invalidUriCallbacks.decode = nullptr;
  invalidUriCallbacks.user_data = nullptr;
  loader.SetURICallbacks(invalidUriCallbacks, &err);

  tinygltf::FsCallbacks invalidFsCallbacks;
  invalidFsCallbacks.FileExists = nullptr;
  invalidFsCallbacks.ExpandFilePath = nullptr;
  invalidFsCallbacks.ReadWholeFile = nullptr;
  invalidFsCallbacks.WriteWholeFile = nullptr;
  invalidFsCallbacks.GetFileSizeInBytes = nullptr;
  invalidFsCallbacks.user_data = nullptr;
  loader.SetFsCallbacks(invalidFsCallbacks, &err);

  return 0;
}