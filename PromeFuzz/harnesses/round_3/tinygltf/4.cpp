// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
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
#include <fstream>
#include <string>
#include <vector>

static std::string dummy_file = "./dummy_file";

static tinygltf::URICallbacks create_dummy_uri_callbacks() {
    tinygltf::URICallbacks callbacks;
    // Use nullptr for callbacks since we don't need actual URI handling
    callbacks.encode = nullptr;
    callbacks.decode = nullptr;
    callbacks.user_data = nullptr;
    return callbacks;
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file(dummy_file, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void cleanup_dummy_file() {
    std::remove(dummy_file.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Initialize required objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test 1: LoadBinaryFromMemory
    if (Size > 20) {
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), ".");
    }
    
    // Test 2: LoadASCIIFromFile
    write_dummy_file(Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
    cleanup_dummy_file();
    
    // Test 3: SetURICallbacks
    auto callbacks = create_dummy_uri_callbacks();
    loader.SetURICallbacks(callbacks, &err);
    
    // Test 4: Accessor::ByteStride
    tinygltf::Accessor accessor;
    tinygltf::BufferView bufferView;
    // Set some values to avoid default -1
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    // Use fuzz data to set componentType and type
    if (Size >= 2) {
        accessor.componentType = Data[0] % 6 + 5120; // Valid component types: 5120-5125
        
        // Set type as integer based on fuzz data
        // TINYGLTF_TYPE_SCALAR = 0, VEC2 = 1, VEC3 = 2, VEC4 = 3, MAT2 = 4, MAT3 = 5, MAT4 = 6
        int type_index = Data[1] % 7;
        accessor.type = type_index;
    }
    int stride = accessor.ByteStride(bufferView);
    (void)stride; // Prevent unused variable warning
    
    // Test 5: LoadBinaryFromFile
    write_dummy_file(Data, Size);
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file);
    cleanup_dummy_file();
    
    // Test 6: LoadASCIIFromString
    if (Size > 0) {
        // Ensure null termination for string
        std::vector<char> str_data(Data, Data + Size);
        str_data.push_back('\0');
        loader.LoadASCIIFromString(&model, &err, &warn, str_data.data(), 
                                   static_cast<unsigned int>(Size), ".");
    }
    
    // Additional state exploration: Test with different check_sections values
    if (Size > 20) {
        unsigned int check_sections = Size % 3; // 0, 1, or 2
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, 
                                   static_cast<unsigned int>(Size), ".", check_sections);
    }
    
    // Test with empty base_dir
    if (Size > 0) {
        std::vector<char> str_data(Data, Data + Size);
        str_data.push_back('\0');
        loader.LoadASCIIFromString(&model, &err, &warn, str_data.data(), 
                                   static_cast<unsigned int>(Size), "");
    }
    
    return 0;
}