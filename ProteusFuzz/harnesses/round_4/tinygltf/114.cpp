// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Prevent excessively large inputs to avoid OOM during vector construction
  if (Size > 1024 * 1024) {
    return 0;
  }

  tinygltf::Value val;

  // Fuzzing Strategy 1: Construct Value from binary data
  // This exercises the constructor: explicit Value(const unsigned char *p, size_t n)
  {
    val = tinygltf::Value(Data, Size);
    
    // Test IsReal
    bool is_real = val.IsReal();
    (void)is_real;

    // Test IsString
    bool is_string = val.IsString();
    (void)is_string;

    // Test IsObject
    bool is_object = val.IsObject();
    (void)is_object;

    // Test IsArray
    bool is_array = val.IsArray();
    (void)is_array;

    // Test GetNumberAsDouble (only safe if IsReal or IsInt, but we check IsReal first per docs)
    // Note: The docs say "Non-number types yield undefined behavior". 
    // Since we constructed from binary, it's likely BINARY_TYPE, not REAL_TYPE.
    // We only call GetNumberAsDouble if IsReal is true to avoid UB in this specific test logic,
    // though a fuzzer might intentionally trigger UB in other contexts. 
    // Here we strictly follow the safety pattern described: "ensure type is numeric first".
    if (is_real) {
      double d = val.GetNumberAsDouble();
      (void)d;
    }

    // Test Get (only safe if IsArray)
    if (is_array) {
      // Try to access index 0
      tinygltf::Value elem = val.Get(0);
      (void)elem;
      
      // Try to access an out-of-bounds index (should return null safely)
      tinygltf::Value elem_oo = val.Get(Size + 100);
      (void)elem_oo;
    }
  }

  // Fuzzing Strategy 2: Manually construct different types to exercise type checks
  // We use the input data to seed values for other types if needed, 
  // but primarily we want to hit the logic paths for REAL, STRING, OBJECT, ARRAY.

  // Case: REAL_TYPE
  {
    double dummy_double = 0.0;
    if (Size > 0) {
      // Interpret first byte as a simple modifier to avoid always using 0.0
      dummy_double = static_cast<double>(Data[0]);
    }
    tinygltf::Value v_real(dummy_double);
    
    if (v_real.IsReal()) {
      double d = v_real.GetNumberAsDouble();
      (void)d;
    }
    (void)v_real.IsString();
    (void)v_real.IsObject();
    (void)v_real.IsArray();
    // Get on non-array should be safe if checked, but we check first
    if (v_real.IsArray()) {
       (void)v_real.Get(0);
    }
  }

  // Case: STRING_TYPE
  {
    std::string s(reinterpret_cast<const char*>(Data), Size);
    tinygltf::Value v_str(s);

    if (v_str.IsString()) {
      // Accessing string content is usually via GetString(), but we just check the type here
    }
    (void)v_str.IsReal();
    (void)v_str.IsObject();
    (void)v_str.IsArray();
    if (v_str.IsArray()) {
       (void)v_str.Get(0);
    }
  }

  // Case: OBJECT_TYPE
  {
    tinygltf::Value::Object obj;
    // Create a dummy object
    obj["key"] = tinygltf::Value(1.0);
    tinygltf::Value v_obj(obj);

    if (v_obj.IsObject()) {
      // Logic for object access would go here
    }
    (void)v_obj.IsReal();
    (void)v_obj.IsString();
    (void)v_obj.IsArray();
    if (v_obj.IsArray()) {
       (void)v_obj.Get(0);
    }
  }

  // Case: ARRAY_TYPE
  {
    tinygltf::Value::Array arr;
    // Fill array with some values based on input
    for (size_t i = 0; i < Size && i < 100; ++i) {
      arr.push_back(tinygltf::Value(static_cast<double>(Data[i])));
    }
    tinygltf::Value v_arr(arr);

    if (v_arr.IsArray()) {
      // Test Get with valid index
      if (arr.size() > 0) {
        tinygltf::Value elem = v_arr.Get(0);
        (void)elem;
      }
      // Test Get with invalid index (large)
      tinygltf::Value elem_invalid = v_arr.Get(999999);
      (void)elem_invalid;
    }
    (void)v_arr.IsReal();
    (void)v_arr.IsString();
    (void)v_arr.IsObject();
  }

  return 0;
}