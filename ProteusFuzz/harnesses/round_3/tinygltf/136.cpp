// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
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
#include <fstream>

// Forward declarations for function pointer types to match tinygltf.h expectations
// The error indicates a mismatch in the 4th argument of WriteWholeFile:
// Expected: const std::vector<unsigned char> &
// Provided: const std::vector<unsigned char> *

// Corrected signature for WriteWholeFile based on compiler error:
// bool WriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data)
static bool FuzzWriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data) {
    (void)user_data;
    if (err) *err = "";
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    if (!contents.empty()) {
        ofs.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    }
    ofs.close();
    return true;
}

// Expected signature for ReadWholeFile:
// bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data)
static bool FuzzReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data) {
    (void)user_data;
    if (err) *err = "";
    if (out) out->clear();
    if (filename.length() > 0) {
        out->resize(filename.length() % 1024 + 1);
        for (size_t i = 0; i < out->size(); ++i) {
            (*out)[i] = static_cast<unsigned char>(i);
        }
    }
    return true;
}

// Expected signature for FileExists:
// bool FileExists(const std::string &filename, void *user_data)
static bool FuzzFileExists(const std::string &filename, void *user_data) {
    (void)user_data;
    (void)filename;
    return true;
}

// Expected signature for ExpandFilePath:
// std::string ExpandFilePath(const std::string &filepath, void *user_data)
static std::string FuzzExpandFilePath(const std::string &filepath, void *user_data) {
    (void)user_data;
    return filepath;
}

// Expected signature for GetFileSizeInBytes:
// bool GetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data)
static bool FuzzGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) {
    (void)user_data;
    if (err) *err = "";
    if (size) *size = 1024;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    std::string err;

    // 1. Prepare FsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = FuzzFileExists;
    fs_callbacks.ExpandFilePath = FuzzExpandFilePath;
    fs_callbacks.ReadWholeFile = FuzzReadWholeFile;
    fs_callbacks.WriteWholeFile = FuzzWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = FuzzGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    // Invoke SetFsCallbacks
    ctx.SetFsCallbacks(fs_callbacks, &err);

    // 2. Prepare Model
    tinygltf::Model model;
    // Add some minimal data to model to ensure serialization isn't trivial
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    // 3. Test Serialize Default Values
    // Set to true
    ctx.SetSerializeDefaultValues(true);
    // Check state
    bool is_default = ctx.GetSerializeDefaultValues();
    (void)is_default; // Use variable to avoid unused warning

    // 4. Test WriteGltfSceneToFile
    // Use a dummy filename
    const std::string dummy_file = "./dummy_file";
    
    // Variation 1: ASCII, no embed
    ctx.WriteGltfSceneToFile(&model, dummy_file + ".gltf", false, false, true, false);
    
    // Variation 2: ASCII, embed images/buffers
    ctx.WriteGltfSceneToFile(&model, dummy_file + "_embed.gltf", true, true, true, false);
    
    // Variation 3: Binary
    ctx.WriteGltfSceneToFile(&model, dummy_file + ".glb", false, false, false, true);

    // 5. Test WriteGltfSceneToStream
    std::ostringstream oss;
    
    // Overload 1: (model, stream, prettyPrint, writeBinary)
    ctx.WriteGltfSceneToStream(&model, oss, true, false);
    
    // Overload 2: (model, stream, prettyPrint, writeBinary) - same signature, different call
    ctx.WriteGltfSceneToStream(&model, oss, false, true);

    // 6. Cleanup
    // No explicit cleanup needed for tinygltf::Model or TinyGLTF as they use RAII
    // Remove dummy files if they exist (optional for fuzzing, but good practice)
    std::remove((dummy_file + ".gltf").c_str());
    std::remove((dummy_file + "_embed.gltf").c_str());
    std::remove((dummy_file + ".glb").c_str());

    return 0;
}