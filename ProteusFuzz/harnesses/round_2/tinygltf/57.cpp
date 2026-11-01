// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Helper to write fuzz data to a dummy file
static void WriteDummyFile(const uint8_t *Data, size_t Size, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Helper to implement GetBaseFilename if not available in the namespace
static std::string GetBaseFilenameImpl(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

// Helper to implement LoadExternalFile logic if not available
// This mimics the behavior described: checks callbacks, reads file, checks size.
static bool LoadExternalFileImpl(std::vector<unsigned char> *out, std::string *err,
                                 std::string *warn, const std::string &filename,
                                 const std::string &basedir, bool required,
                                 size_t reqBytes, bool checkSize,
                                 size_t maxFileSize, tinygltf::FsCallbacks *fs) {
    if (fs == nullptr || fs->ReadWholeFile == nullptr) {
        if (required && err) {
            *err = "Filesystem callbacks not set";
        }
        return false;
    }

    std::string fullpath = filename;
    if (!basedir.empty() && filename.find_first_of("/\\") == std::string::npos) {
        // Simple path join logic for relative files
        fullpath = basedir + "/" + filename;
    }

    if (fs->ExpandFilePath != nullptr) {
        fullpath = fs->ExpandFilePath(fullpath, fs->user_data);
    }

    if (!fs->ReadWholeFile(out, err, fullpath, fs->user_data)) {
        if (required && err) {
            // Error already set by ReadWholeFile usually
        }
        return false;
    }

    if (checkSize) {
        if (out->size() < reqBytes) {
            if (required && err) {
                *err = "File size smaller than required";
            }
            return false;
        }
        if (maxFileSize > 0 && out->size() > maxFileSize) {
            if (required && err) {
                *err = "File size exceeds maximum allowed";
            }
            return false;
        }
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare dummy file paths
    const std::string dummyFile = "./dummy_file";
    
    // Write fuzz data to the dummy file for file-based operations
    WriteDummyFile(Data, Size, dummyFile);

    // Variables to hold output and error messages
    std::vector<unsigned char> out;
    std::string err;
    std::string warn;
    size_t filesize_out = 0;

    // Initialize FsCallbacks with default tinygltf functions
    tinygltf::FsCallbacks fs;
    fs.FileExists = tinygltf::FileExists;
    fs.ExpandFilePath = tinygltf::ExpandFilePath;
    fs.ReadWholeFile = tinygltf::ReadWholeFile;
    fs.WriteWholeFile = nullptr; // Not used in this test
    fs.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs.user_data = nullptr;

    // Fuzzing Target 1: LoadExternalFile
    // Since the function is not directly exposed in the namespace or Model class in this version,
    // we use our implementation that mimics the logic using the available FsCallbacks.
    bool required = (Data[0] % 2) == 0;
    size_t reqBytes = (Size > 1) ? (static_cast<size_t>(Data[1]) * 100) : 0;
    bool checkSize = (Size > 2) ? (Data[2] % 2) == 0 : false;
    size_t maxFileSize = (Size > 3) ? (static_cast<size_t>(Data[3]) * 1000) : 0;
    
    // Test with existing file
    LoadExternalFileImpl(&out, &err, &warn, dummyFile, "", required, reqBytes, checkSize, maxFileSize, &fs);
    
    // Test with non-existing file
    LoadExternalFileImpl(&out, &err, &warn, "./non_existing_file.glb", "", required, reqBytes, checkSize, maxFileSize, &fs);
    
    // Test with missing callbacks (passing nullptr for fs)
    LoadExternalFileImpl(&out, &err, &warn, dummyFile, "", required, reqBytes, checkSize, maxFileSize, nullptr);

    // Fuzzing Target 2: ReadWholeFile
    // This is a free function in the namespace.
    out.clear(); err.clear();
    tinygltf::ReadWholeFile(&out, &err, dummyFile, nullptr);
    out.clear(); err.clear();
    tinygltf::ReadWholeFile(&out, &err, "./non_existing_file.glb", nullptr);

    // Fuzzing Target 3: GetBaseFilename
    // Since it's not in the namespace or Model, we use our implementation.
    std::string pathInput(reinterpret_cast<const char*>(Data), Size);
    std::string baseName = GetBaseFilenameImpl(pathInput);
    
    std::string pathWithSlash = "dir/subdir/file.glb";
    baseName = GetBaseFilenameImpl(pathWithSlash);
    std::string pathWithBackslash = "dir\\subdir\\file.glb";
    baseName = GetBaseFilenameImpl(pathWithBackslash);
    std::string bareFilename = "file.glb";
    baseName = GetBaseFilenameImpl(bareFilename);

    // Fuzzing Target 4: ExpandFilePath
    // Free function in the namespace.
    std::string expandedPath = tinygltf::ExpandFilePath(pathInput, nullptr);
    expandedPath = tinygltf::ExpandFilePath("~/test_file", nullptr);
    expandedPath = tinygltf::ExpandFilePath("", nullptr);

    // Fuzzing Target 5: GetFileSizeInBytes
    // Free function in the namespace.
    tinygltf::GetFileSizeInBytes(&filesize_out, &err, dummyFile, nullptr);
    tinygltf::GetFileSizeInBytes(&filesize_out, &err, "./non_existing_file.glb", nullptr);

    // Fuzzing Target 6: FileExists
    // Free function in the namespace.
    bool exists = tinygltf::FileExists(dummyFile, nullptr);
    exists = tinygltf::FileExists("./non_existing_file.glb", nullptr);
    exists = tinygltf::FileExists("", nullptr);

    return 0;
}