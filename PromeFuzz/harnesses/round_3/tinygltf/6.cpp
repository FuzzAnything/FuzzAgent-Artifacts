// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
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
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static tinygltf::URICallbacks createURICallbacks(const uint8_t *data, size_t size) {
    tinygltf::URICallbacks callbacks;
    // Use the actual function types expected by tinygltf
    callbacks.decode = [](const std::string &uri, std::string *out, void *user_data) -> bool {
        if (out) {
            *out = uri;
        }
        return true;
    };
    callbacks.encode = [](const std::string &uri, const std::string &relative_path,
                         std::string *out, void *user_data) -> bool {
        if (out) {
            *out = uri;
        }
        return true;
    };
    callbacks.user_data = nullptr;
    return callbacks;
}

static tinygltf::BufferView createBufferView(const uint8_t *data, size_t size) {
    tinygltf::BufferView bufferView;
    if (size > 0) {
        bufferView.byteOffset = static_cast<size_t>(data[0]) % 1024;
        bufferView.byteLength = static_cast<size_t>(data[0]) % 4096;
        bufferView.byteStride = static_cast<int>(data[0]) % 256;
        bufferView.target = static_cast<int>(data[0]) % 5;
    }
    return bufferView;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for file-based functions
    writeDummyFile(Data, Size);
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // 1. Test LoadBinaryFromMemory
    if (Size > 20) { // Minimum required size
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, 
                                   static_cast<unsigned int>(Size), "", 
                                   tinygltf::REQUIRE_VERSION);
    }
    
    // 2. Test LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 
                            tinygltf::REQUIRE_VERSION);
    
    // 3. Test SetURICallbacks
    auto callbacks = createURICallbacks(Data, Size);
    loader.SetURICallbacks(callbacks, &err);
    
    // 4. Test Accessor::ByteStride
    tinygltf::Accessor accessor;
    if (Size > 0) {
        // Set componentType to a valid glTF component type
        accessor.componentType = static_cast<int>(Data[0]) % 7 + 5120; // 5120-5126
        // Set type to a valid glTF accessor type (0-6)
        accessor.type = static_cast<int>(Data[0]) % 7;
        
        auto bufferView = createBufferView(Data, Size);
        accessor.ByteStride(bufferView);
    }
    
    // 5. Test LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 
                             tinygltf::REQUIRE_VERSION);
    
    // 6. Test LoadASCIIFromString
    if (Size > 0) {
        std::string asciiStr(reinterpret_cast<const char*>(Data), Size);
        loader.LoadASCIIFromString(&model, &err, &warn, asciiStr.c_str(), 
                                  static_cast<unsigned int>(Size), "");
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}