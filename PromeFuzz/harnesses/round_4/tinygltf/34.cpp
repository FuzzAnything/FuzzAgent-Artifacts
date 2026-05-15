// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
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
#include <vector>

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool DummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string DummyExpandFilePath(const std::string& filepath, void* user_data) {
    return filepath;
}

static bool DummyReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                               const std::string& filename, void* user_data) {
    out->push_back('{');
    out->push_back('}');
    return true;
}

static bool DummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    return true;
}

static bool DummyGetFileSize(size_t* size, std::string* err,
                             const std::string& filename, void* user_data) {
    *size = 0;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize objects
    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF loader;

    // Use first byte to select different code paths
    uint8_t selector = Data[0];
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    // Test LoadASCIIFromString
    if (fuzz_size > 0) {
        std::string gltf_str(reinterpret_cast<const char*>(fuzz_data), 
                             std::min(fuzz_size, (size_t)1000)); // Limit size
        loader.LoadASCIIFromString(&model, &err, &warn, 
                                   gltf_str.c_str(), 
                                   static_cast<unsigned int>(gltf_str.size()),
                                   "./",
                                   tinygltf::REQUIRE_VERSION);
    }

    // Test LoadASCIIFromFile
    WriteDummyFile(fuzz_data, fuzz_size);
    loader.LoadASCIIFromFile(&model, &err, &warn, 
                             "./dummy_file",
                             tinygltf::REQUIRE_VERSION);

    // Test GetMaxExternalFileSize and SetMaxExternalFileSize
    size_t current_max = loader.GetMaxExternalFileSize();
    if (fuzz_size > 0) {
        size_t new_max = static_cast<size_t>(fuzz_data[0]) * 1024;
        loader.SetMaxExternalFileSize(new_max);
        size_t updated_max = loader.GetMaxExternalFileSize();
        // Restore original
        loader.SetMaxExternalFileSize(current_max);
    }

    // Test DecodeDataURI
    if (fuzz_size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string data_uri(reinterpret_cast<const char*>(fuzz_data), 
                            std::min(fuzz_size, (size_t)1000));
        bool checkSize = (selector & 0x01) != 0;
        size_t reqBytes = (selector & 0x02) ? fuzz_size : 0;
        tinygltf::DecodeDataURI(&out, mime_type, data_uri, reqBytes, checkSize);
    }

    // Test SetFsCallbacks
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = DummyFileExists;
    callbacks.ExpandFilePath = DummyExpandFilePath;
    callbacks.ReadWholeFile = DummyReadWholeFile;
    callbacks.WriteWholeFile = DummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = DummyGetFileSize;
    callbacks.user_data = nullptr;

    std::string set_err;
    bool set_result = loader.SetFsCallbacks(callbacks, &set_err);

    // Test with null callbacks (should fail)
    tinygltf::FsCallbacks null_callbacks = {};
    loader.SetFsCallbacks(null_callbacks, &set_err);

    // Clean up
    std::remove("./dummy_file");

    return 0;
}