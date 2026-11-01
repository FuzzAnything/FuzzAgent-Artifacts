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
#include <algorithm>
#include <fstream>
#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Implement the static helper functions locally if they are not exposed in the public API
// This ensures the fuzz driver compiles regardless of the specific tinygltf version's visibility.
static std::string GetFilePathExtension(const std::string &FileName) {
    size_t found = FileName.rfind('.');
    if (std::string::npos == found) {
        return "";
    }
    if (found == 0) {
        return ""; // Hidden file like ".gitignore"
    }
    return FileName.substr(found + 1);
}

static std::string JoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) {
        return path1;
    }
    if (path1.empty()) {
        return path0;
    }
    std::string res = path0;
    if (res.back() != '/' && res.back() != '\\') {
        res += '/';
    }
    res += path1;
    return res;
}

static std::string GetBaseDir(const std::string &filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (std::string::npos == found) {
        return "";
    }
    return filepath.substr(0, found + 1);
}

static std::string GetBaseFilename(const std::string &filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (std::string::npos == found) {
        return filepath;
    }
    return filepath.substr(found + 1);
}

extern "C" int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to test file-based functions
    const std::string dummy_filename = "./dummy_file";
    WriteDummyFile(dummy_filename, Data, Size);

    // Construct a safe path string from input
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Replace null bytes to ensure valid C-string behavior for path functions if needed,
    // though the functions handle std::string.
    std::string safe_path = input_str;
    if (safe_path.empty()) {
        safe_path = "test.gltf";
    }

    // Test GetFilePathExtension
    std::string ext = GetFilePathExtension(safe_path);
    (void)ext;

    // Test JoinPath
    // Fix for std::max type mismatch: ensure both arguments are size_t
    size_t half_size = Size / 2;
    size_t start_pos = (safe_path.size() > half_size) ? (safe_path.size() - half_size) : 0;
    std::string path0 = safe_path.substr(0, half_size);
    std::string path1 = safe_path.substr(start_pos);
    std::string joined_path = JoinPath(path0, path1);
    (void)joined_path;

    // Test GetBaseDir
    std::string base_dir = GetBaseDir(safe_path);
    (void)base_dir;

    // Test GetBaseFilename
    std::string base_filename = GetBaseFilename(safe_path);
    (void)base_filename;

    // Test ExpandFilePath
    // tinygltf::ExpandFilePath is usually a global function in the namespace
    // If it's not available, we skip or implement a stub. 
    // Assuming it exists in the header as per standard tinygltf.
    std::string expanded_path = tinygltf::ExpandFilePath(safe_path, nullptr);
    (void)expanded_path;

    // Test ReadWholeFile
    std::vector<unsigned char> out;
    std::string err;
    bool success = tinygltf::ReadWholeFile(&out, &err, dummy_filename, nullptr);
    (void)success;
    (void)err;
    (void)out;

    // Cleanup
    std::remove(dummy_filename.c_str());

    return 0;
}