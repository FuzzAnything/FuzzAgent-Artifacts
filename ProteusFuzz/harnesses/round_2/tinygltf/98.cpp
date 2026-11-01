// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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
#include <fstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Helper to implement GetBaseFilename if not exposed in the namespace directly
static std::string GetBaseFilename(const std::string &filepath) {
    size_t last_slash = filepath.find_last_of("/\\");
    if (last_slash == std::string::npos) {
        return filepath;
    }
    return filepath.substr(last_slash + 1);
}

// Helper to implement FindFile if not exposed in the namespace directly
static std::string FindFile(const std::vector<std::string> &paths,
                            const std::string &filepath,
                            tinygltf::FsCallbacks *fs) {
    if (!fs || filepath.empty()) {
        return "";
    }
    for (const auto &path : paths) {
        std::string full_path = path + "/" + filepath;
        if (fs->FileExists && fs->FileExists(full_path, fs->user_data)) {
            return full_path;
        }
    }
    return "";
}

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const std::string filepath("./dummy_file");
    WriteDummyFile(filepath, Data, Size);

    std::string err;
    std::vector<unsigned char> out;
    size_t filesize_out = 0;

    // Test 1: ReadWholeFile
    tinygltf::ReadWholeFile(&out, &err, filepath, nullptr);
    out.clear();
    err.clear();

    // Test 2: GetBaseFilename (using local implementation as per error)
    std::string path_with_dir = "/some/path/" + filepath;
    std::string base_name = GetBaseFilename(path_with_dir);
    std::string base_name_simple = GetBaseFilename(filepath);

    // Test 3: ExpandFilePath
    std::string expanded_path = tinygltf::ExpandFilePath(filepath, nullptr);
    std::string expanded_path_tilde = tinygltf::ExpandFilePath("~/dummy_file", nullptr);

    // Test 4: GetFileSizeInBytes
    tinygltf::GetFileSizeInBytes(&filesize_out, &err, filepath, nullptr);
    filesize_out = 0;
    err.clear();

    // Test 5: FileExists
    bool exists = tinygltf::FileExists(filepath, nullptr);
    bool exists_nonexistent = tinygltf::FileExists("/nonexistent/path/file.glb", nullptr);

    // Test 6: FindFile (using local implementation as per error)
    tinygltf::FsCallbacks fs;
    fs.FileExists = tinygltf::FileExists;
    fs.ExpandFilePath = tinygltf::ExpandFilePath;
    fs.ReadWholeFile = tinygltf::ReadWholeFile;
    fs.WriteWholeFile = nullptr;
    fs.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs.user_data = nullptr;

    std::vector<std::string> search_paths = {"/", "/some/path", "."};
    std::string found_path = FindFile(search_paths, "dummy_file", &fs);
    
    // Test with empty paths
    std::vector<std::string> empty_paths;
    std::string found_path_empty = FindFile(empty_paths, "dummy_file", &fs);

    // Test with null fs
    std::string found_path_null_fs = FindFile(search_paths, "dummy_file", nullptr);

    // Test with empty filepath
    std::string found_path_empty_file = FindFile(search_paths, "", &fs);

    return 0;
}