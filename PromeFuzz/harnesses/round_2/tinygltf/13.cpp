// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
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
#include <fstream>
#include <string>
#include <vector>

static bool dummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummyURIEncode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create dummy file for FileExists testing
    std::ofstream dummy_file("./dummy_file", std::ios::binary);
    if (dummy_file.is_open()) {
        dummy_file.write(reinterpret_cast<const char*>(Data), Size);
        dummy_file.close();
    }

    // Convert input to string for various functions
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test URIDecode
    std::string decoded_uri;
    tinygltf::URIDecode(input_str, &decoded_uri, nullptr);
    
    // Test DecodeDataURI with various parameters
    std::vector<unsigned char> out_data;
    std::string mime_type;
    
    // Test with different combinations of reqBytes and checkSize
    size_t reqBytes = Size > 0 ? Data[0] : 0;
    bool checkSize = Size > 1 ? (Data[1] % 2 == 0) : false;
    tinygltf::DecodeDataURI(&out_data, mime_type, input_str, reqBytes, checkSize);
    
    // Test with zero reqBytes and checkSize false
    tinygltf::DecodeDataURI(&out_data, mime_type, input_str, 0, false);
    
    // Test with large reqBytes and checkSize true
    tinygltf::DecodeDataURI(&out_data, mime_type, input_str, 1024, true);
    
    // Test FileExists
    tinygltf::FileExists("./dummy_file", nullptr);
    tinygltf::FileExists(input_str, nullptr);
    
    // Test IsDataURI
    tinygltf::IsDataURI(input_str);
    
    // Test LoadASCIIFromString
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test with default parameters
    loader.LoadASCIIFromString(&model, &err, &warn, input_str.c_str(), 
                              static_cast<unsigned int>(input_str.size()), ".", 0);
    
    // Test with different check_sections values
    if (Size > 2) {
        unsigned int check_sections = static_cast<unsigned int>(Data[2]) % 4;
        loader.LoadASCIIFromString(&model, &err, &warn, input_str.c_str(),
                                  static_cast<unsigned int>(input_str.size()), ".", check_sections);
    }
    
    // Test SetURICallbacks
    tinygltf::URICallbacks callbacks;
    callbacks.encode = nullptr;
    callbacks.decode = nullptr;
    callbacks.user_data = nullptr;
    
    std::string set_err;
    loader.SetURICallbacks(callbacks, &set_err);
    
    // Test with null error pointer
    loader.SetURICallbacks(callbacks, nullptr);
    
    // Test with null decode callback (should fail)
    tinygltf::URICallbacks bad_callbacks;
    bad_callbacks.encode = nullptr;
    bad_callbacks.decode = nullptr;
    bad_callbacks.user_data = nullptr;
    loader.SetURICallbacks(bad_callbacks, &set_err);
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}