// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::JsonParse at tiny_gltf.h:1874:6 in tiny_gltf.h
// tinygltf::ParseAnimation at tiny_gltf.h:5544:13 in tiny_gltf.h
// tinygltf::ParseJsonAsValue at tiny_gltf.h:3664:13 in tiny_gltf.h
// tinygltf::detail::FindMember at tiny_gltf.h:3578:6 in tiny_gltf.h
// tinygltf::detail::FindMember at tiny_gltf.h:3578:6 in tiny_gltf.h
// tinygltf::detail::FindMember at tiny_gltf.h:3578:6 in tiny_gltf.h
// tinygltf::detail::FindMember at tiny_gltf.h:3578:6 in tiny_gltf.h
// tinygltf::detail::FindMember at tiny_gltf.h:3578:6 in tiny_gltf.h
// tinygltf::detail::ForEachInArray at tiny_gltf.h:5963:6 in tiny_gltf.h
// tinygltf::detail::ForEachInArray at tiny_gltf.h:5963:6 in tiny_gltf.h
// tinygltf::detail::ForEachInArray at tiny_gltf.h:5963:6 in tiny_gltf.h
// tinygltf::detail::ForEachInArray at tiny_gltf.h:5963:6 in tiny_gltf.h
// tinygltf::ParseNumberArrayProperty at tiny_gltf.h:3951:13 in tiny_gltf.h
// tinygltf::ParseNumberArrayProperty at tiny_gltf.h:3951:13 in tiny_gltf.h
// tinygltf::ParseNumberArrayProperty at tiny_gltf.h:3951:13 in tiny_gltf.h
// tinygltf::ParseNumberArrayProperty at tiny_gltf.h:3951:13 in tiny_gltf.h
// tinygltf::ParseNumberArrayProperty at tiny_gltf.h:3951:13 in tiny_gltf.h
// tinygltf::ParseNumberArrayProperty at tiny_gltf.h:3951:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

// Include the tinygltf header
#include "tiny_gltf.h"

// Helper to safely call JsonParse without throwing exceptions
// We use the internal namespace structure if available, or fallback to public API if needed.
// Since tinygltf often hides detail in the header, we assume the standard structure.
// If 'detail' is not exposed, we might need to use the public Model loader or parse manually.
// However, based on the task description, these functions exist. We will try to access them.
// If the header uses `namespace detail` inside `tinygltf`, we access it as `tinygltf::detail`.
// If the error persists, it implies the types are not exposed in the public header or require specific macros.
// Let's try to use the types directly as defined in the task description, assuming the header exposes them.
// If not, we might need to define a wrapper or use the public API that internally calls these.
// Given the error, it seems `tinygltf::detail` is not visible.
// We will attempt to use the public `tinygltf::Model` loader which internally uses these, 
// OR we assume the user has configured tinygltf to expose internals (e.g., via TINYGLTF_IMPLEMENTATION or similar).
// However, to fix the compilation error specifically regarding `detail` not being found:
// We will try to use the `tinygltf` namespace directly for the types if they are public, 
// or we will wrap the calls in a way that avoids direct access to `detail` if possible.
// But the task explicitly asks to fuzz `tinygltf::detail::FindMember` etc.
// This implies the header `tiny_gltf.h` must be compiled with `TINYGLTF_IMPLEMENTATION` or similar to expose internals, 
// OR the user's environment has a specific version.
// Let's assume the standard single-header setup where `TINYGLTF_IMPLEMENTATION` is defined before include to expose internals.
// If not, we cannot access `detail`. 
// Since the prompt says "tinygltf is a single-header C++ library", usually one defines `TINYGLTF_IMPLEMENTATION` before including.
// We will add that define to ensure internals are exposed.

#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Helper to safely call JsonParse
static void SafeJsonParse(tinygltf::detail::json &doc, const char *str, size_t length) {
  tinygltf::detail::JsonParse(doc, str, length, false);
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Create a string from the fuzzing input
  std::string input_str(reinterpret_cast<const char*>(Data), Size);

  // Parse JSON
  tinygltf::detail::json doc;
  SafeJsonParse(doc, input_str.c_str(), input_str.size());

  std::string err;
  tinygltf::Animation animation;
  tinygltf::Value value;
  std::vector<double> num_array;
  tinygltf::detail::json_const_iterator it;

  // 1. Test ParseAnimation
  tinygltf::ParseAnimation(&animation, &err, doc, true);

  // 2. Test ParseJsonAsValue
  tinygltf::ParseJsonAsValue(&value, doc);

  // 3. Test FindMember
  tinygltf::detail::FindMember(doc, "channels", it);
  tinygltf::detail::FindMember(doc, "samplers", it);
  tinygltf::detail::FindMember(doc, "name", it);
  tinygltf::detail::FindMember(doc, "extensions", it);
  tinygltf::detail::FindMember(doc, "extras", it);

  // 4. Test ForEachInArray
  auto callback = [](const tinygltf::detail::json &item) {
    return true;
  };

  tinygltf::detail::ForEachInArray(doc, "channels", callback);
  tinygltf::detail::ForEachInArray(doc, "samplers", callback);
  tinygltf::detail::ForEachInArray(doc, "nodes", callback);
  tinygltf::detail::ForEachInArray(doc, "meshes", callback);

  // 5. Test ParseNumberArrayProperty
  tinygltf::ParseNumberArrayProperty(&num_array, &err, doc, "weights", false);
  tinygltf::ParseNumberArrayProperty(&num_array, &err, doc, "min", false);
  tinygltf::ParseNumberArrayProperty(&num_array, &err, doc, "max", false);
  tinygltf::ParseNumberArrayProperty(&num_array, &err, doc, "scale", false);
  tinygltf::ParseNumberArrayProperty(&num_array, &err, doc, "translation", false);
  tinygltf::ParseNumberArrayProperty(&num_array, &err, doc, "rotation", false);

  return 0;
}