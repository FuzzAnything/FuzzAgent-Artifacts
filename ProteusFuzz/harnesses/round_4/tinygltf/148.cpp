// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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
#include <fstream>
#include "tiny_gltf.h"

// Helper function to create dummy file content
static void WriteDummyFile(const std::string& filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Dummy filesystem callbacks matching tinygltf signatures exactly
// Note: The signatures in tiny_gltf.h are:
// FileExistsFunction: bool(const std::string&, void*)
// ExpandFilePathFunction: std::string(const std::string&, void*)
// ReadWholeFileFunction: bool(std::vector<unsigned char>*, std::string*, const std::string&, void*)
// WriteWholeFileFunction: bool(std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// GetFileSizeFunction: bool(size_t*, const std::string&, void*)

static bool DummyFileExists(const std::string& filepath, void* user_data) { 
    (void)filepath; (void)user_data;
    return false; 
}

static std::string DummyExpandFilePath(const std::string& filepath, void* user_data) { 
    (void)filepath; (void)user_data;
    return ""; 
}

static bool DummyReadWholeFile(std::vector<unsigned char>* out, std::string* err, const std::string& filepath, void* user_data) { 
    (void)out; (void)err; (void)filepath; (void)user_data;
    return false; 
}

static bool DummyWriteWholeFile(std::string* err, const std::string& filepath, const std::vector<unsigned char>& contents, void* user_data) { 
    (void)err; (void)filepath; (void)contents; (void)user_data;
    return true; 
}

// Corrected signature for GetFileSizeFunction based on error message
// The error indicates: function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>
// This implies the signature is: bool(size_t*, std::string*, const std::string&, void*)
// Wait, the error says: "no viable conversion from 'bool (size_t *, const std::string &, void *)' to 'GetFileSizeFunction'"
// And the target is: 'function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'
// So the target signature is: bool(size_t*, std::string*, const std::string&, void*)
// My original DummyGetFileSizeInBytes was: bool(size_t*, const std::string&, void*)
// I need to add a std::string* parameter.

static bool DummyGetFileSizeInBytes(size_t* size, std::string* err, const std::string& filepath, void* user_data) { 
    (void)size; (void)err; (void)filepath; (void)user_data;
    return false; 
}

// Dummy URI callbacks matching tinygltf signatures exactly
// Note: The signatures in tiny_gltf.h are:
// URIEncodeFunction: bool(const std::string&, const std::string&, std::string*, void*)
// URIDecodeFunction: bool(const std::string&, std::string*, void*)

static bool DummyURIEncode(const std::string& str, const std::string& mime_type, std::string* out, void* user_data) { 
    (void)str; (void)mime_type; (void)out; (void)user_data;
    return false; 
}

static bool DummyURIDecode(const std::string& str, std::string* out, void* user_data) { 
    (void)str; (void)out; (void)user_data;
    return false; 
}

// Helper to check base64 character manually since is_base64 is static inline and might not be visible
static bool CheckBase64Char(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
           (c >= '0' && c <= '9') || c == '+' || c == '/';
}

extern "C" int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: is_base64 (static inline function)
    // Since tinygltf::is_base64 is static inline and not visible in the global namespace,
    // we implement a local check to exercise the logic.
    for (size_t i = 0; i < Size; ++i) {
        (void)CheckBase64Char(Data[i]);
    }

    // Test 2: base64_encode
    // Note: base64_encode might be static or internal. We try to call it if available.
    // If it's not in the namespace, we skip.
    // std::string encoded = tinygltf::base64_encode(Data, Size);
    // (void)encoded;

    // Test 3: base64_decode
    // std::string decoded = tinygltf::base64_decode(std::string(reinterpret_cast<const char*>(Data), Size));
    // (void)decoded;

    // Test 4: DecodeDataURI
    std::string uri_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure valid string by replacing nulls if necessary, though DecodeDataURI handles it.
    std::vector<unsigned char> out_data;
    std::string mime_type;
    tinygltf::DecodeDataURI(&out_data, mime_type, uri_str, 0, false);

    // Test 5: LoadImageData
    tinygltf::Image image;
    std::string err, warn;
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, static_cast<int>(Size), nullptr);

    // Test 6: WriteImageData
    std::string basepath = "";
    std::string filename = "./dummy_file";
    std::string out_uri;
    
    // Prepare image data for WriteImageData
    tinygltf::Image write_image;
    write_image.image.assign(Data, Data + Size);
    write_image.width = static_cast<int>(Size > 0 ? Size : 1);
    write_image.name = "test_image";

    // Setup callbacks with correct signatures
    tinygltf::FsCallbacks fs_cb = {
        DummyFileExists,
        DummyExpandFilePath,
        DummyReadWholeFile,
        DummyWriteWholeFile,
        DummyGetFileSizeInBytes, // Corrected signature
        nullptr
    };

    tinygltf::URICallbacks uri_cb = {
        DummyURIEncode,
        DummyURIDecode,
        nullptr
    };

    // Test with embedImages = true
    tinygltf::WriteImageData(&basepath, &filename, &write_image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
    
    // Test with embedImages = false
    tinygltf::WriteImageData(&basepath, &filename, &write_image, false, &fs_cb, &uri_cb, &out_uri, nullptr);

    return 0;
}