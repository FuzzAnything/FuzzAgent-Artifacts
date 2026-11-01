// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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

static void CleanupFile(const std::string &filename) {
    std::remove(filename.c_str());
}

// Helper to implement GetBaseFilename logic if not exposed as public static in the specific header version
static std::string GetBaseFilename(const std::string &filepath) {
    std::string result = filepath;
    size_t last_slash = result.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        result = result.substr(last_slash + 1);
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // Use a consistent dummy filename for file operations
    const std::string dummy_file = "./dummy_file";

    // 1. Test ExpandFilePath
    std::string expanded_path = tinygltf::ExpandFilePath(input, nullptr);
    (void)expanded_path; 

    // 2. Test GetBaseFilename (using local implementation to ensure availability)
    std::string base_filename = GetBaseFilename(input);
    (void)base_filename;

    // 3. Test FileExists (on input path and dummy file)
    bool file_exists_input = tinygltf::FileExists(input, nullptr);
    (void)file_exists_input;

    // Write input data to dummy file to test file operations
    WriteDummyFile(dummy_file, Data, Size);
    bool file_exists_dummy = tinygltf::FileExists(dummy_file, nullptr);
    (void)file_exists_dummy;

    // 4. Test GetFileSizeInBytes
    size_t filesize = 0;
    std::string err;
    bool size_result = tinygltf::GetFileSizeInBytes(&filesize, &err, dummy_file, nullptr);
    (void)size_result;
    (void)err;

    // 5. Test ReadWholeFile
    std::vector<unsigned char> out;
    bool read_result = tinygltf::ReadWholeFile(&out, &err, dummy_file, nullptr);
    (void)read_result;
    (void)out;

    // 6. Test LoadExternalFile
    // Note: LoadExternalFile is often a private static helper in tinygltf and not exposed 
    // in the public namespace in all versions. If it is not available as a free function 
    // in the included header, we skip direct invocation to avoid compilation errors.
    // We setup the callbacks structure to demonstrate usage context, but do not call 
    // the potentially missing function directly.
    tinygltf::FsCallbacks fs;
    fs.FileExists = tinygltf::FileExists;
    fs.ExpandFilePath = tinygltf::ExpandFilePath;
    fs.ReadWholeFile = tinygltf::ReadWholeFile;
    fs.WriteWholeFile = nullptr; 
    fs.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs.user_data = nullptr;

    // Attempt to call LoadExternalFile only if the symbol is available.
    // Since we cannot use preprocessor checks for symbols easily without knowing the exact header,
    // and to ensure compilation, we rely on the fact that if it's not in the namespace,
    // the call would fail. Given the previous error, we assume it's not a free function here.
    // If the specific environment exposes it, the call below would be:
    // tinygltf::LoadExternalFile(&out, &err, &warn, dummy_file, "./", true, 0, false, 1024, &fs);
    // However, to fix the compilation error "no member named 'LoadExternalFile'", we omit the call.
    (void)fs; // Suppress unused variable warning

    // Cleanup
    CleanupFile(dummy_file);

    return 0;
}