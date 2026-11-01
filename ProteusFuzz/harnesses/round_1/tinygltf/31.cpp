// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Include the tinygltf library
#include "tiny_gltf.h"

// Forward declarations for tinygltf internal types to avoid compilation issues
// if the internal namespace is not fully exposed or if rapidjson/nlohmann headers
// are not directly included in the fuzzer environment.
namespace tinygltf {
namespace detail {
  // These types are typically defined based on the JSON backend (RapidJSON or nlohmann)
  // We rely on the header to define them correctly.
  // If the header defines them as nlohmann::json or rapidjson::Value, this works.
  // If not, the fuzzer might fail to compile without the specific JSON headers,
  // but the task assumes tiny_gltf.h is sufficient.
}
}

static void FuzzJSONIteration(const uint8_t *Data, size_t Size) {
  // Prepare the environment
  tinygltf::Model model;
  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  // Use a dummy file path as requested
  const char* dummy_file = "./dummy_file";

  // Write the fuzzing input to the dummy file
  {
    std::ofstream ofs(dummy_file, std::ios::binary);
    if (!ofs) {
      // If file creation fails, we cannot proceed with file-based loading
      return;
    }
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();
  }

  // Attempt to load the model. This is the primary entry point to trigger JSON parsing
  // which internally uses the target API functions (ArrayBegin, ArrayEnd, ObjectBegin, etc.)
  bool success = ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);

  // If loading failed, we might still want to test the iteration logic on empty or
  // manually constructed JSON objects if possible, but LoadASCIIFromFile is the main
  // driver for these internal helpers in the context of a model loader.
  // The internal functions are private, so we cannot call them directly on arbitrary
  // data without exposing the internal JSON parsing state.
  // However, the act of parsing the file exercises these functions.
  
  // We can also try to parse the data as a standalone JSON to ensure the parser
  // handles malformed input gracefully, which exercises the iteration logic
  // during error recovery or partial parsing.
  
  // Note: Since the target functions are in the `detail` namespace and private,
  // the only way to fuzz them effectively via the public API is to trigger
  // the parsing logic that calls them.
  
  // Additional exploration: Try to load as binary if the input looks like it could be
  // a binary glTF (though the function name suggests ASCII, the library often handles
  // both or the fuzzer might provide binary data that the parser tries to interpret).
  // We stick to the ASCII loader as per the function name context usually associated
  // with these helpers in the source.

  // Cleanup is handled by the destructor of 'model' and 'ctx'.
  // No explicit cleanup needed for the dummy file in this scope, 
  // but in a real fuzzing campaign, one might remove it.
  // For this driver, we leave it as is to avoid I/O overhead in the loop.
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Fuzz the JSON iteration logic by loading the input as a glTF file.
  // This triggers the internal JSON parsing and iteration (ArrayBegin, ArrayEnd,
  // ObjectBegin, ObjectEnd, GetValue, GetKey) within tinygltf.
  FuzzJSONIteration(Data, Size);

  return 0;
}