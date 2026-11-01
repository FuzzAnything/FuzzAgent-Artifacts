// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
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

static void FuzzValueFromData(const uint8_t *Data, size_t Size, tinygltf::Value &val) {
  // Construct a Value from binary data if possible, otherwise create empty or scalar
  if (Size > 0) {
    val = tinygltf::Value(Data, Size);
  } else {
    val = tinygltf::Value();
  }
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::Value val;
  
  // Strategy 1: Construct from raw binary data
  FuzzValueFromData(Data, Size, val);

  // Test IsArray
  bool isArray = val.IsArray();
  
  // Test IsReal
  bool isReal = val.IsReal();
  
  // Test IsObject
  bool isObject = val.IsObject();

  // Test ArrayLen
  size_t arrayLen = val.ArrayLen();

  // Test Size
  size_t size = val.Size();

  // Test Get with various indices
  // If it's an array, try to access elements
  if (isArray) {
    // Access index 0 if array is not empty
    if (arrayLen > 0) {
      const tinygltf::Value &elem0 = val.Get(0);
      (void)elem0; // Suppress unused warning
    }
    // Access last element if exists
    if (arrayLen > 0) {
      const tinygltf::Value &elemLast = val.Get(arrayLen - 1);
      (void)elemLast;
    }
    // Access out of bounds (should return null safely)
    const tinygltf::Value &outOfBounds = val.Get(arrayLen + 100);
    (void)outOfBounds;
  }
  // Note: Removed the call to val.Get(0) in the else block.
  // tinygltf::Value::Get asserts IsArray() is true. 
  // Calling it on non-array types (like BINARY_TYPE created from raw data) causes a crash.

  // Strategy 2: Create a Value with specific types to exercise type checks
  // Create a real value
  tinygltf::Value realVal(3.14);
  (void)realVal.IsReal();
  (void)realVal.IsArray();
  (void)realVal.IsObject();
  (void)realVal.ArrayLen();
  (void)realVal.Size();
  // Do not call Get on non-array values like realVal

  // Strategy 3: Use the binary data to potentially trigger parsing logic if the library
  // interprets the binary data as JSON-like structure internally (unlikely for binary ctor).
  // But we can try to create a value that might be interpreted as an object or array
  // if the library has logic to parse the binary blob (unlikely for BINARY_TYPE).
  // The main goal is to call the functions on different states.

  // Create a value from a small subset of data to vary input
  if (Size > 10) {
    tinygltf::Value subVal(Data + 10, Size - 10);
    (void)subVal.IsArray();
    (void)subVal.IsReal();
    (void)subVal.IsObject();
    (void)subVal.ArrayLen();
    (void)subVal.Size();
    // Only call Get if subVal is actually an array
    if (subVal.IsArray()) {
      (void)subVal.Get(0);
    }
  }

  return 0;
}