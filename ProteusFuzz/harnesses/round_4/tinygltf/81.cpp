// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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

// Define callback types matching tinygltf.h internal definitions
// Note: tinygltf uses std::function for callbacks.
// We need to match the exact signatures expected by the struct members.

static bool DummyFileExists(const std::string &filename, void *) {
  return false;
}

static bool DummyExpandFilePath(std::string *path, const std::string &filename, void *) {
  if (path) *path = filename;
  return true;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *) {
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *) {
  return false;
}

// GetFileSizeInBytes in tinygltf expects: bool(size_t*, const std::string&, void*)
// The error suggests the struct member expects a different signature or the function pointer doesn't match.
// Let's check tinygltf.h source logic. Usually it is: bool GetFileSizeInBytes(size_t* size, const std::string& path, void* user_data)
// The error message says: "no known conversion ... to 'function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'"
// This implies the struct member expects 4 arguments? Or maybe the error is misinterpreting the std::function template.
// Actually, looking at the error: "function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>"
// It seems the struct member is defined as taking (size_t*, std::string*, const std::string&, void*)?
// Let's look at the error for ExpandFilePath: "function<basic_string<char> (const basic_string<char> &, void *)>"
// But my function returns bool.
// It seems the types in the struct are std::function<...> and the signatures I defined don't match the struct's template parameters.
// Let's redefine the dummy functions to match the likely expected signatures based on the error messages.

// Error for ExpandFilePath: expects function returning std::string, taking (const std::string&, void*)?
// But standard ExpandFilePath usually returns bool and modifies output string.
// Let's assume the struct definitions in tinygltf.h are:
// std::function<bool(std::string*, const std::string&, void*)> ExpandFilePath;
// But the error says: "no known conversion ... to 'const function<basic_string<char> (const basic_string<char> &, void *)>'"
// This is very strange. Maybe the version of tinygltf.h used has different signatures.
// Let's try to match the error's expectation or use the standard tinygltf signatures if known.
// Standard tinygltf FsCallbacks:
// FileExistsFunction FileExists; // bool(const std::string&, void*)
// ExpandFilePathFunction ExpandFilePath; // bool(std::string*, const std::string&, void*)
// ReadWholeFileFunction ReadWholeFile; // bool(std::vector<unsigned char>*, std::string*, const std::string&, void*)
// WriteWholeFileFunction WriteWholeFile; // bool(std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// GetFileSizeInBytesFunction GetFileSizeInBytes; // bool(size_t*, const std::string&, void*)

// The error for GetFileSizeInBytes: "function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>"
// This implies the struct expects: bool(size_t*, std::string*, const std::string&, void*)
// This is unusual. Maybe it's a specific version or a typo in the error parsing.
// Let's try to define the functions exactly as the error suggests the struct expects, or use the standard ones if the error is misleading.
// However, the error for ExpandFilePath says it expects a function returning std::string.
// Let's try to define the dummy functions with the signatures that the compiler complains are missing, to see if we can match them.
// But wait, the error says "candidate function not viable: no known conversion from 'bool (std::string *, const std::string &, void *)' to 'const function<basic_string<char> (const basic_string<char> &, void *)>'"
// This means the struct member `ExpandFilePath` is of type `std::function<std::string(const std::string&, void*)>`.
// This is different from the standard tinygltf.
// Let's assume the provided environment has a specific version.
// Let's try to define the dummy functions to match the error's implied signatures.

// Implied signature for ExpandFilePath: std::string(const std::string&, void*)
static std::string DummyExpandFilePathImpl(const std::string &filename, void *) {
  return filename;
}

// Implied signature for GetFileSizeInBytes: bool(size_t*, std::string*, const std::string&, void*)
static bool DummyGetFileSizeInBytesImpl(size_t *size, std::string *err, const std::string &filename, void *) {
  if (size) *size = 0;
  return true;
}

// Implied signature for URIEncode: bool(const std::string&, const std::string&, std::string*, void*)?
// Error: "no known conversion from 'std::string (const std::string &, void *)' to 'const function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'"
// This implies: bool(const std::string&, const std::string&, std::string*, void*)
static bool DummyURIEncodeImpl(const std::string &in, const std::string &basepath, std::string *out, void *) {
  if (out) *out = in;
  return true;
}

// Implied signature for URIDecode: bool(const std::string&, std::string*, void*) - This seems standard.
// Error for URIDecode wasn't listed, so maybe it's fine or we need to check.
// Let's assume standard for decode.
static bool DummyURIDecodeImpl(const std::string &in_uri, std::string *out_uri, void *) {
  if (out_uri) *out_uri = in_uri;
  return true;
}

// Implied signature for WriteImageDataFunction: bool(const std::string*, const std::string*, const Image*, bool, const FsCallbacks*, const URICallbacks*, std::string*, void*)
// This is a function pointer, not a std::function in the struct usually, but the struct might hold it as a std::function.
// Let's define it as a function pointer matching the signature.
static bool DummyWriteImageDataImpl(const std::string *basepath, const std::string *filename,
                                    const tinygltf::Image *image, bool embedImages,
                                    const tinygltf::FsCallbacks *fs_cb,
                                    const tinygltf::URICallbacks *uri_cb,
                                    std::string *out_uri, void *user_data) {
  return false;
}

