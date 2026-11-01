// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ParseStringProperty at tiny_gltf.h:4060:13 in tiny_gltf.h
// tinygltf::ParseStringProperty at tiny_gltf.h:4060:13 in tiny_gltf.h
// tinygltf::ParseUnsignedProperty at tiny_gltf.h:3858:13 in tiny_gltf.h
// tinygltf::ParseUnsignedProperty at tiny_gltf.h:3858:13 in tiny_gltf.h
// tinygltf::ParseNumberProperty at tiny_gltf.h:3912:13 in tiny_gltf.h
// tinygltf::ParseNumberProperty at tiny_gltf.h:3912:13 in tiny_gltf.h
// tinygltf::ParseSampler at tiny_gltf.h:5614:13 in tiny_gltf.h
// tinygltf::ParseSampler at tiny_gltf.h:5614:13 in tiny_gltf.h
// tinygltf::detail::GetString at tiny_gltf.h:3502:6 in tiny_gltf.h
// tinygltf::ParseTexture at tiny_gltf.h:4439:13 in tiny_gltf.h
// tinygltf::ParseTexture at tiny_gltf.h:4439:13 in tiny_gltf.h
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

// tinygltf uses a different JSON library depending on configuration (RapidJSON, nlohmann, etc.)
// The 'detail' namespace and 'json' type are internal. We must use the public API or
// construct the internal JSON type correctly based on the library's configuration.
// Since we cannot easily know which JSON backend is active without checking macros,
// and the target functions expect `const detail::json &o`, we need to construct a valid
// JSON object that matches the internal type.
//
// However, the target functions are `static` inside `tinygltf` (or `tinygltf::detail`),
// and they expect `detail::json`. Accessing `detail::json` directly is often restricted
// or requires specific includes.
//
// A robust way to fuzz these specific internal parsers is to create a valid `tinygltf::Model`
// or use the `tinygltf::Model` loading mechanism which internally uses these functions,
// but the task specifically asks to fuzz the API functions directly.
//
// Let's try to access the internal JSON type. In many builds of tinygltf, `detail::json`
// is an alias or a class within the `tinygltf::detail` namespace.
// If the build uses nlohmann_json, `tinygltf::detail::json` is `nlohmann::json`.
// If it uses RapidJSON, it's a wrapper.
//
// Since the error says "no member named 'detail' in namespace 'tinygltf'", it implies
// that the `detail` namespace is not exposed or the header `tiny_gltf.h` does not
// expose `tinygltf::detail::json` in this specific build configuration, or the user
// needs to include a specific internal header or use a different approach.
//
// Alternative approach: Use the public `tinygltf::Model` loading or `tinygltf::LoadFromString`
// which internally calls these functions. However, the prompt asks to fuzz the specific
// API functions.
//
// Let's assume the standard tinygltf structure where `detail` is a namespace inside `tinygltf`.
// The error suggests the compiler cannot find `tinygltf::detail`.
// This often happens if `TINYGLTF_IMPLEMENTATION` is not defined correctly or if the
// JSON backend is not selected.
//
// Let's try to define the implementation macro if not defined, and ensure we are using
// the correct type. If `tinygltf::detail::json` is truly inaccessible, we might need to
// use the `nlohmann::json` directly if that's the backend, or `rapidjson`.
//
// However, the safest bet for a fuzz driver that must compile against `tiny_gltf.h`
// without knowing the exact backend is to use the `tinygltf::Model` loading path
// OR, if we must call the functions, we need to ensure the internal types are visible.
//
// Let's try to include the internal definition by defining TINYGLTF_IMPLEMENTATION
// before including the header, which is standard for single-header libraries.
// But the user already included "tiny_gltf.h".
//
// Let's try to use the `tinygltf::detail::json` type by assuming the standard layout.
// If the error persists, it might be that the `detail` namespace is not public.
//
// Wait, the error is "no member named 'detail' in namespace 'tinygltf'".
// This usually means the `tinygltf` namespace does not have a `detail` child.
// In some versions/configurations, the JSON type is exposed differently.
//
// Let's try a different approach: We will create a dummy file and use the public
// `LoadASCIIFromString` or similar to trigger the parsing, but the prompt asks
// to fuzz the specific functions.
//
// If we cannot access `tinygltf::detail::json`, we cannot call these functions directly
// because their signature requires it.
//
// Hypothesis: The `tinygltf` header might be included in a way that hides `detail`.
// Or, the `detail` namespace is actually `tinygltf::detail` but the user's build
// configuration is different.
//
// Let's try to define `TINYGLTF_IMPLEMENTATION` to ensure the implementation is visible.
// Also, we need to check if `tinygltf::detail` is the correct path.
//
// Actually, looking at the provided code snippets in the prompt:
// `static bool ParseStringProperty(..., const detail::json &o, ...)`
// This implies the function is inside a namespace (likely `tinygltf` or `tinygltf::detail`)
// and uses `detail::json`.
//
// If the function is `tinygltf::ParseStringProperty`, then `detail::json` inside it
// likely refers to `tinygltf::detail::json`.
//
// Let's try to force the inclusion of the implementation and access the type.
// If `tinygltf::detail` is not visible, we might need to use `nlohmann::json` directly
// if that's the backend, but we don't know the backend.
//
// However, the prompt says "You need to include `tiny_gltf.h` to invoke functions above."
// This implies the functions are accessible.
//
// Let's try to define `TINYGLTF_IMPLEMENTATION` before the include.
// And we will assume `tinygltf::detail::json` is the correct type.
// If the error persists, it might be a specific build issue.
//
// Let's try to use `tinygltf::detail::json` but with a fallback or by ensuring
// the namespace is correct.
//
// Actually, in many single-header libraries, the `detail` namespace is defined inside
// the main namespace.
//
// Let's try to define `TINYGLTF_IMPLEMENTATION` and re-include.
// But we can't re-include in the same file easily without guards.
//
// Let's assume the user's environment has `tiny_gltf.h` set up correctly for the
// `detail` namespace, but maybe the macro `TINYGLTF_IMPLEMENTATION` was missing.
// We will define it.

