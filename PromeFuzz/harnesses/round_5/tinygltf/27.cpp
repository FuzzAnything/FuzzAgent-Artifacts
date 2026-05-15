// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
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

static bool dummyFileExists(const std::string&, void*) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& path, void*) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filename,
                               void*) {
    out->clear();
    return true;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void*) {
    return true;
}

static bool dummyGetFileSizeInBytes(size_t* size,
                                    std::string* err,
                                    const std::string& filename,
                                    void*) {
    *size = 0;
    return true;
}

static bool dummyURIEncode(const std::string& in,
                           const std::string&,
                           std::string* out,
                           void*) {
    *out = in;
    return true;
}

static bool dummyURIDecode(const std::string& in,
                           std::string* out,
                           void*) {
    *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write data to dummy file for file-based operations
    std::ofstream outfile("./dummy_file", std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
    }

    // Test FileExists
    tinygltf::FileExists("./dummy_file", nullptr);

    // Test IsDataURI
    std::string data_str(reinterpret_cast<const char*>(Data), Size);
    tinygltf::IsDataURI(data_str);

    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Test LoadASCIIFromString
    loader.LoadASCIIFromString(&model, &err, &warn,
                               reinterpret_cast<const char*>(Data),
                               static_cast<unsigned int>(Size),
                               "./",
                               tinygltf::REQUIRE_VERSION);

    // Test LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn,
                             "./dummy_file",
                             tinygltf::REQUIRE_VERSION);

    // Test SetFsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = dummyFileExists;
    fs_callbacks.ExpandFilePath = dummyExpandFilePath;
    fs_callbacks.ReadWholeFile = dummyReadWholeFile;
    fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;
    loader.SetFsCallbacks(fs_callbacks, &err);

    // Test SetURICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = dummyURIEncode;
    uri_callbacks.decode = dummyURIDecode;
    uri_callbacks.user_data = nullptr;
    loader.SetURICallbacks(uri_callbacks, &err);

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}