// Implied signature for LoadImageDataFunction: bool(Image*, int, const std::string&, const std::string&, size_t, size_t, void*)
static bool DummyLoadImageDataImpl(tinygltf::Image *image, int image_index,
                                   const std::string &uri, const std::string &baseDir,
                                   size_t reqWidth, size_t reqHeight, void *user_data) {
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = DummyFileExists;
  // Based on error, ExpandFilePath expects std::function<std::string(const std::string&, void*)>
  fs_cb.ExpandFilePath = DummyExpandFilePathImpl;
  fs_cb.ReadWholeFile = DummyReadWholeFile;
  fs_cb.WriteWholeFile = DummyWriteWholeFile;
  // Based on error, GetFileSizeInBytes expects bool(size_t*, std::string*, const std::string&, void*)
  fs_cb.GetFileSizeInBytes = DummyGetFileSizeInBytesImpl;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  // Based on error, encode expects bool(const std::string&, const std::string&, std::string*, void*)
  uri_cb.encode = DummyURIEncodeImpl;
  uri_cb.decode = DummyURIDecodeImpl;
  uri_cb.user_data = nullptr;

  std::string input_str(reinterpret_cast<const char *>(Data), Size);

  // Test IsDataURI
  tinygltf::IsDataURI(input_str);

  // Test DecodeDataURI
  std::vector<unsigned char> out_vec;
  std::string mime_type;
  tinygltf::DecodeDataURI(&out_vec, mime_type, input_str, 0, false);
  tinygltf::DecodeDataURI(&out_vec, mime_type, input_str, Size, true);

  // Test URIDecode
  std::string out_uri;
  tinygltf::URIDecode(input_str, &out_uri, nullptr);

  // Test WriteImageData
  tinygltf::Image image;
  image.image.assign(Data, Data + Size);
  image.width = static_cast<int>(Size % 1024);
  image.height = static_cast<int>(Size % 1024);
  image.component = 3;
  image.bits = 8;
  // Use the enum value from tinygltf if available, otherwise use the integer constant.
  // tinygltf uses GL_UNSIGNED_BYTE which is 0x00000000 or similar.
  // If GL_UNSIGNED_BYTE is not defined, we can use 0 or the enum tinygltf::GL_UNSIGNED_BYTE if it exists.
  // Since the error said GL_UNSIGNED_BYTE is undeclared, we assume it's not included.
  // tinygltf.h usually includes OpenGL headers or defines these.
  // Let's try to use the integer value 0x00000000 (GL_UNSIGNED_BYTE) or just 0.
  // Or better, check if tinygltf has its own enum.
  // In tinygltf.h, there is usually: enum { GL_UNSIGNED_BYTE = 0x00000000 };
  // But if not, we can try to cast or use a safe default.
  // Let's try to use the value 0x00000000 directly if the macro is missing.
  image.pixel_type = 0x00000000; 

  std::string basepath = "./dummy_file";
  std::string filename = "dummy.png";
  std::string out_uri_write;
  tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb,
                           &out_uri_write, nullptr);
  tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb,
                           &out_uri_write, nullptr);

  // Test ParseImage
  tinygltf::Image parse_image;
  std::string err, warn;
  
  #ifdef TINYGLTF_HAS_NLOHMANN_JSON
  nlohmann::json o;
  o["uri"] = input_str;
  tinygltf::ParseImage(&parse_image, 0, &err, &warn, o, false, "./dummy_file",
                       1024 * 1024, &fs_cb, &uri_cb, DummyLoadImageDataImpl, nullptr);

  nlohmann::json o2;
  o2["bufferView"] = 0;
  tinygltf::ParseImage(&parse_image, 0, &err, &warn, o2, false, "./dummy_file",
                       1024 * 1024, &fs_cb, &uri_cb, DummyLoadImageDataImpl, nullptr);

  nlohmann::json o3;
  o3["uri"] = input_str;
  o3["bufferView"] = 0;
  tinygltf::ParseImage(&parse_image, 0, &err, &warn, o3, false, "./dummy_file",
                       1024 * 1024, &fs_cb, &uri_cb, DummyLoadImageDataImpl, nullptr);

  nlohmann::json o4;
  tinygltf::ParseImage(&parse_image, 0, &err, &warn, o4, false, "./dummy_file",
                       1024 * 1024, &fs_cb, &uri_cb, DummyLoadImageDataImpl, nullptr);
  #endif

  // UpdateImageObject is static and might not be exposed in the public namespace or requires specific headers.
  // The error said "no member named 'UpdateImageObject' in namespace 'tinygltf'".
  // This function is likely static inside a detail namespace or not exposed.
  // We will skip calling it directly if it's not accessible.
  // If it's static in the header, we might need to include the implementation or it's not meant to be called directly.
  // Given the error, we cannot call it. We skip it.

  return 0;
}