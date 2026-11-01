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
#include "tiny_gltf.h"
#include <fstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Replicate the static utility functions from tinygltf to ensure they are callable
// since they are often defined as static inside the header or namespace scope.
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
    size_t pos = FileName.find_last_of(".");
    if (pos == std::string::npos) {
        return "";
    }
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

extern "C" int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string input(reinterpret_cast<const char*>(Data), Size);

    // 1. Test GetBaseFilename
    std::string baseName = GetBaseFilename(input);
    (void)baseName;

    // 2. Test ExpandFilePath
    std::string expandedPath = tinygltf::ExpandFilePath(input, nullptr);
    (void)expandedPath;

    // 3. Test JoinPath
    std::string joinedPath = JoinPath(input, input);
    (void)joinedPath;

    // 4. Test GetFilePathExtension
    std::string extension = GetFilePathExtension(input);
    (void)extension;

    // 5. Test GetBaseDir
    std::string baseDir = GetBaseDir(input);
    (void)baseDir;

    // 6. Test ReadWholeFile
    const std::string dummyFilename = "./dummy_file";
    WriteDummyFile(dummyFilename, Data, Size);

    std::vector<unsigned char> out;
    std::string err;
    bool readSuccess = tinygltf::ReadWholeFile(&out, &err, dummyFilename, nullptr);
    
    if (readSuccess) {
        // Optional: Verify data integrity
        if (out.size() != Size) {
            // Logic check
        }
    } else {
        (void)err;
    }

    // Cleanup
    std::remove(dummyFilename.c_str());

    return 0;
}