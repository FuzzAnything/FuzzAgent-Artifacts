// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    WriteDummyFile(Data, Size);

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. tinygltf::TinyGLTF::LoadASCIIFromFile
    bool loadSuccess = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);

    // We proceed to test Value accessors regardless of load success to explore different states
    // (e.g., empty model vs partially parsed model)

    // Access the root JSON value if available (simulated via model extras or extensions if needed)
    // Since LoadASCIIFromFile populates the Model, we can check specific fields.
    // The task requires specific Value API calls. We will use model.extras or similar fields.
    
    // Helper to safely get a Value from the model for testing
    const tinygltf::Value &rootVal = model.extras;

    // 2. tinygltf::Value::IsObject
    if (rootVal.IsObject()) {
        // 3. tinygltf::Value::Get (Object overload) - try to get a key "test"
        // Note: The prompt mentions "Get(size_t)" for arrays and implies object access.
        // The provided signature list shows `TINYGLTF_VALUE_GET(Value::Object, object_value_)`
        // which implies a Get method for objects exists in the full library, often `Get(const std::string&)`.
        // However, the prompt specifically lists `Get(size_t)` and says "You should invoke all 2 overloads above".
        // Since the prompt only explicitly showed `Get(size_t)` in the signature block but mentioned "all 2 overloads",
        // and the context is glTF (JSON), the second overload is likely `Get(const std::string&)`.
        // We will attempt to call Get with a string key if IsObject is true.
        const tinygltf::Value &objVal = rootVal.Get("test_key");
        
        // 4. tinygltf::Value::IsArray
        if (objVal.IsArray()) {
            // 5. tinygltf::Value::Get (Array overload) - index 0
            const tinygltf::Value &arrVal0 = objVal.Get(0);
            
            // 6. tinygltf::Value::Get (Array overload) - index 1
            const tinygltf::Value &arrVal1 = objVal.Get(1);
            
            // 7. tinygltf::Value::Get (Array overload) - index 2
            const tinygltf::Value &arrVal2 = objVal.Get(2);
            
            // 8. tinygltf::Value::GetNumberAsDouble
            double num1 = arrVal0.GetNumberAsDouble();
            (void)num1;

            // 9. tinygltf::Value::Get (Array overload) - index 3
            const tinygltf::Value &arrVal3 = objVal.Get(3);
            
            // 10. tinygltf::Value::GetNumberAsDouble
            double num2 = arrVal3.GetNumberAsDouble();
            (void)num2;
        }
    }

    // Additional exploration: Try accessing arrays directly if the root is an array (unlikely for glTF but good for fuzzing)
    if (rootVal.IsArray()) {
        // 3. tinygltf::Value::Get (Array overload)
        const tinygltf::Value &val0 = rootVal.Get(0);
        
        // 4. tinygltf::Value::IsArray
        if (val0.IsArray()) {
            // 5. tinygltf::Value::Get (Array overload)
            const tinygltf::Value &val1 = val0.Get(0);
            
            // 6. tinygltf::Value::Get (Array overload)
            const tinygltf::Value &val2 = val0.Get(1);
            
            // 7. tinygltf::Value::Get (Array overload)
            const tinygltf::Value &val3 = val0.Get(2);
            
            // 8. tinygltf::Value::GetNumberAsDouble
            double n1 = val1.GetNumberAsDouble();
            (void)n1;

            // 9. tinygltf::Value::Get (Array overload)
            const tinygltf::Value &val4 = val0.Get(3);
            
            // 10. tinygltf::Value::GetNumberAsDouble
            double n2 = val4.GetNumberAsDouble();
            (void)n2;
        }
    }

    // Try to invoke Get on object with string key even if IsObject was false to test error handling paths if any
    // (Though typically IsObject guards this, fuzzing might trigger internal asserts or checks)
    // We rely on the library's safety for out-of-bounds or type mismatches.

    return 0;
}