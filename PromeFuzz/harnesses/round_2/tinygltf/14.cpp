// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static bool customURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri + "_custom";
        return true;
    }
    return false;
}

static bool customURIEncode(const std::string &in_uri, const std::string &base_path, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri + "_encoded";
        return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // 1. Test URIDecode
    if (Size > 0) {
        std::string in_uri(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)1000));
        std::string out_uri;
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // 2. Test SetURICallbacks
    tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks callbacks;
    callbacks.decode = customURIDecode;
    callbacks.encode = customURIEncode;
    callbacks.user_data = nullptr;
    
    std::string err;
    loader.SetURICallbacks(callbacks, &err);
    
    // Test with null decode callback
    tinygltf::URICallbacks bad_callbacks;
    bad_callbacks.decode = nullptr;
    bad_callbacks.encode = customURIEncode;
    bad_callbacks.user_data = nullptr;
    loader.SetURICallbacks(bad_callbacks, &err);

    // 3. Test DecodeDataURI
    if (Size > 10) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_string(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)500));
        
        // Test with different parameters
        tinygltf::DecodeDataURI(&out, mime_type, in_string, 0, false);
        tinygltf::DecodeDataURI(&out, mime_type, in_string, 100, true);
        tinygltf::DecodeDataURI(&out, mime_type, "data:application/octet-stream;base64,SGVsbG8=", 5, true);
    }

    // 4. Test LoadASCIIFromString
    if (Size > 20) {
        tinygltf::Model model1;
        std::string warn1;
        std::string err1;
        
        const char* str = reinterpret_cast<const char*>(Data);
        unsigned int length = static_cast<unsigned int>(std::min(Size, (size_t)10000));
        
        loader.LoadASCIIFromString(&model1, &err1, &warn1, str, length, "", tinygltf::REQUIRE_VERSION);
        loader.LoadASCIIFromString(&model1, &err1, &warn1, str, length, "./test_dir", tinygltf::REQUIRE_VERSION);
        
        // Test with different check_sections values
        loader.LoadASCIIFromString(&model1, &err1, &warn1, str, length, "", 0);
        loader.LoadASCIIFromString(&model1, &err1, &warn1, str, length, "", tinygltf::REQUIRE_VERSION);
    }

    // 5. Test LoadASCIIFromFile
    {
        tinygltf::Model model2;
        std::string err2;
        std::string warn2;
        
        // Write data to dummy file
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)10000));
            file.close();
            
            loader.LoadASCIIFromFile(&model2, &err2, &warn2, "./dummy_file", tinygltf::REQUIRE_VERSION);
            loader.LoadASCIIFromFile(&model2, &err2, &warn2, "./dummy_file", 0);
            
            // Clean up
            std::remove("./dummy_file");
        }
    }

    // 6. Test LoadBinaryFromMemory
    if (Size > 20) {
        tinygltf::Model model3;
        std::string err3;
        std::string warn3;
        
        // Test with different base_dir values
        loader.LoadBinaryFromMemory(&model3, &err3, &warn3, Data, static_cast<unsigned int>(std::min(Size, (size_t)10000)), "", tinygltf::REQUIRE_VERSION);
        loader.LoadBinaryFromMemory(&model3, &err3, &warn3, Data, static_cast<unsigned int>(std::min(Size, (size_t)10000)), "./test_dir", tinygltf::REQUIRE_VERSION);
        
        // Test with different check_sections values
        loader.LoadBinaryFromMemory(&model3, &err3, &warn3, Data, static_cast<unsigned int>(std::min(Size, (size_t)10000)), "", 0);
        loader.LoadBinaryFromMemory(&model3, &err3, &warn3, Data, static_cast<unsigned int>(std::min(Size, (size_t)10000)), "", tinygltf::REQUIRE_VERSION);
    }

    return 0;
}