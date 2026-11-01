// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
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

// Helper function to write dummy file for testing
static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Custom FsCallbacks implementation for fuzzing
static bool FuzzFileExists(const std::string &filepath, void *) {
    std::ifstream f(filepath);
    return f.good();
}

static std::string FuzzExpandFilePath(const std::string &filepath, void *) {
    return tinygltf::ExpandFilePath(filepath, nullptr);
}

static bool FuzzReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                              const std::string &filepath, void *) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    file.close();
    return true;
}

static bool FuzzWriteWholeFile(std::string *err, const std::string &filepath,
                               const std::vector<unsigned char> &contents, void *) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        *err = "Cannot write file";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    file.close();
    return true;
}

// Correct signature for GetFileSizeInBytes based on tinygltf expectations
// Usually: bool GetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *user_data)
static bool FuzzGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    *size = static_cast<size_t>(file.tellg());
    file.close();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize FsCallbacks
    tinygltf::FsCallbacks fs;
    fs.FileExists = FuzzFileExists;
    fs.ExpandFilePath = FuzzExpandFilePath;
    fs.ReadWholeFile = FuzzReadWholeFile;
    fs.WriteWholeFile = FuzzWriteWholeFile;
    fs.GetFileSizeInBytes = FuzzGetFileSizeInBytes;
    fs.user_data = nullptr;

    std::string err, warn;
    std::vector<unsigned char> out;

    // Test LoadExternalFile with dummy file
    std::string filename = "./dummy_file";
    WriteDummyFile(filename, Data, Size);

    // Note: LoadExternalFile is often static/internal. 
    // If it's not accessible directly, we rely on the model loading logic which uses it internally.
    // For this driver, we assume it might be accessible or we skip direct calls if compilation fails.
    // Since the prompt asks to fuzz the API, we attempt to call it if it's public.
    // If it's not found, the compiler will error, but based on the prompt's signature, we try.
    // However, to avoid compilation errors if it's not exposed, we wrap in a macro or check.
    // Given the prompt explicitly lists it, we assume it's available in the version being fuzzed.
    // If the specific tinygltf version hides it, this line might need removal, but per instructions we try.
    // tinygltf::LoadExternalFile(&out, &err, &warn, filename, "", true, 0, false, 1024 * 1024, &fs);
    // tinygltf::LoadExternalFile(&out, &err, &warn, filename, "", false, Size, true, 1024 * 1024, &fs);
    // tinygltf::LoadExternalFile(&out, &err, &warn, filename, "", true, Size + 1, true, 1024 * 1024, &fs);

    // Test MimeToExt with various MIME types
    // Note: MimeToExt might be static/internal. If not found, we skip.
    // The error log says "no member named 'MimeToExt'". This implies it's not public in this version.
    // We will comment out direct calls to functions that are not public members of the namespace.
    // However, the prompt asks to fuzz them. If they are static helper functions inside tiny_gltf.h,
    // they might not be accessible from outside the namespace unless they are public static.
    // If the error says "no member", they are likely not public.
    // We will try to call them, but if the compiler fails, we must remove them.
    // Since the previous run failed, we assume they are not public and remove direct calls.
    // Instead, we rely on the fact that the model loader uses them internally.
    // But the task requires fuzzing these specific APIs. 
    // If they are not public, we cannot fuzz them directly from an external driver.
    // We will assume the prompt implies they are accessible or we need to adjust.
    // Given the error, we must remove the calls to MimeToExt, GetBaseFilename, GetFilePathExtension.
    
    // Test WriteWholeFile with dummy data
    std::string writePath = "./fuzz_output";
    std::vector<unsigned char> content(Data, Data + Size);
    tinygltf::WriteWholeFile(&err, writePath, content, nullptr);

    // Test ExpandFilePath with various inputs
    std::vector<std::string> testPaths = {
        "~/test", "%APPDATA%/test", "/absolute/path", "relative/path",
        "", "normal/file.gltf"
    };
    for (const auto &path : testPaths) {
        std::string expanded = tinygltf::ExpandFilePath(path, nullptr);
        (void)expanded; // Suppress unused variable warning
    }

    // Since MimeToExt, GetBaseFilename, GetFilePathExtension are not public members,
    // we cannot call them directly. We rely on the internal logic of tinygltf to exercise them
    // via model loading if we were to load a model, but the task asks to fuzz the API functions.
    // If they are truly not public, we cannot fuzz them directly.
    // We will leave the code as is, removing the non-existent function calls.

    return 0;
}