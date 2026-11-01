// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
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
#include <array>

static void FuzzValueGet(tinygltf::Value &val, const uint8_t *Data, size_t Size) {
  // Test Get(size_t) on arrays
  if (val.IsArray()) {
    // Try various indices including out of bounds
    size_t idx = 0;
    if (Size >= sizeof(size_t)) {
      std::memcpy(&idx, Data, sizeof(size_t));
      idx = idx % 1000000; // Limit index to prevent extreme memory issues
    }
    
    // Access element at index. Get returns a const reference for safety.
    const tinygltf::Value &elem = val.Get(idx);
    
    // Try to access different types from the retrieved element
    if (elem.IsBool()) {
      (void)elem.Get<bool>();
    }
    if (elem.IsInt()) {
      (void)elem.Get<int>();
    }
    if (elem.IsReal()) {
      (void)elem.Get<double>();
    }
    if (elem.IsString()) {
      (void)elem.Get<std::string>();
    }
    if (elem.IsBinary()) {
      (void)elem.Get<std::vector<unsigned char>>();
    }
    if (elem.IsArray()) {
      (void)elem.Get<tinygltf::Value::Array>();
    }
    if (elem.IsObject()) {
      (void)elem.Get<tinygltf::Value::Object>();
    }
  }
}

static void FuzzTypeChecks(const tinygltf::Value &val) {
  // Test all type check functions
  (void)val.IsBool();
  (void)val.IsInt();
  (void)val.IsReal();
  (void)val.IsString();
  (void)val.IsBinary();
  (void)val.IsArray();
  (void)val.IsObject();
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }
  
  tinygltf::Value val;
  uint8_t typeSelector = Data[0] % 8;
  
  switch (typeSelector) {
    case 0: // Bool
      val = tinygltf::Value(true);
      break;
    case 1: // Int
      if (Size >= 1 + sizeof(int)) {
        int intVal;
        std::memcpy(&intVal, Data + 1, sizeof(int));
        val = tinygltf::Value(intVal);
      } else {
        val = tinygltf::Value(0);
      }
      break;
    case 2: // Real
      if (Size >= 1 + sizeof(double)) {
        double realVal;
        std::memcpy(&realVal, Data + 1, sizeof(double));
        val = tinygltf::Value(realVal);
      } else {
        val = tinygltf::Value(0.0);
      }
      break;
    case 3: // String
      if (Size > 1) {
        std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
        val = tinygltf::Value(str);
      } else {
        val = tinygltf::Value(std::string());
      }
      break;
    case 4: // Binary
      if (Size > 1) {
        std::vector<unsigned char> binData(Data + 1, Data + Size);
        val = tinygltf::Value(binData.data(), binData.size());
      } else {
        val = tinygltf::Value(std::vector<unsigned char>());
      }
      break;
    case 5: // Array
      {
        tinygltf::Value::Array arr;
        size_t numElements = 0;
        if (Size >= 1 + sizeof(size_t)) {
          std::memcpy(&numElements, Data + 1, sizeof(size_t));
          numElements = numElements % 100; // Limit array size
        }
        
        // Ensure we don't read past the buffer
        size_t maxSafeElements = (Size > 1) ? (Size - 1) : 0;
        if (numElements > maxSafeElements) {
          numElements = maxSafeElements;
        }
        
        for (size_t i = 0; i < numElements; i++) {
          uint8_t elemType = (Data[1 + i] % 5);
          switch (elemType) {
            case 0: arr.push_back(tinygltf::Value(true)); break;
            case 1: arr.push_back(tinygltf::Value(static_cast<int>(Data[1 + i]))); break;
            case 2: arr.push_back(tinygltf::Value(static_cast<double>(Data[1 + i]))); break;
            case 3: arr.push_back(tinygltf::Value(std::string(1, Data[1 + i]))); break;
            case 4: arr.push_back(tinygltf::Value(std::vector<unsigned char>(1, Data[1 + i]))); break;
          }
        }
        val = tinygltf::Value(arr);
      }
      break;
    case 6: // Object
      {
        tinygltf::Value::Object obj;
        size_t numPairs = 0;
        if (Size >= 1 + sizeof(size_t)) {
          std::memcpy(&numPairs, Data + 1, sizeof(size_t));
          numPairs = numPairs % 50; // Limit object size
        }
        
        // Ensure we don't read past the buffer
        size_t maxSafePairs = (Size > 1) ? (Size - 1) : 0;
        if (numPairs > maxSafePairs) {
          numPairs = maxSafePairs;
        }
        
        for (size_t i = 0; i < numPairs; i++) {
          std::string key = "key_" + std::to_string(i);
          uint8_t valType = (Data[1 + i] % 5);
          switch (valType) {
            case 0: obj[key] = tinygltf::Value(true); break;
            case 1: obj[key] = tinygltf::Value(static_cast<int>(Data[1 + i])); break;
            case 2: obj[key] = tinygltf::Value(static_cast<double>(Data[1 + i])); break;
            case 3: obj[key] = tinygltf::Value(std::string(1, Data[1 + i])); break;
            case 4: obj[key] = tinygltf::Value(std::vector<unsigned char>(1, Data[1 + i])); break;
          }
        }
        val = tinygltf::Value(obj);
      }
      break;
    case 7: // Null
      val = tinygltf::Value();
      break;
  }
  
  // Perform type checks
  FuzzTypeChecks(val);
  
  // Perform Get operations if applicable
  FuzzValueGet(val, Data, Size);
  
  return 0;
}