// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::JsonFromString at tiny_gltf.h:6967:14 in tiny_gltf.h
// tinygltf::detail::JsonAssign at tiny_gltf.h:6975:6 in tiny_gltf.h
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::SerializeExtrasAndExtensions at tiny_gltf.h:7306:6 in tiny_gltf.h
// tinygltf::SerializeGltfBufferBin at tiny_gltf.h:7493:13 in tiny_gltf.h
// tinygltf::SerializeGltfBufferData at tiny_gltf.h:7193:13 in tiny_gltf.h
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <utility>

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Ensure null-termination for string parsing
  std::string input(reinterpret_cast<const char*>(Data), Size);
  if (input.back() != '\0') {
    input += '\0';
  }

  // 1. Test tinygltf::detail::JsonFromString
  try {
    tinygltf::detail::json json_obj = tinygltf::detail::JsonFromString(input.c_str());
    tinygltf::detail::json json_copy;
    tinygltf::detail::JsonAssign(json_copy, json_obj);
  } catch (...) {
    // Ignore exceptions from invalid JSON
  }

  // 2. Test tinygltf::detail::JsonPushBack
  try {
    tinygltf::detail::json array_obj = tinygltf::detail::json::array();
    tinygltf::detail::json value_to_push = 123;
    tinygltf::detail::JsonPushBack(array_obj, std::move(value_to_push));
    
    if (Size > 0) {
      tinygltf::detail::json val = static_cast<int>(Data[0]);
      tinygltf::detail::JsonPushBack(array_obj, std::move(val));
    }
  } catch (...) {
    // Ignore
  }

  // 3. Test tinygltf::SerializeExtrasAndExtensions
  try {
    tinygltf::Buffer buffer;
    buffer.data.assign(Data, Data + Size);
    buffer.name = "fuzz_buffer";
    
    if (Size > 0) {
      buffer.extras = tinygltf::Value(input.substr(0, std::min(input.size(), static_cast<size_t>(100))).c_str());
    }
    
    tinygltf::detail::json json_out;
    tinygltf::SerializeExtrasAndExtensions(buffer, json_out);
  } catch (...) {
    // Ignore
  }

  // 4. Test tinygltf::SerializeGltfBufferBin
  try {
    tinygltf::Buffer buffer;
    buffer.data.assign(Data, Data + Size);
    buffer.name = "binary_buffer";
    
    tinygltf::detail::json json_bin;
    std::vector<unsigned char> binBuffer;
    tinygltf::SerializeGltfBufferBin(buffer, json_bin, binBuffer);
  } catch (...) {
    // Ignore
  }

  // 5. Test tinygltf::SerializeGltfBufferData
  try {
    std::vector<unsigned char> data_vec(Data, Data + Size);
    tinygltf::detail::json json_data;
    tinygltf::SerializeGltfBufferData(data_vec, json_data);
  } catch (...) {
    // Ignore
  }

  return 0;
}