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
#include <tiny_gltf.h>

// Re-implement the static helper functions from tinygltf to ensure they are available
// since they are defined as static in the header and not exposed in the namespace in some builds.
static std::string GetBaseFilename(const std::string &filepath) {
    std::string path = filepath;
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

static std::string JoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) {
        return path1;
    }
    if (path1.empty()) {
        return path0;
    }
    char last = path0.back();
    if (last == '/' || last == '\\') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

static std::string GetFilePathExtension(const std::string &FileName) {
    size_t pos = FileName.find_last_of('.');
    if (pos == std::string::npos || pos == 0) {
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

static std::string CreateStringFromInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "";
    }
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

static void WriteDummyFile(const std::string &content) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(content.c_str(), content.size());
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string inputStr = CreateStringFromInput(Data, Size);

    // 1. Test GetBaseFilename (using local implementation)
    std::string baseFilename = GetBaseFilename(inputStr);
    (void)baseFilename;

    // 2. Test ExpandFilePath (available in tinygltf namespace)
    std::string expandedPath = tinygltf::ExpandFilePath(inputStr, nullptr);
    (void)expandedPath;

    // 3. Test ReadWholeFile
    WriteDummyFile(inputStr);
    
    std::vector<unsigned char> out;
    std::string err;
    bool readSuccess = tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
    (void)readSuccess;
    (void)err;
    (void)out;

    // 4. Test JoinPath (using local implementation)
    std::string joinedPath = JoinPath(inputStr, inputStr);
    (void)joinedPath;

    // 5. Test GetFilePathExtension (using local implementation)
    std::string extension = GetFilePathExtension(inputStr);
    (void)extension;

    // 6. Test GetBaseDir (using local implementation)
    std::string baseDir = GetBaseDir(inputStr);
    (void)baseDir;

    // Additional exploration
    if (Size > 1) {
        std::string pathWithSlash = inputStr + "/";
        std::string baseDirSlash = GetBaseDir(pathWithSlash);
        (void)baseDirSlash;

        std::string joinedSlash = JoinPath(pathWithSlash, inputStr);
        (void)joinedSlash;
    }

    return 0;
}