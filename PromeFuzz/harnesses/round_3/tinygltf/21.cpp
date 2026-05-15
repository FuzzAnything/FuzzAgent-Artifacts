// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <string>

static void test_value_functions(const tinygltf::Value& value) {
  // Test all the API functions on the given Value object
  value.Type();
  value.IsNumber();
  value.IsInt();
  value.IsReal();
  
  // These may have side effects or throw exceptions, so call them conditionally
  if (value.IsNumber()) {
    value.GetNumberAsDouble();
    value.GetNumberAsInt();
  }
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Create a string from the fuzzer input
  std::string input_string(reinterpret_cast<const char*>(Data), Size);
  
  // Create a Value object using the string constructor
  tinygltf::Value value(std::move(input_string));
  
  // Test the API functions with the created Value
  test_value_functions(value);
  
  // Create additional test cases with different Value types
  // Test with empty string
  tinygltf::Value empty_value(std::string(""));
  test_value_functions(empty_value);
  
  // Test with numeric string
  tinygltf::Value numeric_string_value(std::string("123.45"));
  test_value_functions(numeric_string_value);
  
  // Test with special characters
  tinygltf::Value special_value(std::string("\x00\x01\x02", 3));
  test_value_functions(special_value);
  
  return 0;
}