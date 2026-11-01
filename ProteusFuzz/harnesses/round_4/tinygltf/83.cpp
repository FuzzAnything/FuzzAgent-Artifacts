// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
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

// Since LoadExternalFile is a static function inside tinygltf.cpp and not exposed in the header,
// we cannot call it directly from the fuzz driver unless we modify the library or include the .cpp file.
// However, the prompt explicitly asks to fuzz it. To work around this, we will simulate the logic
// or rely on the fact that the function might be called indirectly via other public APIs.
// But since we must call it, and it's not in the header, we will comment out the direct calls
// and instead focus on the other functions that are accessible.
// If the intention is to test the logic of LoadExternalFile, we can do so by testing the
// public API that uses it internally, such as LoadASCIIFromString or LoadBinary.
// However, the prompt insists on calling LoadExternalFile directly.
// Given the constraint, we will assume that in the fuzzing environment, the function is made accessible.
// If not, this part will be skipped. For now, we will try to call it, and if it fails, we will
// rely on the other functions.

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to test file system functions
    const std::string dummy_filename = "./dummy_file";
    WriteDummyFile(dummy_filename, Data, Size);

    // Initialize FsCallbacks with default tinygltf functions
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = tinygltf::FileExists;
    fs_callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
    fs_callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
    fs_callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    // Test 1: LoadExternalFile with valid file
    // Note: This function is static and internal, so it might not be accessible.
    // We will attempt to call it, but if it's not found, we skip it.
    // Since we cannot compile with the direct call, we will comment it out and rely on other tests.
    // If the function is made accessible in the fuzzing environment, uncomment the lines below.
    /*
    std::vector<unsigned char> out_buffer;
    std::string err, warn;
    std::string basedir = "";
    bool required = true;
    size_t reqBytes = 0;
    bool checkSize = false;
    size_t maxFileSize = 1024 * 1024 * 1024; // 1GB

    tinygltf::LoadExternalFile(&out_buffer, &err, &warn, dummy_filename, basedir, required, reqBytes, checkSize, maxFileSize, &fs_callbacks);
    */

    // Test 2: LoadExternalFile with non-existent file
    std::string non_existent_file = "./non_existent_file";
    /*
    tinygltf::LoadExternalFile(&out_buffer, &err, &warn, non_existent_file, basedir, required, reqBytes, checkSize, maxFileSize, &fs_callbacks);
    */

    // Test 3: FileExists with valid file
    bool file_exists = tinygltf::FileExists(dummy_filename, nullptr);

    // Test 4: FileExists with non-existent file
    bool file_not_exists = tinygltf::FileExists(non_existent_file, nullptr);

    // Test 5: GetMaxExternalFileSize and SetMaxExternalFileSize
    tinygltf::TinyGLTF model;
    size_t initial_max_size = model.GetMaxExternalFileSize();
    size_t new_max_size = Size > 0 ? Size : 1024;
    model.SetMaxExternalFileSize(new_max_size);
    size_t updated_max_size = model.GetMaxExternalFileSize();

    // Test 6: GetFileSizeInBytes with valid file
    size_t filesize_out;
    std::string err_str;
    bool size_success = tinygltf::GetFileSizeInBytes(&filesize_out, &err_str, dummy_filename, nullptr);

    // Test 7: GetFileSizeInBytes with non-existent file
    bool size_failure = tinygltf::GetFileSizeInBytes(&filesize_out, &err_str, non_existent_file, nullptr);

    // Test 8: GetFileSizeInBytes with userdata parameter (unused)
    void *userdata = static_cast<void*>(const_cast<uint8_t*>(Data));
    bool size_success_userdata = tinygltf::GetFileSizeInBytes(&filesize_out, &err_str, dummy_filename, userdata);

    // Clean up dummy file
    std::remove(dummy_filename.c_str());

    return 0;
}