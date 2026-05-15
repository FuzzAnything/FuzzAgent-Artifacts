// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Use a dummy file for file operations
    const std::string dummy_file = "./dummy_file";
    
    // Convert fuzzer input to string for file path operations
    std::string filepath(reinterpret_cast<const char*>(Data), Size);
    
    // Test ExpandFilePath
    {
        std::string expanded = tinygltf::ExpandFilePath(filepath, nullptr);
        // Expanded path should be the same as input (function is disabled)
        // No validation needed
    }
    
    // Test WriteWholeFile with various content
    {
        std::string err;
        std::vector<unsigned char> contents;
        
        // Test with empty contents
        bool write_empty = tinygltf::WriteWholeFile(&err, dummy_file, contents, nullptr);
        
        // Test with non-empty contents
        if (Size > 0) {
            contents.assign(Data, Data + Size);
            bool write_nonempty = tinygltf::WriteWholeFile(&err, dummy_file, contents, nullptr);
        }
        
        // Test with different file paths
        bool write_different = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
    }
    
    // Test FileExists
    {
        // Check if dummy file exists (it might have been created above)
        bool exists_dummy = tinygltf::FileExists(dummy_file, nullptr);
        
        // Check if fuzzer-generated path exists
        bool exists_fuzzed = tinygltf::FileExists(filepath, nullptr);
    }
    
    // Test GetFileSizeInBytes
    {
        std::string err;
        size_t filesize;
        
        // Try to get size of dummy file
        bool size_dummy = tinygltf::GetFileSizeInBytes(&filesize, &err, dummy_file, nullptr);
        
        // Try to get size of fuzzer-generated path
        bool size_fuzzed = tinygltf::GetFileSizeInBytes(&filesize, &err, filepath, nullptr);
    }
    
    // Test ReadWholeFile
    {
        std::string err;
        std::vector<unsigned char> out;
        
        // Try to read dummy file (might exist from WriteWholeFile)
        bool read_dummy = tinygltf::ReadWholeFile(&out, &err, dummy_file, nullptr);
        
        // Try to read fuzzer-generated path
        bool read_fuzzed = tinygltf::ReadWholeFile(&out, &err, filepath, nullptr);
    }
    
    // Test SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        
        // Create callbacks structure
        tinygltf::FsCallbacks callbacks;
        
        // Set all callbacks to non-null functions
        callbacks.FileExists = tinygltf::FileExists;
        callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
        callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
        callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
        callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        // Test setting valid callbacks
        bool set_valid = loader.SetFsCallbacks(callbacks, &err);
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        bool set_null = loader.SetFsCallbacks(null_callbacks, &err);
    }
    
    // Clean up dummy file
    std::remove(dummy_file.c_str());
    
    return 0;
}