#ifndef TINYGLTF_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#endif

#include "tiny_gltf.h"

// Helper to create a tinygltf::detail::json object from raw input data
static tinygltf::detail::json CreateJsonFromInput(const uint8_t *Data, size_t Size) {
  tinygltf::detail::json json_obj;
  
  if (Size > 0) {
    std::string json_str(reinterpret_cast<const char*>(Data), Size);
    try {
      // Attempt to parse. The method name depends on the JSON backend.
      // If nlohmann, it's json::parse. If RapidJSON, it's different.
      // Since we don't know, we try the common nlohmann approach first.
      // If this fails to compile, we might need to use a different strategy.
      // But the prompt implies we should be able to use these types.
      // Let's assume nlohmann_json is the backend as it's common and the error
      // message in the prompt showed nlohmann types.
      json_obj = tinygltf::detail::json::parse(json_str);
    } catch (...) {
      json_obj = tinygltf::detail::json::object();
    }
  } else {
    json_obj = tinygltf::detail::json::object();
  }
  return json_obj;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Prepare environment
  tinygltf::detail::json json_obj = CreateJsonFromInput(Data, Size);
  std::string err;
  
  // Target 1: ParseStringProperty
  std::string str_ret;
  tinygltf::ParseStringProperty(&str_ret, &err, json_obj, "test_string", true);
  tinygltf::ParseStringProperty(&str_ret, &err, json_obj, "test_string", false);

  // Target 2: ParseUnsignedProperty
  size_t uint_ret = 0;
  tinygltf::ParseUnsignedProperty(&uint_ret, &err, json_obj, "test_uint", true);
  tinygltf::ParseUnsignedProperty(&uint_ret, &err, json_obj, "test_uint", false);

  // Target 3: ParseNumberProperty
  double num_ret = 0.0;
  tinygltf::ParseNumberProperty(&num_ret, &err, json_obj, "test_number", true);
  tinygltf::ParseNumberProperty(&num_ret, &err, json_obj, "test_number", false);

  // Target 4: ParseSampler
  tinygltf::Sampler sampler;
  tinygltf::ParseSampler(&sampler, &err, json_obj, true);
  tinygltf::ParseSampler(&sampler, &err, json_obj, false);

  // Target 5: detail::GetString
  std::string get_str_val;
  tinygltf::detail::GetString(json_obj, get_str_val);

  // Target 6: ParseTexture
  tinygltf::Texture texture;
  tinygltf::ParseTexture(&texture, &err, json_obj, true, "./dummy_file");
  tinygltf::ParseTexture(&texture, &err, json_obj, false, "./dummy_file");

  return 0;
}