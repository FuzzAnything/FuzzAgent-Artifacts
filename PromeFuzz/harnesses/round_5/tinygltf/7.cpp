// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
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

static void fuzzValueFunctions(const tinygltf::Value& value) {
    // Call all the target API functions
    bool isArray = value.IsArray();
    bool isBinary = value.IsBinary();
    bool isBool = value.IsBool();
    bool isReal = value.IsReal();
    bool isString = value.IsString();
    
    // Try to get values based on type checks
    if (value.IsArray()) {
        try {
            const tinygltf::Value::Array& arr = value.Get<tinygltf::Value::Array>();
            (void)arr; // Use variable to avoid unused warning
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    if (value.IsBinary()) {
        try {
            const std::vector<unsigned char>& bin = value.Get<std::vector<unsigned char>>();
            (void)bin;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    if (value.IsBool()) {
        try {
            bool b = value.Get<bool>();
            (void)b;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    if (value.IsReal()) {
        try {
            double d = value.Get<double>();
            (void)d;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    if (value.IsString()) {
        try {
            const std::string& s = value.Get<std::string>();
            (void)s;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Try getting other types that might be present
    try {
        int i = value.Get<int>();
        (void)i;
    } catch (...) {
        // Ignore exceptions
    }
    
    try {
        const tinygltf::Value::Object& obj = value.Get<tinygltf::Value::Object>();
        (void)obj;
    } catch (...) {
        // Ignore exceptions
    }
}

static tinygltf::Value createValueFromFuzzData(const uint8_t* data, size_t size) {
    // Use fuzz data to create different types of Value objects
    if (size == 0) {
        // Create a default value (null)
        return tinygltf::Value();
    }
    
    // Use first byte to determine value type
    uint8_t typeSelector = data[0] % 7;
    
    switch (typeSelector) {
        case 0: {
            // Create bool value
            bool b = (data[0] & 0x1) != 0;
            return tinygltf::Value(b);
        }
        case 1: {
            // Create int value
            int i = static_cast<int>(data[0]);
            return tinygltf::Value(i);
        }
        case 2: {
            // Create double value
            double d = static_cast<double>(data[0]);
            return tinygltf::Value(d);
        }
        case 3: {
            // Create string value
            std::string s;
            if (size > 1) {
                size_t strLen = std::min(size - 1, static_cast<size_t>(data[1]));
                if (strLen > 0 && 2 + strLen <= size) {
                    s.assign(reinterpret_cast<const char*>(data + 2), strLen);
                }
            }
            return tinygltf::Value(s);
        }
        case 4: {
            // Create binary value - use move constructor
            std::vector<unsigned char> bin;
            if (size > 1) {
                size_t binLen = std::min(size - 1, static_cast<size_t>(data[1]));
                if (binLen > 0 && 2 + binLen <= size) {
                    bin.assign(data + 2, data + 2 + binLen);
                }
            }
            return tinygltf::Value(std::move(bin));
        }
        case 5: {
            // Create array value
            tinygltf::Value::Array arr;
            if (size > 1) {
                size_t arrSize = std::min(static_cast<size_t>(data[1]), size - 2);
                for (size_t i = 0; i < arrSize && 2 + i < size; i++) {
                    arr.push_back(tinygltf::Value(static_cast<int>(data[2 + i])));
                }
            }
            return tinygltf::Value(std::move(arr));
        }
        case 6: {
            // Create object value
            tinygltf::Value::Object obj;
            if (size > 2) {
                // Create a simple object with one key-value pair
                std::string key = "key";
                int val = static_cast<int>(data[2]);
                obj[key] = tinygltf::Value(val);
            }
            return tinygltf::Value(std::move(obj));
        }
        default:
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* data, size_t size) {
    // Create Value objects with different types
    tinygltf::Value value = createValueFromFuzzData(data, size);
    
    // Test the Value object with all target functions
    fuzzValueFunctions(value);
    
    // Create additional Value objects with different constructors
    // Test array constructor
    tinygltf::Value::Array arr;
    if (size > 0) {
        size_t arrSize = std::min(static_cast<size_t>(data[0] % 10), size - 1);
        for (size_t i = 0; i < arrSize && 1 + i < size; i++) {
            arr.push_back(tinygltf::Value(static_cast<int>(data[1 + i])));
        }
    }
    tinygltf::Value arrayValue(std::move(arr));
    fuzzValueFunctions(arrayValue);
    
    // Test with color value (using array<double, 4>)
    if (size >= 4) {
        tinygltf::ColorValue color;
        for (int i = 0; i < 4 && i < static_cast<int>(size); i++) {
            color[i] = static_cast<double>(data[i]) / 255.0;
        }
        // Note: ColorValue is not directly constructible to Value, but we can use it in an array
        tinygltf::Value::Array colorArr;
        for (double d : color) {
            colorArr.push_back(tinygltf::Value(d));
        }
        tinygltf::Value colorValue(std::move(colorArr));
        fuzzValueFunctions(colorValue);
    }
    
    return 0;
}