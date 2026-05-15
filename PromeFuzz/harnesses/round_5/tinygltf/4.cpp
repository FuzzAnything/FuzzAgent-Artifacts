// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
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
#include <string>
#include <vector>

// Dummy file path
static const char* DUMMY_FILE = "./dummy_file";

// Dummy filesystem callbacks with correct signatures
static bool dummy_file_exists(const std::string& filename, void* user_data) {
    return filename == DUMMY_FILE;
}

static std::string dummy_expand_path(const std::string& filepath, void* user_data) {
    return filepath;
}

static bool dummy_read_file(std::vector<unsigned char>* out, std::string* err,
                           const std::string& filepath, void* user_data) {
    // Always fail for fuzzing
    return false;
}

static bool dummy_write_file(std::string* out_filename, const std::string& filename,
                            const std::vector<unsigned char>& contents,
                            void* user_data) {
    // Always succeed for fuzzing
    if (out_filename) {
        *out_filename = filename;
    }
    return true;
}

static bool dummy_get_file_size(unsigned long* filesize, std::string* err,
                               const std::string& filename, void* user_data) {
    // Always succeed for fuzzing
    if (filesize) {
        *filesize = 0;
    }
    return true;
}

// Dummy URI callbacks with correct signatures
static bool dummy_uri_encode(const std::string& in_uri, const std::string& base_dir,
                            std::string* out_uri, void* user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummy_uri_decode(const std::string& in_uri, std::string* out_uri,
                            void* user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize fuzzer input as string
    std::string fuzz_data(reinterpret_cast<const char*>(Data), Size);
    
    // Initialize tinygltf objects
    tinygltf::Image image;
    tinygltf::TinyGLTF loader;
    
    // Set up filesystem callbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = dummy_file_exists;
    fs_callbacks.ExpandFilePath = dummy_expand_path;
    fs_callbacks.ReadWholeFile = dummy_read_file;
    fs_callbacks.WriteWholeFile = dummy_write_file;
    fs_callbacks.GetFileSizeInBytes = dummy_get_file_size;
    fs_callbacks.user_data = nullptr;
    
    // Set up URI callbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = dummy_uri_encode;
    uri_callbacks.decode = dummy_uri_decode;
    uri_callbacks.user_data = nullptr;
    
    // Test SetPreserveImageChannels and GetPreserveImageChannels
    if (Size > 0) {
        bool preserve = (Data[0] & 0x01) != 0;
        loader.SetPreserveImageChannels(preserve);
        bool get_preserve = loader.GetPreserveImageChannels();
        // The getter should return what we just set
        (void)get_preserve; // Suppress unused variable warning
    }
    
    // Test SetImagesAsIs and GetImagesAsIs
    if (Size > 1) {
        bool as_is = (Data[1] & 0x01) != 0;
        loader.SetImagesAsIs(as_is);
        bool get_as_is = loader.GetImagesAsIs();
        // The getter should return what we just set
        (void)get_as_is; // Suppress unused variable warning
    }
    
    // Test WriteImageData
    if (Size > 2) {
        std::string basepath = "./";
        std::string filename = "test_image";
        std::string out_uri;
        
        // Initialize image with fuzzer data
        image.width = Size > 3 ? static_cast<int>(Data[3]) : 1;
        image.height = Size > 4 ? static_cast<int>(Data[4]) : 1;
        image.as_is = Size > 5 ? ((Data[5] & 0x01) != 0) : false;
        
        // Fill image data with remaining fuzzer input
        size_t data_start = 6;
        if (data_start < Size) {
            image.image.assign(Data + data_start, Data + Size);
        } else {
            // Use some default data if fuzzer input is too small
            image.image = {0, 1, 2, 3};
        }
        
        bool embed_images = Size > 6 ? ((Data[6] & 0x01) != 0) : false;
        
        // Write dummy file for testing
        std::vector<unsigned char> dummy_content = {'t', 'e', 's', 't'};
        std::string dummy_filename;
        dummy_write_file(&dummy_filename, DUMMY_FILE, dummy_content, nullptr);
        
        // Call WriteImageData
        bool write_result = tinygltf::WriteImageData(
            &basepath, &filename, &image, embed_images,
            &fs_callbacks, &uri_callbacks, &out_uri, nullptr
        );
        (void)write_result; // Suppress unused variable warning
    }
    
    // Test LoadImageData
    if (Size > 7) {
        tinygltf::Image load_image;
        std::string err;
        std::string warn;
        
        int req_width = Size > 8 ? static_cast<int>(Data[8]) : 0;
        int req_height = Size > 9 ? static_cast<int>(Data[9]) : 0;
        
        // Use fuzzer data as image bytes
        const unsigned char* bytes = Data;
        int bytes_size = static_cast<int>(Size);
        
        // Adjust if we need to skip some bytes for other parameters
        size_t bytes_start = 10;
        if (bytes_start < Size) {
            bytes = Data + bytes_start;
            bytes_size = static_cast<int>(Size - bytes_start);
        }
        
        // Call LoadImageData
        bool load_result = tinygltf::LoadImageData(
            &load_image, 0, &err, &warn,
            req_width, req_height, bytes, bytes_size, nullptr
        );
        (void)load_result; // Suppress unused variable warning
    }
    
    return 0;
}