// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <array>

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::Value value;
  
  // Strategy 1: Construct a BINARY_TYPE value directly from fuzz input
  // This exercises the constructor: explicit Value(const unsigned char *p, size_t n)
  value = tinygltf::Value(Data, Size);

  // Test IsBool
  bool is_bool = value.IsBool();
  (void)is_bool;

  // Test IsInt
  bool is_int = value.IsInt();
  (void)is_int;

  // Test IsObject
  bool is_object = value.IsObject();
  (void)is_object;

  // Test IsString
  bool is_string = value.IsString();
  (void)is_string;

  // Test IsArray
  bool is_array = value.IsArray();
  (void)is_array;

  // Strategy 2: Construct other types to exercise type checks and Get methods
  // We create a new value to test Array and Object types specifically
  tinygltf::Value array_val;
  tinygltf::Value object_val;
  tinygltf::Value string_val;
  tinygltf::Value int_val;
  tinygltf::Value bool_val;

  // Initialize Array
  tinygltf::Value::Array arr;
  if (Size > 0) {
    arr.push_back(tinygltf::Value(static_cast<int>(Data[0])));
    if (Size > 1) {
      arr.push_back(tinygltf::Value(static_cast<double>(Data[1])));
    }
    if (Size > 2) {
      arr.push_back(tinygltf::Value(std::string(reinterpret_cast<const char*>(Data), Size)));
    }
  }
  array_val = tinygltf::Value(arr);

  // Initialize Object
  tinygltf::Value::Object obj;
  if (Size > 0) {
    std::string key = "key_" + std::to_string(Data[0]);
    obj[key] = tinygltf::Value(static_cast<int>(Data[0]));
  }
  object_val = tinygltf::Value(obj);

  // Initialize String
  string_val = tinygltf::Value(std::string(reinterpret_cast<const char*>(Data), Size));

  // Initialize Int
  int_val = tinygltf::Value(static_cast<int>(Data[0]));

  // Initialize Bool
  bool_val = tinygltf::Value(Data[0] != 0);

  // Test Get methods on Array
  if (array_val.IsArray()) {
    // Access index 0
    tinygltf::Value elem0 = array_val.Get(0);
    (void)elem0;
    
    // Access out of bounds (should return null)
    tinygltf::Value elem_out = array_val.Get(99999);
    (void)elem_out;
  }

  // Test Get methods on Object (Note: Get(size_t) is for arrays, but we can check type safety)
  // The Get(size_t) method asserts the value is an array. 
  // We intentionally call it on non-arrays to test the assertion/safety logic if any, 
  // though typically this might trigger an assert in debug builds. 
  // For fuzzing, we rely on the IsArray check first.
  if (!object_val.IsArray() && !string_val.IsArray() && !int_val.IsArray() && !bool_val.IsArray()) {
    // These should not be arrays, so Get(size_t) behavior depends on implementation safety.
    // In a safe implementation, it might return a null value or assert.
    // We wrap in a try-catch if exceptions are possible, but tinygltf often uses asserts.
    // Since we cannot catch asserts, we rely on the IsArray check to guide logic.
    // However, to exercise the code path, we call it on the array_val only if it is an array.
    // The prompt says "asserts the value is an array", so calling on non-array might crash in debug.
    // To be safe and explore states, we only call Get on values we know are arrays.
  }

  // Additional exploration: Construct a Value from binary data with varying sizes
  for (size_t i = 0; i < Size; ++i) {
    tinygltf::Value binary_val(Data + i, Size - i);
    (void)binary_val.IsBool();
    (void)binary_val.IsInt();
    (void)binary_val.IsObject();
    (void)binary_val.IsString();
    (void)binary_val.IsArray();
    
    if (binary_val.IsArray()) {
      // Try to get an element if it's an array (though binary type shouldn't be array)
      (void)binary_val.Get(0);
    }
  }

  return 0;
}