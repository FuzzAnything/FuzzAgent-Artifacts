// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ParseMesh at tiny_gltf.h:5150:13 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::detail::JsonAssign at tiny_gltf.h:6975:6 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::SerializeExtrasAndExtensions at tiny_gltf.h:7306:6 in tiny_gltf.h
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::SerializeGltfMesh at tiny_gltf.h:7742:13 in tiny_gltf.h
// tinygltf::ParseMesh at tiny_gltf.h:5150:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include "tiny_gltf.h"

// tinygltf often hides 'detail' namespace in the public header or requires specific macros.
// However, the task explicitly asks to fuzz these functions. 
// In many versions of tinygltf, the 'detail' namespace is internal. 
// If the build configuration hides it, we might need to access it via a friend or by defining a macro if allowed,
// but typically for fuzzing internal APIs, we assume the header exposes them or we use the public API that wraps them.
// Since the error says 'no member named detail', we must check if we can access it.
// In some builds, TINYGLTF_IMPLEMENTATION is defined in the source file, but here we include the header.
// If the header is single-header and we are compiling the fuzzer, we might need to ensure the implementation is visible.
// However, the standard way to fuzz internal functions when they are not exposed is to use the public API.
// But the prompt insists on these specific functions. 
// Let's assume the user's environment has the 'detail' namespace exposed or we need to cast/forward declare if possible.
// Actually, looking at tinygltf source, 'detail' is often a namespace inside tinygltf.
// If it's not visible, it might be because the header is included in a way that hides internal details.
// Let's try to access it directly. If it fails, we might need to define TINYGLTF_IMPLEMENTATION before including if it's a single file header setup,
// but usually, for fuzzing, we link against the library.
// Given the error, the namespace is truly not visible. 
// However, the prompt says "Write a C++ fuzz driver program to fuzz the library tinygltf using the information provided below."
// and lists functions in `tinygltf::detail`.
// This implies the environment should allow access. 
// Perhaps the include path or the way tiny_gltf.h is set up in the fuzzing environment is specific.
// But since I cannot change the build system, I must write code that compiles.
// If `tinygltf::detail` is not available, I cannot call these functions directly.
// Wait, maybe the functions are not in `tinygltf::detail` in the public API but the prompt says they are.
// Let's re-read the error: "no member named 'detail' in namespace 'tinygltf'".
// This means the `detail` namespace is not defined in `tinygltf` in the current compilation unit.
// In the actual tinygltf library, `detail` is often defined inside the header but might be hidden by `#ifdef` or only visible if `TINYGLTF_IMPLEMENTATION` is defined.
// Since this is a fuzz driver, we likely link against a pre-compiled library where `detail` is internal.
// However, the task requires fuzzing these specific functions.
// If the functions are truly internal and not exposed, we cannot fuzz them directly from a separate translation unit unless we use `friend` or the header is included with implementation.
// But the prompt implies it's possible.
// Let's assume the user has configured the build such that `tinygltf` exposes `detail` or we need to define a macro.
// A common pattern in single-header libraries is to define `TINYGLTF_IMPLEMENTATION` in exactly one source file.
// If this fuzzer is that source file, we might need to define it.
// Let's try defining `TINYGLTF_IMPLEMENTATION` before including the header to force the implementation (and thus the `detail` namespace) to be visible.
// This is a common requirement for single-header libraries like tinygltf when you want to use internal functions or if the library is designed to be included in one file.

#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Ensure null-termination for string parsing
  std::string input_str(reinterpret_cast<const char*>(Data), Size);
  if (input_str.back() != '\0') {
    input_str += '\0';
  }

  try {
    // 1. Fuzz tinygltf::detail::JsonFromString
    tinygltf::detail::json json_obj = tinygltf::detail::JsonFromString(input_str.c_str());

    // 2. Fuzz tinygltf::detail::JsonPushBack
    tinygltf::detail::json target_array = tinygltf::detail::JsonFromString("[{}]");
    if (target_array.is_array()) {
      tinygltf::detail::json value_to_push = tinygltf::detail::JsonFromString("123");
      tinygltf::detail::JsonPushBack(target_array, std::move(value_to_push));
    }

    // 3. Fuzz tinygltf::detail::JsonAssign
    tinygltf::detail::json dest_json = tinygltf::detail::JsonFromString("{}");
    tinygltf::detail::json src_json = tinygltf::detail::JsonFromString("null");
    tinygltf::detail::JsonAssign(dest_json, src_json);

    // 4. Fuzz tinygltf::SerializeExtrasAndExtensions
    tinygltf::Mesh mesh;
    mesh.name = "fuzz_mesh";
    tinygltf::Primitive prim;
    prim.attributes["POSITION"] = 0;
    mesh.primitives.push_back(prim);
    
    tinygltf::detail::json mesh_json = tinygltf::detail::JsonFromString("{}");
    tinygltf::SerializeExtrasAndExtensions(mesh, mesh_json);

    // 5. Fuzz tinygltf::SerializeGltfMesh
    tinygltf::detail::json mesh_out_json = tinygltf::detail::JsonFromString("{}");
    tinygltf::SerializeGltfMesh(mesh, mesh_out_json);

    // 6. Fuzz tinygltf::ParseMesh
    tinygltf::Mesh parsed_mesh;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    bool parse_success = tinygltf::ParseMesh(
      &parsed_mesh,
      &model,
      &err,
      &warn,
      json_obj,
      false,
      tinygltf::Permissive
    );
    
    tinygltf::Mesh parsed_mesh_strict;
    bool parse_success_strict = tinygltf::ParseMesh(
      &parsed_mesh_strict,
      &model,
      &err,
      &warn,
      json_obj,
      true,
      tinygltf::Strict
    );

  } catch (const std::exception &) {
    return 0;
  } catch (...) {
    return 0;
  }

  return 0;
}