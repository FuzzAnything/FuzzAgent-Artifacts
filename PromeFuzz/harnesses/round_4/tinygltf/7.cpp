// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
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
        *out_uri = in_uri;
        return true;
    }
    return false;
}

static bool customURIEncode(const std::string &in_uri, const std::string &relative_path, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri + relative_path;
        return true;
    }
    return false;
}

static void writeDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test SetURICallbacks
    tinygltf::URICallbacks callbacks;
    callbacks.decode = customURIDecode;
    callbacks.encode = customURIEncode;
    callbacks.user_data = nullptr;
    
    bool setCallbackResult = loader.SetURICallbacks(callbacks, &err);
    (void)setCallbackResult;
    
    // Test with null decode callback (should fail)
    tinygltf::URICallbacks badCallbacks;
    badCallbacks.decode = nullptr;
    badCallbacks.encode = customURIEncode;
    badCallbacks.user_data = nullptr;
    loader.SetURICallbacks(badCallbacks, &err);

    // Test LoadASCIIFromString
    if (Size > 0) {
        std::string inputStr(reinterpret_cast<const char*>(Data), Size);
        std::string baseDir = ".";
        unsigned int checkSections = tinygltf::REQUIRE_VERSION;
        
        bool loadStringResult = loader.LoadASCIIFromString(&model, &err, &warn, 
                                                          inputStr.c_str(), 
                                                          static_cast<unsigned int>(inputStr.length()),
                                                          baseDir, checkSections);
        (void)loadStringResult;
    }

    // Test LoadASCIIFromFile
    writeDummyFile(Data, Size);
    bool loadFileResult = loader.LoadASCIIFromFile(&model, &err, &warn, 
                                                  "./dummy_file", 
                                                  tinygltf::REQUIRE_VERSION);
    (void)loadFileResult;

    // Test URIDecode
    if (Size > 0) {
        std::string uriInput(reinterpret_cast<const char*>(Data), Size);
        std::string uriOutput;
        bool decodeResult = tinygltf::URIDecode(uriInput, &uriOutput, nullptr);
        (void)decodeResult;
    }

    // Test DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> outData;
        std::string mimeType;
        std::string dataURI(reinterpret_cast<const char*>(Data), Size);
        
        // Test with different combinations of parameters
        bool decodeDataResult1 = tinygltf::DecodeDataURI(&outData, mimeType, 
                                                        dataURI, 0, false);
        (void)decodeDataResult1;
        
        bool decodeDataResult2 = tinygltf::DecodeDataURI(&outData, mimeType,
                                                        dataURI, Size, true);
        (void)decodeDataResult2;
    }

    // Test LoadBinaryFromMemory
    if (Size > 20) { // Minimum size for binary glTF header
        bool loadBinaryResult = loader.LoadBinaryFromMemory(&model, &err, &warn,
                                                           Data,
                                                           static_cast<unsigned int>(Size),
                                                           ".",
                                                           tinygltf::REQUIRE_VERSION);
        (void)loadBinaryResult;
    }

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}