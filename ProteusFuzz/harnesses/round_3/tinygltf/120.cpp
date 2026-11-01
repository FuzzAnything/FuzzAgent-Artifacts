// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Helper to safely parse a double from fuzzed input
static double ParseDouble(const uint8_t *Data, size_t Size, size_t &Offset) {
    if (Offset + 1 > Size) return 0.0;
    double val = 0.0;
    // Simple heuristic: take a few bytes to form a double or int
    if (Offset + 8 <= Size) {
        std::memcpy(&val, Data + Offset, 8);
        Offset += 8;
    } else {
        int32_t iv = 0;
        if (Offset + 4 <= Size) {
            std::memcpy(&iv, Data + Offset, 4);
            Offset += 4;
            val = static_cast<double>(iv);
        } else {
            val = static_cast<double>(Data[Offset]);
            Offset++;
        }
    }
    return val;
}

// Helper to safely parse an int from fuzzed input
static int32_t ParseInt(const uint8_t *Data, size_t Size, size_t &Offset) {
    int32_t val = 0;
    if (Offset + 4 <= Size) {
        std::memcpy(&val, Data + Offset, 4);
        Offset += 4;
    } else if (Offset + 2 <= Size) {
        int16_t iv = 0;
        std::memcpy(&iv, Data + Offset, 2);
        Offset += 2;
        val = static_cast<int32_t>(iv);
    } else if (Offset < Size) {
        val = static_cast<int32_t>(Data[Offset]);
        Offset++;
    }
    return val;
}

// Helper to generate a JSON string from fuzzed data
static std::string GenerateJsonString(const uint8_t *Data, size_t Size) {
    std::string json_str = "{";
    // Add some random-looking but valid JSON structure
    json_str += "\"test\":";
    // Try to add a number
    size_t offset = 0;
    if (Size > 0) {
        int32_t num = ParseInt(Data, Size, offset);
        std::stringstream ss;
        ss << num;
        json_str += ss.str();
    } else {
        json_str += "0";
    }
    json_str += ", \"factor\":";
    if (Size > 0) {
        double d = ParseDouble(Data, Size, offset);
        std::stringstream ss;
        ss << d;
        json_str += ss.str();
    } else {
        json_str += "1.0";
    }
    json_str += ", \"scale\":";
    if (Size > 0) {
        double d = ParseDouble(Data, Size, offset);
        std::stringstream ss;
        ss << d;
        json_str += ss.str();
    } else {
        json_str += "0.5";
    }
    json_str += ", \"texCoord\":";
    if (Size > 0) {
        int32_t num = ParseInt(Data, Size, offset);
        std::stringstream ss;
        ss << num;
        json_str += ss.str();
    } else {
        json_str += "0";
    }
    json_str += "}";
    return json_str;
}

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment for tinygltf::detail::GetInt and GetNumber
    // We need to create a JSON object to test these functions.
    // Since we cannot directly construct tinygltf::detail::json without knowing the backend,
    // we will use the library's parser to create a Model from a dummy JSON file.
    // However, the target functions operate on the internal json type.
    // We can try to construct a minimal glTF JSON string and parse it.
    
    std::string json_content = GenerateJsonString(Data, Size);
    
    // Write to a dummy file to use with tinygltf loader if needed, 
    // but for direct API testing on internal types, we might need to access them differently.
    // Since tinygltf::detail::GetInt is internal, we can't call it directly without access to the internal json object.
    // However, we can test the public APIs that rely on these or simulate the conditions.
    // The prompt asks to fuzz the API functions. If they are internal, we might need to use the public API 
    // that exercises them or use a trick if the library exposes them in a specific build.
    // Given the constraints, we will focus on the public member functions: 
    // Parameter::Factor, Parameter::TextureScale, Parameter::TextureTexCoord, Value::GetNumberAsInt.
    // We will also try to trigger the internal logic by parsing a malformed JSON.

    // Let's create a dummy glTF JSON file to test the parser which uses GetInt and GetNumber internally.
    std::ofstream dummy_file("./dummy_file");
    if (dummy_file.is_open()) {
        dummy_file << json_content;
        dummy_file.close();
    } else {
        return 0;
    }

    // 2. Test tinygltf::Model loading (which internally uses GetInt, GetNumber)
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Attempt to load the dummy file. This exercises the parsing logic including GetInt/GetNumber.
    // LoadASCIIFromString requires 6 arguments: model, err, warn, str, length, base_dir, check_sections
    // We provide a base_dir and check_sections.
    loader.LoadASCIIFromString(&model, &err, &warn, json_content.c_str(), static_cast<unsigned int>(json_content.length()), "", 0);
    
    // 3. Test tinygltf::Parameter methods
    // We need to construct a Parameter object.
    tinygltf::Parameter param;
    
    // Test Factor()
    // The documentation says: "Returned value is only valid if the parameter represent a texture from a material"
    // But the function signature is double Factor() const.
    // If the parameter is not set up correctly, it might return undefined behavior or 0.
    // We will call it on a default constructed parameter to see if it crashes.
    double factor_val = param.Factor();
    (void)factor_val; // Suppress unused warning

    // Test TextureScale()
    double scale_val = param.TextureScale();
    (void)scale_val;

    // Test TextureTexCoord()
    int texcoord_val = param.TextureTexCoord();
    (void)texcoord_val;

    // 4. Test tinygltf::Value::GetNumberAsInt
    tinygltf::Value val;
    // Default constructed Value is likely null or 0.
    int int_val = val.GetNumberAsInt();
    (void)int_val;

    // 5. Try to construct Value with different types to test GetNumberAsInt robustness
    // We can create a Value with a number.
    tinygltf::Value val_num(123.456);
    int int_val_num = val_num.GetNumberAsInt();
    (void)int_val_num;

    // Create a Value with an integer
    tinygltf::Value val_int(42);
    int int_val_int = val_int.GetNumberAsInt();
    (void)int_val_int;

    // Create a Value with a string (should probably return 0 or handle gracefully)
    tinygltf::Value val_str("hello");
    int int_val_str = val_str.GetNumberAsInt();
    (void)int_val_str;

    // 6. Try to trigger internal GetInt/GetNumber by creating a more complex JSON
    // and parsing it. The loader will call these internally.
    // We already did this with LoadASCIIFromString.
    
    // 7. Cleanup
    // The Model and other objects are RAII, so they will be cleaned up automatically.
    // We should remove the dummy file.
    std::remove("./dummy_file");

    return 0;
}