// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

// Dummy file system callbacks for testing - matching the actual function signatures
static bool dummyFileExists(const std::string& filename, void* user_data) {
    (void)user_data;
    std::ifstream file(filename.c_str());
    return file.good();
}

static std::string dummyExpandFilePath(const std::string& filepath, void* user_data) {
    (void)user_data;
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filepath,
                               void* user_data) {
    (void)user_data;
    (void)err;
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return false;
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    return true;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    (void)user_data;
    (void)err;
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

static bool dummyGetFileSizeInBytes(unsigned long* size,
                                    std::string* err,
                                    const std::string& filename,
                                    void* user_data) {
    (void)user_data;
    (void)err;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) return false;
    *size = file.tellg();
    return true;
}

// Dummy URI encode callback
static bool dummyURIEncode(const std::string& in,
                           const std::string& basepath,
                           std::string* out,
                           void* user_data) {
    (void)user_data;
    (void)basepath;
    *out = in;
    return true;
}

// Dummy URI decode callback
static bool dummyURIDecode(const std::string& in,
                           std::string* out,
                           void* user_data) {
    (void)user_data;
    *out = in;
    return true;
}

// Helper function to create dummy image data
static tinygltf::Image createDummyImage(const uint8_t* data, size_t size) {
    tinygltf::Image image;
    image.width = 1;
    image.image.resize(std::min(size, static_cast<size_t>(100)));
    if (size > 0) {
        size_t copy_size = std::min(size, static_cast<size_t>(100));
        std::copy(data, data + copy_size, image.image.begin());
    }
    return image;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Create dummy file with fuzzer data
    const std::string dummy_file = "./dummy_file";
    std::ofstream file(dummy_file, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }

    // Initialize structures with correct function signatures
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = &dummyFileExists;
    fs_callbacks.ExpandFilePath = &dummyExpandFilePath;
    fs_callbacks.ReadWholeFile = &dummyReadWholeFile;
    fs_callbacks.WriteWholeFile = &dummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = &dummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = &dummyURIEncode;
    uri_callbacks.decode = &dummyURIDecode;
    uri_callbacks.user_data = nullptr;

    // Test 1: WriteImageData
    {
        tinygltf::Image image = createDummyImage(Data, Size);
        std::string basepath = ".";
        std::string filename = dummy_file;
        std::string out_uri;
        bool embedImages = (Size % 2 == 0);
        bool as_is = (Size % 3 == 0);
        image.as_is = as_is;
        
        // Test with different parameters - never pass nullptr for basepath or filename
        tinygltf::WriteImageData(&basepath, &filename, &image, embedImages,
                                 &fs_callbacks, &uri_callbacks, &out_uri, nullptr);
        
        // Test with empty strings instead of nullptr
        std::string empty_basepath;
        std::string empty_filename;
        tinygltf::WriteImageData(&empty_basepath, &empty_filename, &image, !embedImages,
                                 &fs_callbacks, &uri_callbacks, &out_uri, nullptr);
    }

    // Test 2: SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        
        // Test with valid callbacks
        loader.SetURICallbacks(uri_callbacks, &err);
        
        // Test with null decode callback (should fail)
        tinygltf::URICallbacks invalid_callbacks = uri_callbacks;
        invalid_callbacks.decode = nullptr;
        loader.SetURICallbacks(invalid_callbacks, &err);
        
        // Test without error string
        loader.SetURICallbacks(uri_callbacks, nullptr);
    }

    // Test 3: DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_str(reinterpret_cast<const char*>(Data), 
                          std::min(Size, static_cast<size_t>(1000)));
        
        // Test with different parameters
        bool checkSize = (Size % 2 == 0);
        size_t reqBytes = (Size % 10) * 100;
        
        tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
        
        // Test with empty string
        std::string empty;
        tinygltf::DecodeDataURI(&out, mime_type, empty, 0, false);
    }

    // Test 4: URIDecode
    {
        std::string in_uri(reinterpret_cast<const char*>(Data), 
                          std::min(Size, static_cast<size_t>(1000)));
        std::string out_uri;
        
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
        
        // Test with empty string
        std::string empty;
        tinygltf::URIDecode(empty, &out_uri, nullptr);
    }

    // Test 5: IsDataURI
    {
        std::string test_str(reinterpret_cast<const char*>(Data), 
                            std::min(Size, static_cast<size_t>(100)));
        
        tinygltf::IsDataURI(test_str);
        
        // Test with common data URI patterns
        tinygltf::IsDataURI("data:image/png;base64,");
        tinygltf::IsDataURI("data:image/jpeg;base64,");
        tinygltf::IsDataURI("data:text/plain;base64,");
        tinygltf::IsDataURI("");
    }

    // Test 6: LoadImageData
    {
        tinygltf::Image image;
        std::string err, warn;
        int req_width = (Size % 100) + 1;
        int req_height = ((Size / 100) % 100) + 1;
        bool as_is = (Size % 4 == 0);
        image.as_is = as_is;
        
        // Test with fuzzer data as image bytes
        tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height,
                               Data, static_cast<int>(std::min(Size, static_cast<size_t>(10000))), nullptr);
        
        // Test with null error/warning strings
        tinygltf::LoadImageData(&image, 1, nullptr, nullptr, 0, 0,
                               Data, static_cast<int>(std::min(Size, static_cast<size_t>(100))), nullptr);
        
        // Test with zero size
        tinygltf::LoadImageData(&image, 2, &err, &warn, 0, 0, nullptr, 0, nullptr);
    }

    // Clean up dummy file
    std::remove(dummy_file.c_str());
    
    return 0;
}