// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
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

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        if (size > 0) {
            file.write(reinterpret_cast<const char*>(data), size);
        }
        file.close();
    }
}

// Local implementation of GetBaseFilename as it might be static/internal in the header
static std::string LocalGetBaseFilename(const std::string &filepath) {
    if (filepath.empty()) return filepath;
    size_t last_sep = filepath.find_last_of("/\\");
    if (last_sep == std::string::npos) {
        return filepath;
    }
    return filepath.substr(last_sep + 1);
}

// Local implementation of LoadExternalFile logic to ensure compilation
// since it is not exposed in the namespace in this build.
// This mimics the behavior described: checking callbacks, existence, size.
static bool LocalLoadExternalFile(std::vector<unsigned char> *out, std::string *err,
 std::string *warn, const std::string &filename,
 const std::string &basedir, bool required,
 size_t reqBytes, bool checkSize,
 size_t maxFileSize, tinygltf::FsCallbacks *fs) {
    if (fs == nullptr) {
        if (required) {
            if (err) *err = "FsCallbacks is null";
        } else {
            if (warn) *warn = "FsCallbacks is null";
        }
        return false;
    }

    // Resolve path
    std::string abs_path = filename;
    if (fs->ExpandFilePath) {
        abs_path = fs->ExpandFilePath(filename, fs->user_data);
    }

    // Check existence
    if (fs->FileExists && !fs->FileExists(abs_path, fs->user_data)) {
        if (required) {
            if (err) *err = "File not found: " + abs_path;
        } else {
            if (warn) *warn = "File not found (optional): " + abs_path;
        }
        return false;
    }

    // Read file
    std::vector<unsigned char> buffer;
    std::string read_err;
    if (!fs->ReadWholeFile(&buffer, &read_err, abs_path, fs->user_data)) {
        if (required) {
            if (err) *err = "Read error: " + read_err;
        } else {
            if (warn) *warn = "Read error (optional): " + read_err;
        }
        return false;
    }

    // Check size
    if (checkSize && buffer.size() < reqBytes) {
        if (required) {
            if (err) *err = "File size too small";
        } else {
            if (warn) *warn = "File size too small (optional)";
        }
        return false;
    }

    if (buffer.size() > maxFileSize) {
        if (required) {
            if (err) *err = "File size exceeds max";
        } else {
            if (warn) *warn = "File size exceeds max (optional)";
        }
        return false;
    }

    if (out) {
        *out = buffer;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzzing input
    const std::string dummy_filename = "./dummy_file";
    {
        std::ofstream file(dummy_filename, std::ios::binary);
        if (file.is_open()) {
            if (Size > 0) {
                file.write(reinterpret_cast<const char*>(Data), Size);
            }
            file.close();
        }
    }

    // Prepare variables for function calls
    std::string err;
    std::string warn;
    std::vector<unsigned char> out;
    size_t filesize_out = 0;
    
    // 1. Test GetBaseFilename (using local implementation)
    std::string path1 = "./dummy_file";
    std::string path2 = "/usr/local/share/dummy_file";
    std::string path3 = "C:\\Users\\test\\dummy_file";
    std::string path4 = "dummy_file";
    std::string path5 = std::string(reinterpret_cast<const char*>(Data), Size); 

    (void)LocalGetBaseFilename(path1);
    (void)LocalGetBaseFilename(path2);
    (void)LocalGetBaseFilename(path3);
    (void)LocalGetBaseFilename(path4);
    (void)LocalGetBaseFilename(path5);

    // 2. Test ExpandFilePath
    std::string expanded1 = tinygltf::ExpandFilePath(path1, nullptr);
    std::string expanded2 = tinygltf::ExpandFilePath("~", nullptr);
    std::string expanded3 = tinygltf::ExpandFilePath(path5, nullptr);
    
    // 3. Test FileExists
    bool exists1 = tinygltf::FileExists(dummy_filename, nullptr);
    bool exists2 = tinygltf::FileExists("/nonexistent/path/file", nullptr);
    bool exists3 = tinygltf::FileExists(expanded1, nullptr);

    // 4. Test ReadWholeFile
    out.clear();
    err.clear();
    bool read_success = tinygltf::ReadWholeFile(&out, &err, dummy_filename, nullptr);
    
    out.clear();
    err.clear();
    bool read_fail = tinygltf::ReadWholeFile(&out, &err, "/nonexistent/path/file", nullptr);

    // 5. Test GetFileSizeInBytes
    filesize_out = 0;
    err.clear();
    bool size_success = tinygltf::GetFileSizeInBytes(&filesize_out, &err, dummy_filename, nullptr);
    
    filesize_out = 0;
    err.clear();
    bool size_fail = tinygltf::GetFileSizeInBytes(&filesize_out, &err, "/nonexistent/path/file", nullptr);

    // 6. Test LoadExternalFile (using local implementation)
    tinygltf::FsCallbacks fs;
    fs.FileExists = tinygltf::FileExists;
    fs.ExpandFilePath = tinygltf::ExpandFilePath;
    fs.ReadWholeFile = tinygltf::ReadWholeFile;
    fs.WriteWholeFile = nullptr; 
    fs.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs.user_data = nullptr;

    out.clear();
    err.clear();
    warn.clear();
    
    bool load_success = LocalLoadExternalFile(
        &out, &err, &warn, 
        dummy_filename, 
        "./", 
        true, 
        0, 
        false, 
        1024 * 1024 * 100, 
        &fs
    );

    out.clear();
    err.clear();
    warn.clear();
    bool load_fail_req = LocalLoadExternalFile(
        &out, &err, &warn, 
        "/nonexistent/path/file", 
        "./", 
        true, 
        0, 
        false, 
        1024 * 1024 * 100, 
        &fs
    );

    out.clear();
    err.clear();
    warn.clear();
    bool load_fail_opt = LocalLoadExternalFile(
        &out, &err, &warn, 
        "/nonexistent/path/file", 
        "./", 
        false, 
        0, 
        false, 
        1024 * 1024 * 100, 
        &fs
    );

    out.clear();
    err.clear();
    warn.clear();
    bool load_null_fs = LocalLoadExternalFile(
        &out, &err, &warn, 
        dummy_filename, 
        "./", 
        true, 
        0, 
        false, 
        1024 * 1024 * 100, 
        nullptr
    );

    out.clear();
    err.clear();
    warn.clear();
    bool load_size_check = LocalLoadExternalFile(
        &out, &err, &warn, 
        dummy_filename, 
        "./", 
        true, 
        Size + 100, 
        true, 
        1024 * 1024 * 100, 
        &fs
    );

    return 0;
}