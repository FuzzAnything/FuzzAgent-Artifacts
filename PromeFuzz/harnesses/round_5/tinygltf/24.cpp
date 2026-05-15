// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
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
#include <cstddef>
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static bool dummyFileExists(const std::string&, void*) {
    return true;
}

static std::string dummyExpandPath(const std::string& path, void*) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out, std::string* err, const std::string&, void*) {
    if (out) {
        out->push_back(0);
    }
    return true;
}

static bool dummyWriteWholeFile(std::string* err, const std::string& path, const std::vector<unsigned char>& data, void* userdata) {
    return true;
}

static bool dummyGetFileSize(size_t* size, std::string* err, const std::string&, void*) {
    if (size) *size = 1024;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    writeDummyFile(Data, Size);
    
    std::string dummy_path = "./dummy_file";
    
    // Test GetFileSizeInBytes
    {
        size_t file_size = 0;
        std::string err;
        tinygltf::GetFileSizeInBytes(&file_size, &err, dummy_path, nullptr);
    }
    
    // Test FileExists
    {
        tinygltf::FileExists(dummy_path, nullptr);
    }
    
    // Test ExpandFilePath
    {
        tinygltf::ExpandFilePath(dummy_path, nullptr);
    }
    
    // Test TinyGLTF functions
    {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err, warn;
        
        // Test SetFsCallbacks with default callbacks
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandPath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSize;
        callbacks.user_data = nullptr;
        
        loader.SetFsCallbacks(callbacks, &err);
        
        // Test LoadASCIIFromFile with different check_sections values
        loader.LoadASCIIFromFile(&model, &err, &warn, dummy_path, tinygltf::REQUIRE_VERSION);
        loader.LoadASCIIFromFile(&model, &err, &warn, dummy_path, 0);
        
        // Test LoadBinaryFromFile with different check_sections values
        loader.LoadBinaryFromFile(&model, &err, &warn, dummy_path, tinygltf::REQUIRE_VERSION);
        loader.LoadBinaryFromFile(&model, &err, &warn, dummy_path, 0);
        
        // Test SetFsCallbacks with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        loader.SetFsCallbacks(null_callbacks, &err);
    }
    
    // Test with empty model
    {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err, warn;
        
        // Re-set valid callbacks
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandPath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSize;
        callbacks.user_data = nullptr;
        
        loader.SetFsCallbacks(callbacks, nullptr); // Test without error string
        
        loader.LoadASCIIFromFile(&model, nullptr, nullptr, dummy_path); // Test without error/warn strings
        loader.LoadBinaryFromFile(&model, nullptr, nullptr, dummy_path);
    }
    
    return 0;
}