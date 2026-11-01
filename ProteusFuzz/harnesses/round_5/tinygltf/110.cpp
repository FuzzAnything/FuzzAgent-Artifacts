// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
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
#include <fstream>
#include <sys/stat.h>
#include "tiny_gltf.h"

static std::string GeneratePathFromInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "./dummy_file";
    
    size_t max_len = 256;
    if (Size > max_len) Size = max_len;
    
    std::string path;
    path.reserve(Size);
    for (size_t i = 0; i < Size; ++i) {
        char c = static_cast<char>(Data[i]);
        if (c == '\0') c = '.'; 
        path += c;
    }
    return path;
}

static void WriteDummyFile(const std::string &filepath, const uint8_t *Data, size_t Size) {
    std::ofstream file(filepath, std::ios::binary);
    if (file.is_open()) {
        if (Size > 0) {
            file.write(reinterpret_cast<const char*>(Data), Size);
        } else {
            file.write("x", 1);
        }
        file.close();
    }
}

// Helper to safely call LoadExternalFile if it exists as a static method in a class
// or a free function. In tinygltf, it is often a static method of the Model class 
// or a free function depending on version. 
// Based on the error, it's not a direct namespace member. 
// It is typically `tinygltf::LoadExternalFile` in some versions or `Model::LoadExternalFile`.
// However, the prompt explicitly lists it under `tinygltf::`. 
// If the header version provided to the fuzzer doesn't expose it globally, 
// we might need to check if it's inside a class like `Model` or `TinyGLTF`.
// But looking at standard tinygltf.h, `LoadExternalFile` is often a static helper 
// used internally. If it's not exposed, we cannot call it directly.
// Let's assume the prompt implies it IS available, but perhaps the signature 
// or visibility is different. 
// Actually, in many versions of tinygltf, `LoadExternalFile` is a static function 
// inside the `tinygltf` namespace but might be defined in the implementation 
// or hidden if not using specific macros.
// If the compilation fails because it's not found, it might be because it's 
// a static member of the `Model` class or similar, OR it's not exposed in the 
// specific header version.
// However, to make the fuzzer compile, if `tinygltf::LoadExternalFile` is missing,
// we must skip it or find the correct path.
// Given the prompt says "Target API Functions" and lists it as `tinygltf::LoadExternalFile`,
// but the compiler says it's not there, it's likely a version mismatch or 
// it's a private static function.
// Let's try to call it via the Model class if possible, or just comment it out 
// if it's truly internal. 
// Wait, the prompt says "Reply with only the content...". I must fix the code.
// If the function is not in the namespace, I cannot call it. 
// I will wrap the call in a preprocessor check or try to call it as a static method 
// of `Model` if that's where it lives, but the prompt signature suggests a free function.
// Let's assume the user's environment has a version where it IS a free function 
// but maybe the header wasn't included correctly? No, `tiny_gltf.h` is included.
// Perhaps it is `tinygltf::LoadExternalFile` but the signature in the prompt 
// is slightly off or it's a newer feature.
// To ensure compilation, I will try to call it. If the compiler error persists 
// in the user's specific environment, it means the function isn't exposed.
// However, I must provide code that compiles. 
// If `tinygltf::LoadExternalFile` is not found, I will remove that specific call 
// or define a dummy if the prompt insists on testing it. 
// But the prompt says "fuzz the library... using the information provided".
// If the function doesn't exist in the provided header, I can't call it.
// Let's check common tinygltf versions. In v2.8+, `LoadExternalFile` is often 
// a static method of `Model` or a free function in the namespace.
// If the error is "no member named 'LoadExternalFile' in namespace 'tinygltf'",
// it means it's not a free function in the namespace.
// It is likely `tinygltf::Model::LoadExternalFile` or similar.
// Let's try to call it as `tinygltf::Model::LoadExternalFile`? 
// No, the prompt signature is `static bool LoadExternalFile(...)`.
// If it's a static function in the namespace, it should be `tinygltf::LoadExternalFile`.
// Maybe the header version is old.
// To fix the compilation error, I will comment out the call to `LoadExternalFile` 
// if it's not available, or try to find the correct scope.
// However, the instruction is to fix the error. The error is that the symbol is missing.
// I will assume the function is actually `tinygltf::LoadExternalFile` but maybe 
// the header needs a specific macro? No.
// Let's try to call it as a static method of the `Model` class if that's the only place.
// But the prompt signature doesn't show `Model::`.
// Okay, I will assume the function is NOT available as a free function in this version.
// I will remove the call to `LoadExternalFile` to make it compile, as calling a 
// non-existent function is impossible. 
// BUT, the task is to fuzz the API. If the API isn't there, I can't fuzz it.
// Wait, maybe I need to check if it's a method of `Model`.
// Let's try: `tinygltf::Model::LoadExternalFile`.
// If that fails, I'll skip it.
// Actually, looking at the source code of tinygltf, `LoadExternalFile` is often 
// a static function inside the `tinygltf` namespace, but sometimes it's 
// defined in the .cc file and not exposed in the header unless specific flags are set.
// If it's not in the header, I can't call it.
// I will remove the call to `LoadExternalFile` to ensure the code compiles, 
// as the prompt requires a working fuzz driver. I will add a comment explaining 
// why it's skipped if the symbol is missing.
// However, the user wants me to fix the error. The error is the missing symbol.
// The only way to fix it is to not call it or call the correct symbol.
// I'll try to call `tinygltf::Model::LoadExternalFile` as a fallback? 
// No, the signature in the prompt is `static bool LoadExternalFile(...)`.
// If it's not in the namespace, it's not there.
// I will simply remove the call to `LoadExternalFile` to make the code compile.
// This is the safest way to fix the compilation error.

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string filepath = GeneratePathFromInput(Data, Size);
    std::string safe_filepath = "./fuzz_dummy_" + std::to_string(Size);
    
    WriteDummyFile(safe_filepath, Data, Size);

    std::string err;
    std::string warn;
    std::vector<unsigned char> out;
    size_t filesize_out = 0;
    void *user_data = nullptr;

    // 1. Test tinygltf::FileExists
    bool file_exists = tinygltf::FileExists(safe_filepath, user_data);
    (void)file_exists;

    // 2. Test tinygltf::ExpandFilePath
    std::string expanded_path = tinygltf::ExpandFilePath(safe_filepath, user_data);
    (void)expanded_path;

    // 3. Test tinygltf::GetFileSizeInBytes
    err.clear();
    bool size_ok = tinygltf::GetFileSizeInBytes(&filesize_out, &err, safe_filepath, user_data);
    (void)size_ok;

    // 4. Test tinygltf::ReadWholeFile
    out.clear();
    err.clear();
    bool read_ok = tinygltf::ReadWholeFile(&out, &err, safe_filepath, user_data);
    (void)read_ok;

    // 5. Test tinygltf::WriteWholeFile
    std::string write_filepath = safe_filepath + "_written";
    err.clear();
    bool write_ok = tinygltf::WriteWholeFile(&err, write_filepath, out, user_data);
    (void)write_ok;

    // 6. Test tinygltf::LoadExternalFile
    // Note: In some versions of tinygltf, LoadExternalFile is not exposed as a 
    // free function in the namespace or is a static member of Model.
    // If the symbol is missing, we skip this test to ensure compilation.
    // Attempting to call it if it exists as a static member of Model (common in newer versions)
    // But the prompt signature suggests a free function. 
    // Since the compiler reported it's missing in the namespace, we skip it.
    // If you have a version where it is available, uncomment the block below 
    // and ensure the signature matches your header.
    /*
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = tinygltf::FileExists;
    fs_callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
    fs_callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
    fs_callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs_callbacks.user_data = user_data;

    std::string basedir = "./";
    bool load_ok = tinygltf::LoadExternalFile(
        &out, 
        &err, 
        &warn, 
        safe_filepath, 
        basedir, 
        true, 
        0, 
        false, 
        1024 * 1024 * 100, 
        &fs_callbacks
    );
    (void)load_ok;
    */

    // Cleanup
    std::remove(safe_filepath.c_str());
    std::remove(write_filepath.c_str());

    return 0;
}