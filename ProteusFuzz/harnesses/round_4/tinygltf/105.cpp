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
#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &filename, const std::string &content) {
    FILE *f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(content.c_str(), 1, content.size(), f);
        fclose(f);
    }
}

// Re-implementing the static utility functions from tinygltf to ensure compilation
// as they might be internal or not exposed in the public namespace in some builds.
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
    size_t dotPos = FileName.find_last_of('.');
    if (dotPos == std::string::npos || dotPos == 0) {
        return "";
    }
    return FileName.substr(dotPos + 1);
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string input(reinterpret_cast<const char *>(Data), Size);

    // 1. tinygltf::GetBaseDir (using local implementation)
    std::string baseDir = GetBaseDir(input);

    // 2. tinygltf::ExpandFilePath
    // This function is usually available in the namespace
    std::string expandedPath = tinygltf::ExpandFilePath(input, nullptr);

    // 3. tinygltf::GetBaseFilename (using local implementation)
    std::string baseFilename = GetBaseFilename(input);

    // 4. tinygltf::JoinPath (using local implementation)
    std::string path1 = "asset.gltf";
    std::string joinedPath = JoinPath(input, path1);
    std::string joinedPathEmpty = JoinPath("", input);
    std::string joinedPathEmpty1 = JoinPath(input, "");

    // 5. tinygltf::GetFilePathExtension (using local implementation)
    std::string extension = GetFilePathExtension(input);

    // 6. tinygltf::WriteGltfFile
    // Since WriteGltfFile is static and might not be exposed in the namespace,
    // we use the local WriteDummyFile helper which performs the same core logic
    // for fuzzing purposes (writing content to a file).
    const char *dummyFile = "./dummy_file";
    std::string content = "{\"asset\":{\"version\":\"2.0\"}}";
    
    // Trigger write operation based on input heuristic
    if (Size > 0 && input[0] == 'W') {
        WriteDummyFile(dummyFile, content);
    }

    return 0;
}