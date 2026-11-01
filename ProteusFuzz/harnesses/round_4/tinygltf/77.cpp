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

// Include tinygltf header
#include "tiny_gltf.h"

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // LoadASCIIFromString requires 6 arguments: model, err, warn, str, length, base_dir, check_sections
    // We provide an empty base_dir and default check_sections
    bool success = ctx.LoadASCIIFromString(&model, &err, &warn, input_str.c_str(), 
                                           static_cast<unsigned int>(input_str.size()), 
                                           "", 
                                           tinygltf::REQUIRE_VERSION);
    
    // We don't care about the return value for fuzzing, just that it doesn't crash.
    (void)success;
    
    return 0;
}