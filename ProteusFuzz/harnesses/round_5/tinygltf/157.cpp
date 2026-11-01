// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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

// Helper to generate a valid JSON string from raw fuzzing input
static std::string MakeValidJson(const uint8_t *Data, size_t Size) {
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    if (Size == 0) {
        return "{}";
    }

    char first = Data[0];
    bool looks_like_json = (first == '{' || first == '[' || first == '"' || 
                            (first >= '0' && first <= '9') || first == '-' || first == 't' || first == 'f' || first == 'n');

    if (looks_like_json) {
        return input;
    } else {
        std::ostringstream oss;
        oss << "{\"data\":\"";
        for (size_t i = 0; i < Size; ++i) {
            unsigned char c = Data[i];
            if (c == '"') oss << "\\\"";
            else if (c == '\\') oss << "\\\\";
            else if (c == '\b') oss << "\\b";
            else if (c == '\f') oss << "\\f";
            else if (c == '\n') oss << "\\n";
            else if (c == '\r') oss << "\\r";
            else if (c == '\t') oss << "\\t";
            else if (c < 32) {
                char buf[8];
                snprintf(buf, sizeof(buf), "\\u%04x", c);
                oss << buf;
            } else {
                oss << c;
            }
        }
        oss << "\"}";
        return oss.str();
    }
}

extern "C" int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        std::string json_str = MakeValidJson(Data, Size);
        
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;
        std::string base_dir = "";
        unsigned int check_sections = tinygltf::REQUIRE_VERSION;

        // Write the JSON string to a temporary file
        const char* temp_file = "./dummy_file";
        FILE* f = fopen(temp_file, "w");
        if (f) {
            fwrite(json_str.c_str(), 1, json_str.size(), f);
            fclose(f);
            
            // Try to load the model from the file
            loader.LoadASCIIFromString(&model, &err, &warn, 
                                      json_str.c_str(), 
                                      static_cast<unsigned int>(json_str.size()),
                                      base_dir,
                                      check_sections);
        }

    } catch (const std::exception& e) {
        // Ignore parsing or logic errors to continue fuzzing
        (void)e;
    } catch (...) {
        // Ignore unknown exceptions
    }

    return 0;
}