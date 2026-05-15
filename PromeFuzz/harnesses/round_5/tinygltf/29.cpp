// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>

#include "tiny_gltf.h"

// Dummy callback implementations with correct signatures
static bool dummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& path, void* user_data) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filename,
                               void* user_data) {
    return false;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    return false;
}

static bool dummyGetFileSizeInBytes(size_t* size_out,
                                    std::string* err,
                                    const std::string& filename,
                                    void* user_data) {
    return false;
}

static bool dummyURIEncode(const std::string& in, const std::string& base, std::string* out, void* user_data) {
    *out = in;
    return true;
}

static bool dummyURIDecode(const std::string& in, std::string* out, void* user_data) {
    *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test LoadASCIIFromString
    if (Size > 0) {
        // Use the input data as the glTF string
        const char* str = reinterpret_cast<const char*>(Data);
        unsigned int length = static_cast<unsigned int>(Size);
        if (length > Size) length = static_cast<unsigned int>(Size);
        
        // Try with empty base_dir
        loader.LoadASCIIFromString(&model, &err, &warn, str, length, "", tinygltf::REQUIRE_VERSION);
        
        // Try with dummy base_dir
        loader.LoadASCIIFromString(&model, &err, &warn, str, length, "./test", tinygltf::REQUIRE_VERSION);
        
        // Try with different check_sections (use 0 instead of NO_REQUIRE_VERSION)
        loader.LoadASCIIFromString(&model, &err, &warn, str, length, "", 0);
    }
    
    // Test LoadASCIIFromFile
    if (Size > 0) {
        // Write data to dummy file
        std::ofstream outfile("./dummy_file", std::ios::binary);
        if (outfile.is_open()) {
            outfile.write(reinterpret_cast<const char*>(Data), Size);
            outfile.close();
            
            // Try to load from the file
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 0);
            
            // Clean up
            std::remove("./dummy_file");
        }
    }
    
    // Test SetURICallbacks
    {
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.encode = dummyURIEncode;
        uri_callbacks.decode = dummyURIDecode;
        uri_callbacks.user_data = nullptr;
        
        loader.SetURICallbacks(uri_callbacks, &err);
        
        // Test with null decode callback (should fail)
        tinygltf::URICallbacks bad_uri_callbacks;
        bad_uri_callbacks.encode = dummyURIEncode;
        bad_uri_callbacks.decode = nullptr;
        bad_uri_callbacks.user_data = nullptr;
        
        loader.SetURICallbacks(bad_uri_callbacks, &err);
    }
    
    // Test FileExists
    if (Size > 0) {
        // Create a string from the data (limited to 256 chars to avoid very long filenames)
        size_t str_len = Size > 256 ? 256 : Size;
        std::string filename(reinterpret_cast<const char*>(Data), str_len);
        
        // Remove null characters from filename
        filename.erase(std::remove(filename.begin(), filename.end(), '\0'), filename.end());
        
        if (!filename.empty()) {
            tinygltf::FileExists(filename, nullptr);
        }
    }
    
    // Test IsDataURI
    if (Size > 0) {
        // Create a string from the data
        std::string data_str(reinterpret_cast<const char*>(Data), Size);
        tinygltf::IsDataURI(data_str);
        
        // Test with known data URI patterns
        tinygltf::IsDataURI("data:image/png;base64,");
        tinygltf::IsDataURI("data:text/plain,");
        tinygltf::IsDataURI("data:application/octet-stream;base64,");
    }
    
    // Test SetFsCallbacks
    {
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = dummyFileExists;
        fs_callbacks.ExpandFilePath = dummyExpandFilePath;
        fs_callbacks.ReadWholeFile = dummyReadWholeFile;
        fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        fs_callbacks.user_data = nullptr;
        
        loader.SetFsCallbacks(fs_callbacks, &err);
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks bad_fs_callbacks;
        bad_fs_callbacks.FileExists = nullptr;
        bad_fs_callbacks.ExpandFilePath = dummyExpandFilePath;
        bad_fs_callbacks.ReadWholeFile = dummyReadWholeFile;
        bad_fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
        bad_fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        bad_fs_callbacks.user_data = nullptr;
        
        loader.SetFsCallbacks(bad_fs_callbacks, &err);
    }
    
    // Test with various combinations of callbacks set
    if (Size > 0) {
        // Set both URI and FS callbacks
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.encode = dummyURIEncode;
        uri_callbacks.decode = dummyURIDecode;
        uri_callbacks.user_data = nullptr;
        
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = dummyFileExists;
        fs_callbacks.ExpandFilePath = dummyExpandFilePath;
        fs_callbacks.ReadWholeFile = dummyReadWholeFile;
        fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        fs_callbacks.user_data = nullptr;
        
        loader.SetURICallbacks(uri_callbacks, &err);
        loader.SetFsCallbacks(fs_callbacks, &err);
        
        // Try loading again with callbacks set
        const char* str = reinterpret_cast<const char*>(Data);
        unsigned int length = static_cast<unsigned int>(Size);
        if (length > Size) length = static_cast<unsigned int>(Size);
        
        loader.LoadASCIIFromString(&model, &err, &warn, str, length, "", tinygltf::REQUIRE_VERSION);
        
        // Write to file and try loading with callbacks
        std::ofstream outfile("./dummy_file2", std::ios::binary);
        if (outfile.is_open()) {
            outfile.write(reinterpret_cast<const char*>(Data), Size);
            outfile.close();
            
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file2", tinygltf::REQUIRE_VERSION);
            
            // Clean up
            std::remove("./dummy_file2");
        }
    }
    
    return 0;
}