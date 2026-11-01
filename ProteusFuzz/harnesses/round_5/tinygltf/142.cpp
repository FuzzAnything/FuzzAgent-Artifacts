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

// Helper to create a dummy FsCallbacks structure for testing
static tinygltf::FsCallbacks CreateDummyFsCallbacks() {
    tinygltf::FsCallbacks fs;
    // Initialize with null pointers to test error handling paths
    fs.FileExists = nullptr;
    fs.ExpandFilePath = nullptr;
    fs.ReadWholeFile = nullptr;
    fs.WriteWholeFile = nullptr;
    fs.GetFileSizeInBytes = nullptr;
    fs.user_data = nullptr;
    return fs;
}

// Helper to write data to a dummy file for file-based tests
static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

// Re-implement the static helper functions locally since they are not exposed in the public header
// This allows us to fuzz the logic of these functions using the input data.
static std::string GetFilePathExtension(const std::string &FileName) {
    size_t lastDot = FileName.find_last_of('.');
    if (lastDot == std::string::npos || lastDot == 0) {
        return "";
    }
    return FileName.substr(lastDot + 1);
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

static std::string GetBaseFilename(const std::string &filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return filepath;
    }
    return filepath.substr(lastSlash + 1);
}

// Local implementation of FindFile to fuzz its logic
static std::string FindFileLocal(const std::vector<std::string> &paths,
                                 const std::string &filepath,
                                 tinygltf::FsCallbacks *fs) {
    if (!fs || !fs->FileExists) {
        return "";
    }
    if (filepath.empty()) {
        return "";
    }

    for (const auto &path : paths) {
        std::string full_path = JoinPath(path, filepath);
        if (fs->FileExists(full_path, fs->user_data)) {
            return full_path;
        }
    }
    return "";
}

// Local implementation of LoadExternalFile to fuzz its logic
// Note: The signature of GetFileSizeInBytes in tinygltf is:
// bool GetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data);
static bool LoadExternalFileLocal(std::vector<unsigned char> *out, std::string *err,
                                  std::string *warn, const std::string &filename,
                                  const std::string &basedir, bool required,
                                  size_t reqBytes, bool checkSize,
                                  size_t maxFileSize, tinygltf::FsCallbacks *fs) {
    if (!fs || !fs->ReadWholeFile || !fs->GetFileSizeInBytes) {
        if (required && err) {
            *err = "Missing filesystem callbacks";
        }
        return false;
    }

    std::string full_path = JoinPath(basedir, filename);
    if (full_path.empty()) {
        if (required && err) {
            *err = "Invalid path";
        }
        return false;
    }

    std::string read_err;
    std::vector<unsigned char> buffer;
    if (!fs->ReadWholeFile(&buffer, &read_err, full_path, fs->user_data)) {
        if (required && err) {
            *err = "File not found or read error: " + read_err;
        } else if (warn) {
            *warn = "File not found or read error: " + read_err;
        }
        return false;
    }

    if (checkSize) {
        size_t size = 0;
        std::string size_err;
        // Correct usage of GetFileSizeInBytes: returns bool, fills size pointer and error string
        if (!fs->GetFileSizeInBytes(&size, &size_err, full_path, fs->user_data)) {
            // If getting size fails, we might treat it as an error or skip check depending on logic
            // For fuzzing, we treat failure to get size as a failure if required
            if (required && err) {
                *err = "Failed to get file size: " + size_err;
            } else if (warn) {
                *warn = "Failed to get file size: " + size_err;
            }
            return false;
        }

        if (size > maxFileSize) {
            if (required && err) {
                *err = "File size exceeds limit";
            } else if (warn) {
                *warn = "File size exceeds limit";
            }
            return false;
        }
        if (reqBytes > 0 && size < reqBytes) {
            if (required && err) {
                *err = "File size smaller than required";
            } else if (warn) {
                *warn = "File size smaller than required";
            }
            return false;
        }
    }

    if (out) {
        *out = buffer;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input data to a string for path manipulation
    // Replace null bytes with spaces to prevent premature truncation in string operations
    std::string safe_input;
    safe_input.reserve(Size);
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == 0) {
            safe_input.push_back(' ');
        } else {
            safe_input.push_back(Data[i]);
        }
    }

    // 1. Fuzz GetFilePathExtension (using local implementation)
    std::string ext = GetFilePathExtension(safe_input);
    (void)ext;

    // 2. Fuzz JoinPath (using local implementation)
    size_t split_point = Size / 2;
    std::string path0 = safe_input.substr(0, split_point);
    std::string path1 = safe_input.substr(split_point);
    
    std::string joined = JoinPath(path0, path1);
    (void)joined;
    
    std::string joined_empty0 = JoinPath("", path1);
    (void)joined_empty0;
    
    std::string joined_empty1 = JoinPath(path0, "");
    (void)joined_empty1;

    // 3. Fuzz GetBaseFilename (using local implementation)
    std::string base_name = GetBaseFilename(safe_input);
    (void)base_name;

    // 4. Fuzz ExpandFilePath
    // This function is part of the public API in tinygltf
    std::string expanded = tinygltf::ExpandFilePath(safe_input, nullptr);
    (void)expanded;

    // 5. Fuzz FindFile (using local implementation)
    tinygltf::FsCallbacks fs = CreateDummyFsCallbacks();
    
    std::vector<std::string> paths;
    paths.push_back(safe_input); 
    paths.push_back("./dummy_dir");
    
    std::string found_path = FindFileLocal(paths, safe_input, &fs);
    (void)found_path;
    
    std::string found_path_null = FindFileLocal(paths, safe_input, nullptr);
    (void)found_path_null;

    // 6. Fuzz LoadExternalFile (using local implementation)
    std::vector<unsigned char> out_buffer;
    std::string err_msg;
    std::string warn_msg;
    
    bool required = (Data[0] % 2 == 0);
    bool check_size = (Size > 1 && (Data[1] % 2 == 0));
    size_t max_file_size = (Size > 2) ? static_cast<size_t>(Data[2]) : 1024;
    size_t req_bytes = (Size > 3) ? static_cast<size_t>(Data[3]) : 0;

    WriteDummyFile(Data, Size);
    
    bool load_result = LoadExternalFileLocal(
        &out_buffer, 
        &err_msg, 
        &warn_msg, 
        "./dummy_file", 
        safe_input, 
        required, 
        req_bytes, 
        check_size, 
        max_file_size, 
        &fs
    );
    (void)load_result;

    bool load_result_null = LoadExternalFileLocal(
        &out_buffer, 
        &err_msg, 
        &warn_msg, 
        "./dummy_file", 
        safe_input, 
        required, 
        req_bytes, 
        check_size, 
        max_file_size, 
        nullptr
    );
    (void)load_result_null;

    bool load_result_missing = LoadExternalFileLocal(
        &out_buffer, 
        &err_msg, 
        &warn_msg, 
        "./non_existent_file_" + safe_input, 
        safe_input, 
        required, 
        req_bytes, 
        check_size, 
        max_file_size, 
        &fs
    );
    (void)load_result_missing;

    return 0;
}