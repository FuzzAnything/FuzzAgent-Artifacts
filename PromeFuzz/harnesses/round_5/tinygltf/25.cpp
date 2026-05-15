// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
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

// Static helper functions with correct signatures
static bool dummyFileExists(const std::string &, void *) {
    return false;
}

static std::string dummyExpandFilePath(const std::string &, void *) {
    return "";
}

static bool dummyReadWholeFile(std::vector<unsigned char> *, std::string *, const std::string &, void *) {
    return false;
}

static bool dummyWriteWholeFile(std::string *, const std::string &, const std::vector<unsigned char> &, void *) {
    return false;
}

static bool dummyGetFileSizeInBytes(size_t *, std::string *, const std::string &, void *) {
    return false;
}

static bool dummyURIEncode(const std::string &, const std::string &, std::string *, void *) {
    return false;
}

static bool dummyURIDecode(const std::string &, std::string *, void *) {
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Initialize objects
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Use first byte to select function to test
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
        Data++;
        Size--;
    }
    
    // Create string from remaining data
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test different functions based on selector
    switch (selector % 6) {
        case 0: {
            // Test LoadASCIIFromString
            loader.LoadASCIIFromString(&model, &err, &warn, input_str.c_str(), 
                                      static_cast<unsigned int>(input_str.size()), 
                                      "./", tinygltf::REQUIRE_VERSION);
            break;
        }
        case 1: {
            // Test LoadASCIIFromFile
            // Write data to dummy file
            std::ofstream outfile("./dummy_file", std::ios::binary);
            if (outfile.is_open()) {
                outfile.write(input_str.c_str(), input_str.size());
                outfile.close();
                loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 
                                        tinygltf::REQUIRE_VERSION);
                // Clean up file
                std::remove("./dummy_file");
            }
            break;
        }
        case 2: {
            // Test SetURICallbacks
            tinygltf::URICallbacks uri_callbacks;
            uri_callbacks.encode = dummyURIEncode;
            uri_callbacks.decode = dummyURIDecode;
            uri_callbacks.user_data = nullptr;
            loader.SetURICallbacks(uri_callbacks, &err);
            break;
        }
        case 3: {
            // Test FileExists
            tinygltf::FileExists(input_str, nullptr);
            break;
        }
        case 4: {
            // Test IsDataURI
            tinygltf::IsDataURI(input_str);
            break;
        }
        case 5: {
            // Test SetFsCallbacks
            tinygltf::FsCallbacks fs_callbacks;
            fs_callbacks.FileExists = dummyFileExists;
            fs_callbacks.ExpandFilePath = dummyExpandFilePath;
            fs_callbacks.ReadWholeFile = dummyReadWholeFile;
            fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
            fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
            fs_callbacks.user_data = nullptr;
            loader.SetFsCallbacks(fs_callbacks, &err);
            break;
        }
    }
    
    // Additional test: try LoadASCIIFromString with different base_dir
    if (Size > 1 && (selector % 3 == 0)) {
        loader.LoadASCIIFromString(&model, &err, &warn, input_str.c_str(),
                                  static_cast<unsigned int>(input_str.size()),
                                  "/tmp/", tinygltf::REQUIRE_VERSION);
    }
    
    // Additional test: try LoadASCIIFromString with different check_sections
    if (Size > 2 && (selector % 5 == 0)) {
        loader.LoadASCIIFromString(&model, &err, &warn, input_str.c_str(),
                                  static_cast<unsigned int>(input_str.size()),
                                  "./", 0); // Use 0 instead of NO_REQUIRE_VERSION
    }
    
    return 0;
}