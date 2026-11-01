// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
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
#include <algorithm>
#include "tiny_gltf.h"

// Helper to write data to a dummy file
static bool WriteDummyFile(const uint8_t *Data, size_t Size, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    return file.good();
}

// Implementations of the missing static functions based on the prompt's description
// These are often internal utilities in tinygltf that might not be exposed in all versions or headers
// We replicate them here to ensure the fuzzer compiles and tests the logic described.

static std::string GetBaseDir(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(0, pos + 1);
}

static std::string GetBaseFilename(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

static std::string GetFilePathExtension(const std::string &FileName) {
    size_t pos = FileName.find_last_of(".");
    if (pos == std::string::npos) {
        return "";
    }
    // Check if it's a hidden file (starts with .)
    if (pos == 0) {
        return "";
    }
    return FileName.substr(pos + 1);
}

static std::string JoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) {
        return path1;
    }
    if (path0.back() == '/' || path0.back() == '\\') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

extern "C" int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input data to string for path testing
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Replace null bytes with spaces to avoid premature string termination issues in paths
    std::replace(input_str.begin(), input_str.end(), '\0', ' ');

    // Test GetBaseDir
    std::string base_dir = GetBaseDir(input_str);
    (void)base_dir; // Suppress unused variable warning

    // Test GetBaseFilename
    std::string base_filename = GetBaseFilename(input_str);
    (void)base_filename;

    // Test GetFilePathExtension
    std::string extension = GetFilePathExtension(input_str);
    (void)extension;

    // Test JoinPath with various combinations
    std::string path1 = input_str;
    std::string path2 = input_str;
    
    // Ensure path2 is not empty for meaningful join test if path1 is empty
    if (path1.empty()) {
        path2 = "test_file.gltf";
    }
    
    std::string joined_path = JoinPath(path1, path2);
    (void)joined_path;

    // Test ExpandFilePath
    // Note: userdata is typically nullptr
    std::string expanded_path = tinygltf::ExpandFilePath(input_str, nullptr);
    (void)expanded_path;

    // Test ReadWholeFile
    // We need a valid file to test ReadWholeFile
    std::string dummy_filename = "./dummy_file";
    
    // Write the fuzz input to the dummy file
    if (WriteDummyFile(Data, Size, dummy_filename)) {
        std::vector<unsigned char> out;
        std::string err;
        bool result = tinygltf::ReadWholeFile(&out, &err, dummy_filename, nullptr);
        
        // Use the result to ensure the compiler doesn't optimize away the call
        if (result) {
            // File read successfully, 'out' contains data
            (void)out.size();
        } else {
            // File read failed, 'err' contains message
            (void)err.size();
        }
    }

    return 0;
}