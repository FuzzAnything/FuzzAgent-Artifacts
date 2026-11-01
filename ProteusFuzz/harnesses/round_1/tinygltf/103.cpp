// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include "tiny_gltf.h"

// Helper to create a minimal valid glTF JSON string
static std::string CreateMinimalGltfJson() {
    return R"({
        "asset": { "version": "2.0" },
        "scene": 0,
        "scenes": [{}]
    })";
}

// Helper to create a minimal valid glTF Binary (GLB) header + JSON
static std::vector<unsigned char> CreateMinimalGlb(const std::string &jsonStr) {
    std::vector<unsigned char> glb;
    // Header
    glb.push_back('g'); glb.push_back('l'); glb.push_back('T'); glb.push_back('F');
    glb.push_back(0x02); glb.push_back(0x00); glb.push_back(0x00); glb.push_back(0x00); // Version 2.0

    size_t jsonSize = jsonStr.size();
    // Pad JSON to 4-byte alignment
    size_t padding = (4 - (jsonSize % 4)) % 4;
    size_t totalJsonSize = jsonSize + padding;

    // JSON Chunk Header
    uint32_t jsonChunkSize = static_cast<uint32_t>(totalJsonSize);
    uint32_t jsonChunkType = 0x4E4F534A; // "JSON"
    glb.push_back(jsonChunkSize & 0xFF); glb.push_back((jsonChunkSize >> 8) & 0xFF);
    glb.push_back((jsonChunkSize >> 16) & 0xFF); glb.push_back((jsonChunkSize >> 24) & 0xFF);
    glb.push_back(jsonChunkType & 0xFF); glb.push_back((jsonChunkType >> 8) & 0xFF);
    glb.push_back((jsonChunkType >> 16) & 0xFF); glb.push_back((jsonChunkType >> 24) & 0xFF);

    // JSON Data
    glb.insert(glb.end(), jsonStr.begin(), jsonStr.end());
    for (size_t i = 0; i < padding; ++i) glb.push_back(0);

    // Total file size (Header + JSON Chunk)
    uint32_t totalSize = 12 + totalJsonSize;
    // Overwrite total size in header (bytes 4-7)
    glb[4] = totalSize & 0xFF; glb[5] = (totalSize >> 8) & 0xFF;
    glb[6] = (totalSize >> 16) & 0xFF; glb[7] = (totalSize >> 24) & 0xFF;

    return glb;
}

// Dummy Filesystem Callbacks matching tinygltf.h signatures
// Note: tinygltf.h defines these as std::function with specific signatures.
// We must match them exactly.
// ReadWholeFile: bool (std::vector<unsigned char>*, std::string*, const std::string&, void*)
static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *) {
    if (err) *err = "File not found (dummy)";
    return false;
}
// WriteWholeFile: bool (std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// Note the const std::vector<unsigned char>& for contents
static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *) {
    (void)contents; // Unused
    if (err) *err = "Write disabled (dummy)";
    return false;
}
// GetFileSizeInBytes: bool (uint64_t*, std::string*, const std::string&, void*)
static bool DummyGetFileSize(uint64_t *size, std::string *err, const std::string &filename, void *) {
    (void)filename;
    if (err) *err = "Size not available (dummy)";
    if (size) *size = 0;
    return false;
}

// Dummy URI Callbacks matching tinygltf.h signatures
// URICallbacks:
// encode: bool (const std::string&, const std::string&, std::string*, void*)
// decode: bool (const std::string&, std::string*, void*)
static bool DummyURIEncode(const std::string &str, const std::string &base_dir, std::string *out, void *) {
    (void)base_dir;
    if (out) *out = str;
    return true;
}
static bool DummyURIDecode(const std::string &str, std::string *out, void *) {
    if (out) *out = str;
    return true;
}

