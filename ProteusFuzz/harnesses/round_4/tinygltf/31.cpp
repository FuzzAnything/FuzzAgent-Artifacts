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
#include <cstdlib>
#include <string>
#include <vector>
#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Implement the utility functions locally if they are not exposed in the namespace
// This handles the case where the specific version of tinygltf hides these static functions.
static std::string GetBaseDir(const std::string &filepath) {
    std::string path = filepath;
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(0, pos + 1);
    }
    return "";
}

static std::string GetBaseFilename(const std::string &filepath) {
    std::string path = filepath;
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

static std::string JoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) return path1;
    if (path1.empty()) return path0;
    if (path0.back() == '/' || path0.back() == '\\') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

static std::string GetFilePathExtension(const std::string &FileName) {
    size_t pos = FileName.find_last_of(".");
    if (pos != std::string::npos && pos > 0 && pos < FileName.length() - 1) {
        return FileName.substr(pos + 1);
    }
    return "";
}

// Local implementation of WriteGltfFile logic since it's static and not in namespace
static bool WriteGltfFile(const std::string &output, const std::string &content) {
#ifdef TINYGLTF_NO_FS
    return false;
#else
    FILE *fp = fopen(output.c_str(), "wb");
    if (fp) {
        size_t result = fwrite(content.c_str(), sizeof(char), content.size(), fp);
        fclose(fp);
        return (result == content.size());
    }
    return false;
#endif
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string input(reinterpret_cast<const char *>(Data), Size);

    // 1. Fuzz GetBaseDir
    std::string baseDir = GetBaseDir(input);
    (void)baseDir;

    // 2. Fuzz ExpandFilePath
    // This function is usually available in the namespace. If it fails, we skip or handle gracefully.
    // Assuming ExpandFilePath is available in tinygltf namespace as per prompt.
    std::string expandedPath = tinygltf::ExpandFilePath(input, nullptr);
    (void)expandedPath;

    // 3. Fuzz GetBaseFilename
    std::string baseFilename = GetBaseFilename(input);
    (void)baseFilename;

    // 4. Fuzz JoinPath
    std::string path0 = input;
    std::string path1 = input;
    std::string joinedPath = JoinPath(path0, path1);
    (void)joinedPath;

    // Test JoinPath with empty strings
    std::string joinedEmpty0 = JoinPath("", input);
    std::string joinedEmpty1 = JoinPath(input, "");
    (void)joinedEmpty0;
    (void)joinedEmpty1;

    // 5. Fuzz GetFilePathExtension
    std::string extension = GetFilePathExtension(input);
    (void)extension;

    // 6. Fuzz WriteGltfFile
    // Create a dummy file content to write
    std::string dummyContent = "{\"asset\":{\"version\":\"2.0\"}}";
    std::string dummyFile = "./dummy_file.gltf";
    
    // Write the input data to the dummy file first to simulate a source file if needed
    WriteDummyFile(dummyFile, Data, Size);
    
    // Attempt to write glTF content using the local implementation
    bool writeSuccess = WriteGltfFile(dummyFile, dummyContent);
    (void)writeSuccess;

    // Cleanup: Remove the dummy file if it exists
    std::remove(dummyFile.c_str());

    return 0;
}