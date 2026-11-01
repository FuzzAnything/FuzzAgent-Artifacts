// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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
#include <cstddef>
#include <vector>
#include <string>
#include <array>

static void FuzzValueWithInput(const uint8_t *Data, size_t Size, tinygltf::Value &val) {
    if (Size == 0) return;

    int choice = Data[0] % 7;
    size_t offset = 1;
    size_t remaining = Size - offset;

    switch (choice) {
        case 0: // Real
            if (remaining >= sizeof(double)) {
                double d = 0.0;
                std::memcpy(&d, &Data[offset], sizeof(double));
                val = tinygltf::Value(d);
            } else {
                val = tinygltf::Value(0.0);
            }
            break;
        case 1: // Bool
            // Ensure offset is valid before accessing Data[offset]
            if (offset < Size) {
                val = tinygltf::Value((Data[offset] != 0));
            } else {
                val = tinygltf::Value(false);
            }
            break;
        case 2: // Int
            if (remaining >= sizeof(int)) {
                int i = 0;
                std::memcpy(&i, &Data[offset], sizeof(int));
                val = tinygltf::Value(i);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 3: // String
            {
                size_t len = (remaining > 0) ? (Data[offset] % remaining) : 0;
                if (len > 0 && offset + len <= Size) {
                    std::string s(reinterpret_cast<const char*>(&Data[offset]), len);
                    val = tinygltf::Value(s);
                } else {
                    val = tinygltf::Value(std::string());
                }
            }
            break;
        case 4: // Binary
            {
                size_t len = (remaining > 0) ? (Data[offset] % remaining) : 0;
                if (len > 0 && offset + len <= Size) {
                    val = tinygltf::Value(&Data[offset], len);
                } else {
                    val = tinygltf::Value(std::vector<unsigned char>());
                }
            }
            break;
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                size_t count = (remaining > 0) ? (Data[offset] % (remaining + 1)) : 0;
                size_t idx = offset;
                for (size_t i = 0; i < count; ++i) {
                    if (idx >= Size) break;
                    int subChoice = Data[idx] % 5;
                    tinygltf::Value subVal;
                    if (idx + 1 >= Size) {
                        subVal = tinygltf::Value(0);
                    } else {
                        if (subChoice == 0) subVal = tinygltf::Value(static_cast<double>(Data[idx+1]));
                        else if (subChoice == 1) subVal = tinygltf::Value(Data[idx+1] != 0);
                        else if (subChoice == 2) subVal = tinygltf::Value(static_cast<int>(Data[idx+1]));
                        else if (subChoice == 3) subVal = tinygltf::Value(std::string(1, Data[idx+1]));
                        else {
                            subVal = tinygltf::Value(&Data[idx+1], 1);
                        }
                    }
                    arr.push_back(subVal);
                    idx += 2;
                }
                val = tinygltf::Value(arr);
            }
            break;
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                size_t count = (remaining > 0) ? (Data[offset] % (remaining + 1)) : 0;
                size_t idx = offset;
                for (size_t i = 0; i < count; ++i) {
                    if (idx >= Size) break;
                    size_t keyLen = (Data[idx] % 10) + 1;
                    std::string key;
                    if (idx + keyLen <= Size) {
                        key = std::string(reinterpret_cast<const char*>(&Data[idx]), keyLen);
                    } else {
                        key = "key";
                    }
                    idx += keyLen;
                    
                    int subChoice = (idx < Size) ? (Data[idx] % 5) : 0;
                    tinygltf::Value subVal;
                    if (idx + 1 >= Size) {
                        subVal = tinygltf::Value(0);
                    } else {
                        if (subChoice == 0) subVal = tinygltf::Value(static_cast<double>(Data[idx+1]));
                        else if (subChoice == 1) subVal = tinygltf::Value(Data[idx+1] != 0);
                        else if (subChoice == 2) subVal = tinygltf::Value(static_cast<int>(Data[idx+1]));
                        else if (subChoice == 3) subVal = tinygltf::Value(std::string(1, Data[idx+1]));
                        else {
                            subVal = tinygltf::Value(&Data[idx+1], 1);
                        }
                    }
                    
                    obj[key] = subVal;
                    idx += 2;
                }
                val = tinygltf::Value(obj);
            }
            break;
    }
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val;
    
    // Create a value based on input data
    FuzzValueWithInput(Data, Size, val);

    // Fuzz IsReal
    bool isReal = val.IsReal();
    if (isReal) {
        double d = val.Get<double>();
        (void)d;
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

    // Fuzz IsString
    bool isString = val.IsString();
    if (isString) {
        std::string s = val.Get<std::string>();
        (void)s;
    }

    // Fuzz IsBinary
    bool isBinary = val.IsBinary();
    if (isBinary) {
        std::vector<unsigned char> bin = val.Get<std::vector<unsigned char>>();
        (void)bin;
    }

    // Fuzz Get (Array access)
    if (val.IsArray()) {
        tinygltf::Value::Array arr = val.Get<tinygltf::Value::Array>();
        if (!arr.empty()) {
            tinygltf::Value first = arr[0];
            (void)first;
            
            if (arr.size() > 0) {
                tinygltf::Value last = arr[arr.size() - 1];
                (void)last;
            }
        }
    }

    return 0;
}