// Dummy Image Loader matching tinygltf.h signatures
// LoadImageDataFunction is:
// bool (Image*, const int, std::string*, std::string*, int, int, const unsigned char*, int, void*)
// Note: image_index is const int, width/height are int (not pointers)
static bool DummyImageLoader(tinygltf::Image* image, const int image_index,
                             std::string* uri, std::string* base_dir,
                             int width, int height,
                             const unsigned char* data, int data_length,
                             void* user_data) {
    (void)image; (void)image_index; (void)uri; (void)base_dir;
    (void)width; (void)height; (void)data; (void)data_length; (void)user_data;
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF parser;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Interpret first byte to select test case
    int op = Data[0] % 7;

    switch (op) {
        case 0: {
            // Test SetFsCallbacks
            tinygltf::FsCallbacks callbacks;
            callbacks.FileExists = nullptr;
            callbacks.ExpandFilePath = nullptr;
            callbacks.ReadWholeFile = DummyReadWholeFile;
            callbacks.WriteWholeFile = DummyWriteWholeFile;
            callbacks.GetFileSizeInBytes = DummyGetFileSize;
            callbacks.user_data = nullptr;

            // Try with valid callbacks
            parser.SetFsCallbacks(callbacks, &err);
            // Try with null function pointers to trigger error path
            tinygltf::FsCallbacks bad_callbacks = callbacks;
            bad_callbacks.ReadWholeFile = nullptr;
            parser.SetFsCallbacks(bad_callbacks, &err);
            break;
        }
        case 1: {
            // Test SetURICallbacks
            tinygltf::URICallbacks callbacks;
            callbacks.encode = DummyURIEncode;
            callbacks.decode = DummyURIDecode;
            callbacks.user_data = nullptr;

            parser.SetURICallbacks(callbacks, &err);

            tinygltf::URICallbacks bad_callbacks = callbacks;
            bad_callbacks.encode = nullptr;
            parser.SetURICallbacks(bad_callbacks, &err);
            break;
        }
        case 2: {
            // Test LoadASCIIFromString
            // Use the input data as the JSON string (cast to char*)
            // Ensure it's null-terminated or use length
            std::string json_str(reinterpret_cast<const char*>(Data), Size);
            // If input is too short or not valid JSON, try minimal valid JSON
            if (Size < 20 || json_str.find("asset") == std::string::npos) {
                json_str = CreateMinimalGltfJson();
            }
            parser.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(),
                                       json_str.length(), "",
                                       tinygltf::REQUIRE_VERSION);
            break;
        }
        case 3: {
            // Test LoadASCIIFromFile
            // Write input to dummy file
            std::string filename = "./dummy_file.gltf";
            std::ofstream ofs(filename, std::ios::binary);
            ofs.write(reinterpret_cast<const char*>(Data), Size);
            ofs.close();

            parser.LoadASCIIFromFile(&model, &err, &warn, filename,
                                     tinygltf::REQUIRE_VERSION);
            break;
        }
        case 4: {
            // Test LoadBinaryFromFile
            // Write input to dummy file
            std::string filename = "./dummy_file.glb";
            std::ofstream ofs(filename, std::ios::binary);
            ofs.write(reinterpret_cast<const char*>(Data), Size);
            ofs.close();

            parser.LoadBinaryFromFile(&model, &err, &warn, filename,
                                      tinygltf::REQUIRE_VERSION);
            break;
        }
        case 5: {
            // Test SetImageLoader
            parser.SetImageLoader(DummyImageLoader, nullptr);
            // Set to nullptr
            parser.SetImageLoader(nullptr, nullptr);
            break;
        }
        case 6: {
            // Combined test: Set callbacks then load from string
            tinygltf::FsCallbacks fs_cb;
            fs_cb.FileExists = nullptr;
            fs_cb.ExpandFilePath = nullptr;
            fs_cb.ReadWholeFile = DummyReadWholeFile;
            fs_cb.WriteWholeFile = DummyWriteWholeFile;
            fs_cb.GetFileSizeInBytes = DummyGetFileSize;
            fs_cb.user_data = nullptr;
            parser.SetFsCallbacks(fs_cb, &err);

            tinygltf::URICallbacks uri_cb;
            uri_cb.encode = DummyURIEncode;
            uri_cb.decode = DummyURIDecode;
            uri_cb.user_data = nullptr;
            parser.SetURICallbacks(uri_cb, &err);

            parser.SetImageLoader(DummyImageLoader, nullptr);

            std::string json_str(reinterpret_cast<const char*>(Data), Size);
            if (Size < 20) json_str = CreateMinimalGltfJson();

            parser.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(),
                                       json_str.length(), "",
                                       tinygltf::REQUIRE_VERSION);
            break;
        }
    }

    return 0;
}