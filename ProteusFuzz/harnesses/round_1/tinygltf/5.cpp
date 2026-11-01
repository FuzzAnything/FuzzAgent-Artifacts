// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  // Create a tinygltf::Value based on the first byte to determine type
  tinygltf::Value val;
  uint8_t typeSelector = Data[0];

  switch (typeSelector % 7) {
    case 0: // BOOL_TYPE
      if (Size >= 2) {
        val = tinygltf::Value((Data[1] & 1) != 0);
      } else {
        val = tinygltf::Value(false);
      }
      break;
    case 1: // INT_TYPE
      if (Size >= 5) {
        int intVal = *reinterpret_cast<const int*>(Data + 1);
        val = tinygltf::Value(intVal);
      } else {
        val = tinygltf::Value(0);
      }
      break;
    case 2: // REAL_TYPE
      if (Size >= 9) {
        double realVal = *reinterpret_cast<const double*>(Data + 1);
        val = tinygltf::Value(realVal);
      } else {
        val = tinygltf::Value(0.0);
      }
      break;
    case 3: // STRING_TYPE
      if (Size > 1) {
        std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
        val = tinygltf::Value(str);
      } else {
        val = tinygltf::Value("");
      }
      break;
    case 4: // ARRAY_TYPE
      {
        tinygltf::Value::Array arr;
        size_t offset = 1;
        while (offset < Size) {
          size_t remaining = Size - offset;
          if (remaining == 0) break;
          size_t chunkSize = remaining / 2;
          if (chunkSize == 0) chunkSize = 1;
          
          if (chunkSize > remaining) chunkSize = remaining;
          
          std::string chunk(reinterpret_cast<const char*>(Data + offset), chunkSize);
          arr.push_back(tinygltf::Value(chunk));
          offset += chunkSize;
        }
        val = tinygltf::Value(arr);
      }
      break;
    case 5: // OBJECT_TYPE
      {
        tinygltf::Value::Object obj;
        size_t offset = 1;
        while (offset < Size) {
          size_t remaining = Size - offset;
          if (remaining < 2) break;

          size_t keySize = remaining / 3;
          if (keySize == 0) keySize = 1;
          if (keySize > remaining) keySize = remaining;

          std::string key(reinterpret_cast<const char*>(Data + offset), keySize);
          offset += keySize;

          remaining = Size - offset;
          if (remaining == 0) break;

          size_t valSize = remaining / 2;
          if (valSize == 0) valSize = 1;
          if (valSize > remaining) valSize = remaining;

          std::string valStr(reinterpret_cast<const char*>(Data + offset), valSize);
          offset += valSize;

          obj[key] = tinygltf::Value(valStr);
        }
        val = tinygltf::Value(obj);
      }
      break;
    case 6: // NULL_TYPE
      val = tinygltf::Value(); 
      break;
  }

  // Fuzz IsBool
  bool isBool = val.IsBool();
  if (isBool) {
    bool b = val.Get<bool>();
    (void)b;
  }

  // Fuzz IsInt
  bool isInt = val.IsInt();
  if (isInt) {
    int i = val.Get<int>();
    (void)i;
  }

  // Fuzz IsNumber
  bool isNum = val.IsNumber();
  if (isNum) {
    double d = val.Get<double>();
    (void)d;
  }

  // Fuzz IsArray
  bool isArray = val.IsArray();
  if (isArray) {
    const auto& arr = val.Get<tinygltf::Value::Array>();
    if (Size > 2) {
      if (!arr.empty()) {
        size_t idx = Data[1] % arr.size();
        const auto& elem = arr[idx];
        (void)elem;
      }
    }
  }

  // Fuzz IsObject
  bool isObj = val.IsObject();
  if (isObj) {
    const auto& obj = val.Get<tinygltf::Value::Object>();
    (void)obj;
  }

  // Fuzz Get with index (only if array)
  if (isArray && Size > 0) {
    const auto& arrRef = val.Get<tinygltf::Value::Array>();
    size_t arrSize = arrRef.size();
    if (arrSize > 0) {
      size_t index = Data[0] % (arrSize + 1); 
      const auto& retrieved = val.Get(index);
      (void)retrieved;
    }
  }

  return 0;
}