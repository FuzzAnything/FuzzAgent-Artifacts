// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
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
#include <fstream>
#include <tiny_gltf.h>

// Helper to write data to a dummy file if needed
static void WriteToFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input to dummy file to simulate file-based loading if needed
    WriteToFile(Data, Size);

    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err, warn;

    // Attempt to load the input as a glTF file (triggers internal parsing)
    // This exercises GetInt, GetNumber, etc. indirectly during parsing.
    // Ensure we pass a null-terminated string or handle size correctly.
    // LoadASCIIFromString expects a C-string, so we must ensure null termination or use a safe wrapper.
    // However, tinygltf's LoadASCIIFromString typically takes a const char* and size_t, 
    // but internally might assume null-termination or use the size. 
    // To be safe against non-null-terminated input causing crashes in string operations:
    std::string inputStr(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety if the library uses strlen or similar
    inputStr += '\0'; 

    ctx.LoadASCIIFromString(&model, &err, &warn, 
                            inputStr.c_str(), inputStr.size(), nullptr);
    
    // 1. Test tinygltf::Parameter::Factor, TextureScale, TextureTexCoord
    tinygltf::Parameter p;
    
    // Test Factor()
    double f = p.Factor();
    (void)f;

    // Test TextureScale()
    double ts = p.TextureScale();
    (void)ts;

    // Test TextureTexCoord()
    int tc = p.TextureTexCoord();
    (void)tc;

    // 2. Test tinygltf::Value::GetNumberAsInt
    if (Size > 0) {
        // Try to create a Value from a string representation of a number
        std::string numStr;
        for (size_t i = 0; i < Size && i < 20; ++i) {
            char c = Data[i];
            if (std::isdigit(static_cast<unsigned char>(c)) || c == '.' || c == '-' || c == '+') {
                numStr += c;
            } else {
                break;
            }
        }

        if (!numStr.empty()) {
            // Create a Value. The constructor Value(const char*) creates a STRING_TYPE.
            tinygltf::Value strVal(numStr.c_str());
            int intVal = strVal.GetNumberAsInt();
            (void)intVal;
        }

        // Try to create a Value with a double and call GetNumberAsInt.
        double dVal = 0.0;
        if (Size > 0) {
            dVal = static_cast<double>(Data[0]);
        }
        tinygltf::Value numVal(dVal);
        int intVal2 = numVal.GetNumberAsInt();
        (void)intVal2;
    }

    // Additional stress test: Try to parse the input as JSON multiple times with different offsets
    // to trigger different parsing paths in the internal GetInt/GetNumber logic.
    for (size_t offset = 0; offset < Size && offset < 10; ++offset) {
        tinygltf::Model m;
        std::string e, w;
        std::string subStr(reinterpret_cast<const char*>(Data) + offset, Size - offset);
        subStr += '\0'; // Ensure null termination
        ctx.LoadASCIIFromString(&m, &e, &w, 
                                subStr.c_str(), subStr.size(), nullptr);
    }

    return 0;
}