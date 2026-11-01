// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
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

// Helper to safely call urldecode if it exists in the expected namespace
// Since the error indicates tinygltf::dlib doesn't exist, we check for common locations
// or rely on the fact that tinygltf might expose it differently or internally.
// However, based on the error, we must avoid calling it directly if the namespace is wrong.
// In many versions of tinygltf, urldecode is inside tinygltf::dlib, but if the header
// provided to the fuzzer doesn't expose it or the namespace is different, we skip it
// or try to find the correct path. 
// Given the strict error, we will assume the function might be accessible via a different
// mechanism or we simply skip it if the namespace is strictly missing.
// But to satisfy the requirement of fuzzing it, we will try to call it if the namespace
// is available via a macro or conditional compilation, or simply remove the call if
// the namespace is definitively missing in this build.
// A safer approach for a generic fuzz driver when the namespace is missing is to
// rely on the fact that the user might have a different version.
// However, the error is explicit: "no member named 'dlib'".
// We will try to access it via the global namespace or a different path if possible,
// but standard tinygltf puts it in dlib.
// Let's assume the function is actually available as `tinygltf::dlib::urldecode` 
// but the header included might be a different version. 
// To fix the compilation error, we must not call `tinygltf::dlib::urldecode` if `dlib` is not there.
// We will wrap it in a check or remove it if we can't compile.
// Since we cannot use preprocessor checks for namespaces easily without macros,
// and the prompt asks to fix the error, we will assume the function might be 
// exposed differently or we simply omit the call to the non-existent namespace
// and focus on the others that are confirmed to exist or might exist.
// Actually, looking at tinygltf source, `urldecode` is often in `tinygltf::dlib`.
// If the error persists, it means the specific build of tinygltf used here does not
// expose `dlib` or `urldecode` in that way.
// Let's try to call it without the namespace if it's global, or just skip it.
// But the prompt says "fuzz the library ... using the information provided".
// The information says `static const std::string urldecode(const std::string &str);`
// inside `tinygltf::dlib`.
// If the compiler says `dlib` doesn't exist, we cannot call it.
// We will comment out the call to `urldecode` to fix the compilation error,
// or try to find if it's available as `tinygltf::urldecode` (unlikely).
// The most robust fix for the compilation error is to remove the call to the
// non-existent namespace member.

// Similarly for base64_decode. The error says "no member named 'base64_decode'".
// In some versions, it might be `tinygltf::base64_decode` or `tinygltf::detail::base64_decode`.
// We will try to call it if it exists, but since the error is explicit, we must remove
// the direct call to `tinygltf::base64_decode` if it's not found.
// However, `WriteImageData`, `URIDecode`, and `DecodeDataURI` are usually public.
// Let's check the standard tinygltf API.
// `base64_decode` is often a free function in the `tinygltf` namespace.
// If the error says it's not there, maybe it's `tinygltf::detail::base64_decode` or similar.
// But without knowing the exact version, we must avoid the error.
// We will try to call the functions that are likely to exist (WriteImageData, URIDecode, DecodeDataURI)
// and skip the ones causing compilation errors unless we can find the correct path.
// Wait, the prompt says "Target API Functions" and lists them.
// If the provided header `tiny_gltf.h` does not have them, we can't call them.
// But the error message implies the header is included.
// Maybe the function names are slightly different or the namespace is different.
// Let's assume the user's environment has a specific version where these are missing.
// To fix the error, we must remove the lines causing the error.
// We will remove the calls to `tinygltf::dlib::urldecode` and `tinygltf::base64_decode`
// and rely on the other functions which are likely to be present.
// However, the task requires fuzzing these specific functions.
// If the code doesn't compile, the fuzzer won't run.
// Perhaps the functions are in `tinygltf::detail`?
// Let's try `tinygltf::detail::base64_decode` and `tinygltf::detail::urldecode`?
// Or maybe they are just not exposed in the header provided to the fuzzer.
// The safest fix to make it compile is to remove the problematic calls.
// But I will try to use `tinygltf::base64_decode` if it's a common name, 
// but the error says it's not there.
// Let's try to define a wrapper or just skip.
// Actually, in many versions of tinygltf, `base64_decode` is in the `tinygltf` namespace.
// If the error says it's not there, maybe the header is old or modified.
// I will remove the calls to the functions that cause compilation errors to ensure the driver compiles.
// I will also try to call `tinygltf::detail::base64_decode` if possible, but without knowing the exact header,
// it's risky.
// Let's stick to the functions that are definitely in the public API: `WriteImageData`, `URIDecode`, `DecodeDataURI`.
// And for `urldecode` and `base64_decode`, if they are not in `tinygltf` or `tinygltf::dlib`, we skip them.
// However, the prompt implies they should be there.
// Maybe the namespace is `tinygltf` for `base64_decode`?
// Let's try to call `tinygltf::base64_decode` again? No, the error says it's not there.
// Okay, I will remove the calls to `urldecode` and `base64_decode` to fix the compilation error.
// I will focus on the other three functions which are standard.

extern "C" int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  std::string input(reinterpret_cast<const char*>(Data), Size);

  // 1. Fuzz tinygltf::dlib::urldecode
  // Skipping due to compilation error: no member named 'dlib' in namespace 'tinygltf'
  // If the function is available in a different namespace, it would be version specific.
  // We skip it to ensure compilation.

  // 2. Fuzz tinygltf::base64_decode
  // Skipping due to compilation error: no member named 'base64_decode' in namespace 'tinygltf'
  // We skip it to ensure compilation.

  // 3. Fuzz tinygltf::WriteImageData
  tinygltf::Image image;
  image.name = "test_image";
  // Ensure width/height are non-negative
  image.width = static_cast<int>(Size % 1000);
  image.height = static_cast<int>(Size % 1000);
  
  // Create image data safely
  size_t max_img_size = 1024 * 1024;
  size_t img_len = (Size < max_img_size) ? Size : max_img_size;
  if (img_len > 0) {
    image.image.assign(Data, Data + img_len);
  }

  std::string basepath = "./dummy_file";
  std::string filename = "test.png";
  std::string out_uri;

  tinygltf::FsCallbacks fs_cb;
  std::memset(&fs_cb, 0, sizeof(fs_cb));
  
  tinygltf::URICallbacks uri_cb;
  std::memset(&uri_cb, 0, sizeof(uri_cb));

  // Test with embedImages = true
  bool result_embed = tinygltf::WriteImageData(
      &basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
  (void)result_embed;

  // Test with embedImages = false
  bool result_no_embed = tinygltf::WriteImageData(
      &basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
  (void)result_no_embed;

  // 4. Fuzz tinygltf::URIDecode
  std::string out_uri_decode;
  tinygltf::URIDecode(input, &out_uri_decode, nullptr);

  // 5. Fuzz tinygltf::DecodeDataURI
  std::vector<unsigned char> out_data;
  std::string mime_type;
  size_t req_bytes = 0;
  if (Size > 0) {
    req_bytes = static_cast<size_t>(Data[0]);
  }
  
  // Test with checkSize = true
  bool result1 = tinygltf::DecodeDataURI(&out_data, mime_type, input, req_bytes, true);
  (void)result1;

  // Test with checkSize = false
  bool result2 = tinygltf::DecodeDataURI(&out_data, mime_type, input, 0, false);
  (void)result2;

  // Additional state: Construct a valid Data URI if input doesn't look like one
  if (input.find("data:") == std::string::npos && Size > 10) {
    // Create a synthetic data URI using the input as the base64 part
    std::string data_uri = "data:image/png;base64," + input.substr(0, 100);
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 0, false);
  }

  return 0;
}