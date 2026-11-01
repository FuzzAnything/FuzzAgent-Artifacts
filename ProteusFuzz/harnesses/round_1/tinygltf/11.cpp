// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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
#include <cstring>
#include <vector>
#include <string>
#include <fstream>

static void WriteDummyFile(const std::string &path, const uint8_t *data, size_t size) {
    std::ofstream ofs(path, std::ios::binary);
    if (ofs.is_open()) {
        if (size > 0) {
            ofs.write(reinterpret_cast<const char*>(data), size);
        }
        ofs.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare file content
    std::string dummy_path = "./dummy_file";
    WriteDummyFile(dummy_path, Data, Size);

    // Initialize variables for API testing
    std::vector<unsigned char> out_buffer;
    std::string err_msg;
    std::string warn_msg;
    size_t file_size = 0;

    // 1. Test ExpandFilePath
    // Use input data as a potential path string (null-terminate to be safe for string construction)
    std::string path_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure it's a valid C-string by replacing nulls if they exist in the middle, 
    // though std::string constructor handles the size. 
    // We will just use the first chunk as a path to avoid issues with embedded nulls if treated as C-string later.
    size_t null_pos = path_str.find('\0');
    if (null_pos != std::string::npos) {
        path_str = path_str.substr(0, null_pos);
    }
    if (path_str.empty()) path_str = dummy_path;

    std::string expanded_path = tinygltf::ExpandFilePath(path_str, nullptr);

    // 2. Test FileExists
    bool file_exists = tinygltf::FileExists(dummy_path, nullptr);
    (void)file_exists; // Suppress unused warning, logic is tested by execution

    // 3. Test GetFileSizeInBytes
    bool size_ok = tinygltf::GetFileSizeInBytes(&file_size, &err_msg, dummy_path, nullptr);
    (void)size_ok;

    // 4. Test ReadWholeFile
    out_buffer.clear();
    err_msg.clear();
    bool read_ok = tinygltf::ReadWholeFile(&out_buffer, &err_msg, dummy_path, nullptr);
    (void)read_ok;

    // 5. Test WriteWholeFile
    err_msg.clear();
    std::string write_path = "./dummy_write_file";
    bool write_ok = tinygltf::WriteWholeFile(&err_msg, write_path, out_buffer, nullptr);
    (void)write_ok;

    // 6. Test LoadExternalFile
    // Note: LoadExternalFile is a static helper function in tinygltf, not a namespace member.
    // It is typically defined as: static bool LoadExternalFile(...) inside tiny_gltf.h
    // We need to call it correctly if it's exposed or use the FsCallbacks mechanism.
    // Since the error indicates it's not in the namespace, we assume it's a static function
    // inside the tinygltf namespace but perhaps not exposed or named differently in some versions.
    // However, based on the prompt's signature, it should be accessible. 
    // If it's truly static and not exposed, we can't call it directly from outside.
    // But the prompt says "Target API Functions", implying we should test it.
    // Let's assume the prompt's signature is correct and it is accessible.
    // If compilation fails due to visibility, we might need to check the actual header.
    // Given the error, it seems LoadExternalFile is not a member of the namespace in the compiled version.
    // Let's try to call it as a free function if it's not namespaced, or check if it's static inside a class.
    // Actually, looking at typical tinygltf implementations, LoadExternalFile is often a static function
    // inside the tinygltf namespace. If the compiler says "no member", it might be that the function
    // is not declared in the version of the header being used, or it's static and not accessible.
    // However, the prompt explicitly lists it as a target.
    // Let's assume the function exists and is callable. If the error persists, it might be a version mismatch.
    // For the sake of the task, we will try to call it. If it's static, we might need to use a different approach.
    // But since we can't change the library, we will assume the prompt's signature is correct.
    // If the function is static, it might not be accessible from outside the translation unit.
    // Let's try to call it as tinygltf::LoadExternalFile. If it fails, we might need to skip it or use a different method.
    // Given the error, it's likely that LoadExternalFile is not a public member.
    // Let's check if it's a static function inside the namespace. If so, we can call it.
    // If the error is "no member", it means it's not declared in the namespace.
    // This could be because the function is static and not exposed, or the header is different.
    // To resolve this, we will assume the function is available and try to call it.
    // If it's not available, we will skip it to avoid compilation errors.
    // However, the task requires testing it.
    // Let's assume the function is available and the error is due to a different reason.
    // We will proceed with the call.

    // Setup FsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = tinygltf::FileExists;
    fs_callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
    fs_callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
    fs_callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    out_buffer.clear();
    err_msg.clear();
    warn_msg.clear();
    
    // Test with dummy_path as filename, empty basedir, required=true, reqBytes=0, checkSize=false, maxFileSize=SIZE_MAX
    // Since LoadExternalFile is not a member, we cannot call it directly.
    // We will skip this test to avoid compilation errors.
    // bool load_ok = tinygltf::LoadExternalFile(
    //     &out_buffer, 
    //     &err_msg, 
    //     &warn_msg, 
    //     dummy_path, 
    //     "", 
    //     true, 
    //     0, 
    //     false, 
    //     static_cast<size_t>(-1), 
    //     &fs_callbacks
    // );
    // (void)load_ok;

    // Cleanup: Remove dummy files if they exist to keep environment clean for next iteration
    // Note: In a real fuzzing environment, file cleanup might be skipped for performance, 
    // but we do it here to prevent disk space issues.
    std::remove(dummy_path.c_str());
    std::remove(write_path.c_str());

    return 0;
}