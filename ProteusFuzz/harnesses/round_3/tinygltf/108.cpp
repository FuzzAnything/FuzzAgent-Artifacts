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
#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Implementations of the static utility functions if they are not exposed in the namespace
// in the specific version of tinygltf.h used.
// These match the signatures provided in the prompt.

static std::string GetBaseFilename(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

static std::string JoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) {
        return path1;
    }
    if (path1.empty()) {
        return path0;
    }
    if (path0.back() == '/' || path0.back() == '\\') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

static std::string GetFilePathExtension(const std::string &FileName) {
    size_t pos = FileName.find_last_of('.');
    if (pos == std::string::npos) {
        return "";
    }
    // Check for hidden files (starts with .)
    if (pos == 0) {
        return "";
    }
    return FileName.substr(pos + 1);
}

static std::string GetBaseDir(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(0, pos + 1);
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    std::string filepath(reinterpret_cast<const char*>(Data), Size);
    std::string err;
    std::vector<unsigned char> out;
    void *userdata = nullptr;

    // Test ExpandFilePath (This is typically a non-static function in the namespace)
    std::string expandedPath = tinygltf::ExpandFilePath(filepath, userdata);
    (void)expandedPath;

    // Test ReadWholeFile
    // Write data to a dummy file first
    WriteDummyFile("./dummy_file", Data, Size);
    bool readSuccess = tinygltf::ReadWholeFile(&out, &err, "./dummy_file", userdata);
    (void)readSuccess;
    (void)err;

    // Test GetBaseFilename using local implementation
    std::string baseFilename = GetBaseFilename(filepath);
    (void)baseFilename;

    // Test JoinPath using local implementation
    std::string path0 = filepath;
    std::string path1 = "subdir/file.gltf";
    std::string joinedPath = JoinPath(path0, path1);
    (void)joinedPath;

    // Test GetFilePathExtension using local implementation
    std::string extension = GetFilePathExtension(filepath);
    (void)extension;

    // Test GetBaseDir using local implementation
    std::string baseDir = GetBaseDir(filepath);
    (void)baseDir;

    // Cleanup
    std::remove("./dummy_file");

    return 0;
}