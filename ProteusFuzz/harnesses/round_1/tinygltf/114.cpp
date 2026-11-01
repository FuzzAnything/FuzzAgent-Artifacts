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
#include <tiny_gltf.h>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Helper implementations for utility functions that are static/internal in tinygltf
// and not exposed in the public namespace in some builds.
static std::string GetFilePathExtension(const std::string &FileName) {
    size_t dot_pos = FileName.find_last_of('.');
    if (dot_pos == std::string::npos) return "";
    if (dot_pos == 0) return ""; // Hidden file
    return FileName.substr(dot_pos + 1);
}

static std::string GetBaseDir(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) return "";
    return filepath.substr(0, pos + 1);
}

static std::string GetBaseFilename(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) return filepath;
    return filepath.substr(pos + 1);
}

static std::string JoinPath(const std::string &path0, const std::string &path1) {
    if (path0.empty()) return path1;
    if (path1.empty()) return path0;
    if (path0.back() == '/' || path0.back() == '\\') {
        return path0 + path1;
    }
    return path0 + "/" + path1;
}

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input to string for path manipulation functions
    std::string input_str(reinterpret_cast<const char *>(Data), Size);

    // 1. Test ExpandFilePath
    // userdata is typically nullptr
    std::string expanded = tinygltf::ExpandFilePath(input_str, nullptr);

    // 2. Test GetFilePathExtension (using local implementation if not in namespace)
    std::string ext = GetFilePathExtension(input_str);
    
    // 3. Test GetBaseDir
    std::string base_dir = GetBaseDir(input_str);
    
    // 4. Test GetBaseFilename
    std::string base_filename = GetBaseFilename(input_str);
    
    // 5. Test JoinPath
    std::string joined1 = JoinPath(input_str, input_str);
    std::string joined2 = JoinPath("", input_str);
    std::string joined3 = JoinPath(input_str, "");
    std::string joined4 = JoinPath(input_str, "test.gltf");

    // 6. Test ReadWholeFile
    // Write the fuzz input to a dummy file first
    const char *dummy_file = "./dummy_file";
    WriteDummyFile(dummy_file, Data, Size);

    std::vector<unsigned char> file_content;
    std::string error_msg;
    bool read_success = tinygltf::ReadWholeFile(&file_content, &error_msg, dummy_file, nullptr);

    // Cleanup: The file content vector is automatically cleaned up by RAII.
    // The dummy file is left on disk, which is acceptable for fuzzing environments.

    // Ensure we don't crash on any return values
    (void)expanded;
    (void)ext;
    (void)base_dir;
    (void)base_filename;
    (void)joined1;
    (void)joined2;
    (void)joined3;
    (void)joined4;
    (void)read_success;
    (void)error_msg;
    (void)file_content;

    return 0;
}