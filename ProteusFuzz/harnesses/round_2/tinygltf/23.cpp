// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>
#include "tiny_gltf.h"

// Helper to generate random string from input buffer
static std::string GenerateString(const uint8_t *Data, size_t Size, size_t offset, size_t max_len) {
    if (offset >= Size) return "";
    size_t len = (Data[offset] % 256);
    if (len == 0) len = 1;
    if (len > max_len) len = max_len;
    if (offset + len > Size) len = Size - offset;
    
    std::string result;
    result.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        char c = static_cast<char>(Data[offset + i]);
        // Avoid null terminators in middle of strings for safety
        if (c == '\0') c = ' ';
        result += c;
    }
    return result;
}

// Helper to generate random vector from input buffer
static std::vector<unsigned char> GenerateVector(const uint8_t *Data, size_t Size, size_t offset, size_t max_len) {
    if (offset >= Size) return {};
    size_t len = (Data[offset] % 256);
    if (len == 0) len = 1;
    if (len > max_len) len = max_len;
    if (offset + len > Size) len = Size - offset;
    
    std::vector<unsigned char> result;
    result.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        result.push_back(Data[offset + i]);
    }
    return result;
}

// Dummy callback implementations matching tinygltf signatures exactly
static bool DummyFileExists(const std::string &path, void *user_data) { 
    (void)path; (void)user_data;
    return false; 
}

static std::string DummyExpandFilePath(const std::string &path, void *user_data) { 
    (void)path; (void)user_data;
    return path; 
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data) { 
    (void)out; (void)err; (void)filename; (void)user_data;
    return false; 
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data) { 
    (void)err; (void)filename; (void)contents; (void)user_data;
    return true; 
}

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) { 
    if (size) *size = 0;
    (void)err; (void)filename; (void)user_data;
    return false; 
}

static bool DummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    (void)user_data;
    if (!out_uri) return false;
    *out_uri = in_uri;
    return true;
}

static bool DummyURIEncode(const std::string &in_uri, const std::string &base, std::string *out_uri, void *user_data) {
    (void)in_uri; (void)base; (void)user_data;
    if (out_uri) *out_uri = in_uri;
    return true;
}

static bool DummyLoadImageData(std::vector<unsigned char> *out, std::string *ext, std::string *mime_type,
                               int *width, int *height, const std::string &filename, const std::string &base_dir, void *user_data) { 
    (void)out; (void)ext; (void)mime_type; (void)width; (void)height; (void)filename; (void)base_dir; (void)user_data;
    return false; 
}

static bool DummyWriteImageData(const std::string *basepath, const std::string *filename,
                                const tinygltf::Image *image, bool embedImages,
                                const tinygltf::FsCallbacks *fs_cb, const tinygltf::URICallbacks *uri_cb,
                                std::string *out_uri, void *user_data) { 
    (void)basepath; (void)filename; (void)image; (void)embedImages; (void)fs_cb; (void)uri_cb; (void)user_data;
    if (out_uri) *out_uri = "data:dummy";
    return true; 
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Seed offset for different test cases
    size_t offset = 0;
    int test_case = (Size > 0) ? (Data[0] % 4) : 0;

    // Common setup
    tinygltf::Image image;
    std::string baseDir = "./dummy_file";
    std::string out_uri;
    std::string err, warn;
    std::string mime_type;
    std::vector<unsigned char> decoded_out;
    
    // Initialize callbacks with nulls or dummy implementations
    tinygltf::FsCallbacks fs_cb = {};
    tinygltf::URICallbacks uri_cb = {};
    
    // Set FsCallbacks with correct signatures
    fs_cb.FileExists = DummyFileExists;
    fs_cb.ExpandFilePath = DummyExpandFilePath;
    fs_cb.ReadWholeFile = DummyReadWholeFile;
    fs_cb.WriteWholeFile = DummyWriteWholeFile;
    fs_cb.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_cb.user_data = nullptr;

    // Set URICallbacks with correct signatures
    uri_cb.encode = DummyURIEncode;
    uri_cb.decode = DummyURIDecode;
    uri_cb.user_data = nullptr;

    switch (test_case) {
        case 0: {
            // Test IsDataURI
            std::string test_str = GenerateString(Data, Size, offset, 1024);
            // Ensure we test both valid and invalid patterns
            if (test_str.empty()) test_str = "data:text/plain;base64,SGVsbG8=";
            bool result = tinygltf::IsDataURI(test_str);
            (void)result; 
            break;
        }
        case 1: {
            // Test URIDecode
            std::string input_uri = GenerateString(Data, Size, offset, 1024);
            std::string output_uri;
            bool result = tinygltf::URIDecode(input_uri, &output_uri, nullptr);
            (void)result;
            break;
        }
        case 2: {
            // Test DecodeDataURI
            std::string data_uri = GenerateString(Data, Size, offset, 2048);
            if (data_uri.empty()) {
                data_uri = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNk+M9QDwADhgGAWjR9awAAAABJRU5ErkJggg==";
            }
            size_t reqBytes = (Size > offset + 1) ? (Data[offset+1] % 100) : 0;
            bool checkSize = (Size > offset + 2) ? (Data[offset+2] % 2) : true;
            bool result = tinygltf::DecodeDataURI(&decoded_out, mime_type, data_uri, reqBytes, checkSize);
            (void)result;
            break;
        }
        case 3: {
            // Test WriteImageData
            image.name = GenerateString(Data, Size, offset, 64);
            image.image = GenerateVector(Data, Size, offset, 1024);
            bool embedImages = (Size > offset) ? (Data[offset] % 2) : true;
            
            bool result = tinygltf::WriteImageData(
                &baseDir, &image.name, &image, embedImages,
                &fs_cb, &uri_cb, &out_uri, nullptr
            );
            (void)result;
            break;
        }
    }

    return 0;
}