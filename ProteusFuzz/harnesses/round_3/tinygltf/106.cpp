// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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
#include <sstream>
#include <cstdio>

// Helper to write fuzz data to a dummy file
static bool WriteDummyFile(const uint8_t *Data, size_t Size, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    if (Size > 0) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    }
    file.close();
    return true;
}

// Local implementation of GetBaseFilename as it might be internal in tinygltf
static std::string LocalGetBaseFilename(const std::string &filepath) {
    size_t last_sep = filepath.find_last_of("/\\");
    if (last_sep == std::string::npos) {
        return filepath;
    }
    return filepath.substr(last_sep + 1);
}

// Local implementation of LoadExternalFile logic to ensure compilation
// This mimics the behavior described: load file using FsCallbacks
static bool LocalLoadExternalFile(std::vector<unsigned char> *out, std::string *err,
                                  std::string *warn, const std::string &filename,
                                  const std::string &basedir, bool required,
                                  size_t reqBytes, bool checkSize,
                                  size_t maxFileSize, tinygltf::FsCallbacks *fs) {
    if (!fs) {
        if (err) *err = "FsCallbacks is null";
        return false;
    }
    
    std::string full_path = basedir + filename;
    std::string expanded_path = fs->ExpandFilePath(full_path, fs->user_data);
    
    if (!fs->FileExists(expanded_path, fs->user_data)) {
        if (required) {
            if (err) *err = "File not found: " + expanded_path;
            return false;
        } else {
            if (warn) *warn = "File not found (optional): " + expanded_path;
            return false;
        }
    }

    if (checkSize) {
        size_t file_size = 0;
        std::string size_err;
        if (!fs->GetFileSizeInBytes(&file_size, &size_err, expanded_path, fs->user_data)) {
            if (err) *err = "Failed to get file size: " + size_err;
            return false;
        }
        if (file_size > maxFileSize) {
            if (err) *err = "File size exceeds max limit";
            return false;
        }
        if (file_size < reqBytes) {
            if (err) *err = "File size is less than required";
            return false;
        }
    }

    out->clear();
    std::string read_err;
    if (!fs->ReadWholeFile(out, &read_err, expanded_path, fs->user_data)) {
        if (err) *err = "Failed to read file: " + read_err;
        return false;
    }

    return true;
}

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file path string from input to test path handling
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Sanitize input for file creation (replace nulls or invalid chars if needed, 
    // but for fuzzing we want to test edge cases, so we'll use a safe prefix)
    std::string dummy_path = "./dummy_file_fuzz";
    
    // Write data to dummy file for file-based API tests
    WriteDummyFile(Data, Size, dummy_path);

    // 1. Test GetBaseFilename (using local implementation due to visibility issues)
    std::string base_name = LocalGetBaseFilename(input_str);
    (void)base_name; // Suppress unused warning

    // 2. Test ExpandFilePath
    std::string expanded_path = tinygltf::ExpandFilePath(input_str, nullptr);
    (void)expanded_path;

    // 3. Test ReadWholeFile
    std::vector<unsigned char> read_out;
    std::string read_err;
    tinygltf::ReadWholeFile(&read_out, &read_err, dummy_path, nullptr);
    // Also test with a non-existent file path derived from input
    std::string non_existent_path = input_str + "_nonexistent";
    tinygltf::ReadWholeFile(&read_out, &read_err, non_existent_path, nullptr);

    // 4. Test FileExists
    bool exists = tinygltf::FileExists(dummy_path, nullptr);
    (void)exists;
    bool exists_fake = tinygltf::FileExists(non_existent_path, nullptr);
    (void)exists_fake;

    // 5. Test GetFileSizeInBytes
    size_t filesize = 0;
    std::string size_err;
    tinygltf::GetFileSizeInBytes(&filesize, &size_err, dummy_path, nullptr);
    tinygltf::GetFileSizeInBytes(&filesize, &size_err, non_existent_path, nullptr);

    // 6. Test LoadExternalFile logic using local implementation
    std::vector<unsigned char> load_out;
    std::string load_err;
    std::string load_warn;
    
    std::string basedir = "./";
    
    // Prepare FsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = tinygltf::FileExists;
    fs_callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
    fs_callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
    fs_callbacks.WriteWholeFile = nullptr; // Not used in this test
    fs_callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    // Try loading the dummy file we created
    LocalLoadExternalFile(&load_out, &load_err, &load_warn, 
                          dummy_path, basedir, 
                          true, 0, false, 
                          SIZE_MAX, &fs_callbacks);
    
    // Try loading a non-existent file
    LocalLoadExternalFile(&load_out, &load_err, &load_warn, 
                          non_existent_path, basedir, 
                          true, 0, false, 
                          SIZE_MAX, &fs_callbacks);

    // Try with checkSize enabled and a specific size requirement
    LocalLoadExternalFile(&load_out, &load_err, &load_warn, 
                          dummy_path, basedir, 
                          true, Size, true, 
                          SIZE_MAX, &fs_callbacks);

    return 0;
}