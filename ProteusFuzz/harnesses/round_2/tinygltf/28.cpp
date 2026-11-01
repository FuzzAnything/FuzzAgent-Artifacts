// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include <fstream>
#include "tiny_gltf.h"

// Helper to implement the logic of GetBaseDir if not available as a public static
static std::string GetBaseDirImpl(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return filepath.substr(0, pos + 1);
}

// Helper to implement the logic of GetFilePathExtension if not available as a public static
static std::string GetFilePathExtensionImpl(const std::string &FileName) {
    size_t pos = FileName.find_last_of('.');
    if (pos == std::string::npos || pos == 0) {
        return "";
    }
    return FileName.substr(pos + 1);
}

// Helper to implement the logic of GetBaseFilename if not available as a public static
static std::string GetBaseFilenameImpl(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

// Helper to implement the logic of JoinPath if not available as a public static
static std::string JoinPathImpl(const std::string &path0, const std::string &path1) {
    if (path0.empty()) {
        return path1;
    }
    if (path1.empty()) {
        return path0;
    }
    char lastChar = path0.back();
    if (lastChar == '/' || lastChar == '\\') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

// Helper to implement WriteGltfFile logic manually to avoid namespace issues
static bool WriteGltfFileImpl(const std::string &output, const std::string &content) {
    std::ofstream file(output, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    file.close();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string input(reinterpret_cast<const char*>(Data), Size);

    // Fuzz GetBaseDir logic
    std::string baseDir = GetBaseDirImpl(input);
    (void)baseDir;

    // Fuzz GetFilePathExtension logic
    std::string extension = GetFilePathExtensionImpl(input);
    (void)extension;

    // Fuzz GetBaseFilename logic
    std::string baseFilename = GetBaseFilenameImpl(input);
    (void)baseFilename;

    // Fuzz ExpandFilePath
    // This is a standard function in tinygltf, usually available as tinygltf::ExpandFilePath
    std::string expandedPath = tinygltf::ExpandFilePath(input, nullptr);
    (void)expandedPath;

    // Fuzz JoinPath logic
    std::string path0 = input.substr(0, Size / 2);
    std::string path1 = input.substr(Size / 2);
    std::string joinedPath = JoinPathImpl(path0, path1);
    (void)joinedPath;

    // Fuzz WriteGltfFile logic using the helper implementation
    std::string dummyFilePath = "./dummy_file_" + std::to_string(static_cast<unsigned int>(Size));
    std::string content = input;
    bool writeResult = WriteGltfFileImpl(dummyFilePath, content);
    (void)writeResult;

    // Cleanup
    if (writeResult) {
        std::remove(dummyFilePath.c_str());
    }

    return 0;
}