// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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
#include <functional>
#include <fstream>
#include <algorithm>
#include "tiny_gltf.h"

// Dummy file callbacks for testing matching tinygltf signatures
static bool FileExistsCallback(const std::string &, void *) {
    return false;
}

static std::string ExpandFilePathCallback(const std::string &, void *) {
    return "";
}

static bool ReadWholeFileCallback(std::vector<unsigned char> *, std::string *,
                                  const std::string &, void *) {
    return false;
}

static bool WriteWholeFileCallback(std::string *, const std::string &,
                                   const std::vector<unsigned char> &, void *) {
    return false;
}

// Correct signature for GetFileSizeInBytesFunction
static bool GetFileSizeInBytesCallback(size_t *out, std::string *err,
                                       const std::string &, void *) {
    if (out) *out = 0;
    return true;
}

// Correct signature for URIEncodeFunction based on error message:
// bool URIEncodeFunction(const std::string &src, const std::string &dst, std::string *out, void *user_data)
static bool URIEncodeCallback(const std::string &src, const std::string &dst, std::string *out, void *) {
    (void)src; (void)dst;
    if (out) *out = "";
    return true;
}

// Correct signature for URIDecodeFunction
static bool URIDecodeCallback(const std::string &src, std::string *dst, void *) {
    (void)src;
    if (dst) *dst = "";
    return true;
}

static tinygltf::LoadImageDataFunction LoadImageDataCallback = nullptr;

static tinygltf::FsCallbacks g_fs_callbacks = {
    FileExistsCallback,
    ExpandFilePathCallback,
    ReadWholeFileCallback,
    WriteWholeFileCallback,
    GetFileSizeInBytesCallback,
    nullptr
};

static tinygltf::URICallbacks g_uri_callbacks = {
    URIEncodeCallback,
    URIDecodeCallback,
    nullptr
};

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    tinygltf::Model model;
    tinygltf::Buffer buffer;
    tinygltf::Image image;
    std::string err;
    std::string warn;
    std::string basedir = "";
    size_t max_buffer_size = 1024 * 1024; // 1MB
    size_t max_file_size = 1024 * 1024;   // 1MB
    bool store_original_json = false;

    // 2. Fuzz LoadExternalFile (Internal function, not directly accessible)
    // We skip direct call to tinygltf::LoadExternalFile as it is static/internal
    // and not exposed in the public API. The functionality is tested indirectly
    // via the parser's file loading mechanisms.
    
    // 3. Fuzz TinyGLTF methods
    {
        tinygltf::TinyGLTF parser;
        
        // SetMaxExternalFileSize
        parser.SetMaxExternalFileSize(Size > 0 ? Size : 1024);
        
        // GetMaxExternalFileSize
        size_t max_size = parser.GetMaxExternalFileSize();
        (void)max_size; // Suppress unused warning
        
        // LoadBinaryFromMemory
        // Try to load as binary glTF (might fail, but that's okay for fuzzing)
        std::string bin_err;
        std::string bin_warn;
        parser.LoadBinaryFromMemory(&model, &bin_err, &bin_warn, Data, Size, basedir);
    }

    // 4. Fuzz ParseBuffer and ParseImage via LoadASCIIFromString if possible
    // Since ParseBuffer and ParseImage are static and internal, we can't call them directly
    // without access to internal json types. Instead, we rely on LoadASCIIFromString
    // to trigger their execution internally.
    {
        tinygltf::TinyGLTF parser;
        std::string ascii_err;
        std::string ascii_warn;
        
        // Try to parse as ASCII glTF
        std::string json_str(reinterpret_cast<const char*>(Data), Size);
        // Ensure it's valid C-string by removing nulls
        std::string cleaned_str;
        cleaned_str.reserve(json_str.size());
        for (char c : json_str) {
            if (c != '\0') {
                cleaned_str += c;
            }
        }
        
        parser.LoadASCIIFromString(&model, &ascii_err, &ascii_warn, 
                                   cleaned_str.c_str(), cleaned_str.length(), 
                                   basedir.c_str());
    }

    // 5. Cleanup
    // Model, Buffer, Image are automatically cleaned up by destructors
    // No manual cleanup needed for these types

    return 0;
}