// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
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
#include <fstream>
#include "tiny_gltf.h"

static std::string GenerateStringFromData(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

static void WriteDummyFile(const std::string &filename, const uint8_t *Data, size_t Size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        if (Size > 0) {
            file.write(reinterpret_cast<const char*>(Data), Size);
        }
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const std::string dummy_filename = "./dummy_file";
    WriteDummyFile(dummy_filename, Data, Size);

    std::vector<unsigned char> out_buffer;
    std::string err;
    std::string warn;
    size_t filesize_out = 0;
    
    // FsCallbacks structure
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = nullptr;
    fs_callbacks.ExpandFilePath = nullptr;
    fs_callbacks.ReadWholeFile = nullptr;
    fs_callbacks.WriteWholeFile = nullptr;
    fs_callbacks.GetFileSizeInBytes = nullptr;
    fs_callbacks.user_data = nullptr;

    std::string test_path = GenerateStringFromData(Data, Size);

    // 1. Test GetBaseFilename (Free function in tinygltf namespace)
    // Note: In some versions of tinygltf, this might be a static method of Model or a free function.
    // Based on the error, it's not directly in the namespace as a free function in this build.
    // However, the task description says it is a function. Let's assume it's available as a free function 
    // or we need to access it differently. If it's truly missing, we skip it to avoid compilation errors.
    // Given the error message, it seems it's not exposed as `tinygltf::GetBaseFilename`.
    // We will comment it out or try to find an alternative if the API description implies it exists.
    // Since the prompt explicitly lists it, but the compiler says no, we assume it might be internal or 
    // the prompt description is slightly off regarding the namespace exposure.
    // To fix compilation, we remove the direct call if it's not found.
    // However, to adhere to the task, we assume the function exists but maybe the header version is different.
    // Let's try to call it if it's a free function, but if the compiler errors, we must omit it.
    // Since I must provide compiling code, I will omit the call that causes the error.
    // std::string base_name = tinygltf::GetBaseFilename(test_path);
    // (void)base_name;

    // 2. Test ExpandFilePath (Free function in namespace)
    std::string expanded_path = tinygltf::ExpandFilePath(test_path, nullptr);
    (void)expanded_path;

    // 3. Test FileExists (Free function)
    bool exists = tinygltf::FileExists(dummy_filename, nullptr);
    (void)exists;
    bool exists_test_path = tinygltf::FileExists(test_path, nullptr);
    (void)exists_test_path;

    // 4. Test GetFileSizeInBytes (Free function)
    err.clear();
    bool size_ok = tinygltf::GetFileSizeInBytes(&filesize_out, &err, dummy_filename, nullptr);
    (void)size_ok;
    err.clear();
    bool size_ok_test_path = tinygltf::GetFileSizeInBytes(&filesize_out, &err, test_path, nullptr);
    (void)size_ok_test_path;

    // 5. Test ReadWholeFile (Free function)
    out_buffer.clear();
    err.clear();
    bool read_ok = tinygltf::ReadWholeFile(&out_buffer, &err, dummy_filename, nullptr);
    (void)read_ok;
    out_buffer.clear();
    err.clear();
    bool read_ok_test_path = tinygltf::ReadWholeFile(&out_buffer, &err, test_path, nullptr);
    (void)read_ok_test_path;

    // 6. Test LoadExternalFile
    // The error indicates `LoadExternalFile` is not in `tinygltf` namespace as a free function.
    // In many versions of tinygltf, this is a static method of the `Model` class or a private helper.
    // However, the prompt says it's a function. If it's not accessible, we cannot call it directly.
    // Let's check if it's a static method of `tinygltf::Model`.
    // If the prompt insists on it being a free function, but the code doesn't compile, 
    // it's likely a private static method. We cannot call private methods from outside.
    // Therefore, we must skip direct calls to `LoadExternalFile` if it's not public.
    // But wait, the prompt says "Target API Functions" and lists it. 
    // Perhaps it's exposed in a different way or the version used in the fuzzer has it.
    // Since the compiler error is explicit, we must remove the calls to `LoadExternalFile` 
    // to make the code compile, unless we can find the correct scope.
    // Given the constraints, I will remove the calls to `LoadExternalFile` and `GetBaseFilename` 
    // as they are not accessible in the provided environment, focusing on the other functions.
    
    // Attempting to call LoadExternalFile as a static method of Model if possible?
    // The prompt signature doesn't show a class scope.
    // If it's truly not accessible, we skip it.
    
    // Let's try to define a dummy call if we can't access the real one, but that defeats the purpose.
    // The safest bet for a compiling driver is to only call what is accessible.
    // However, the task requires testing these functions. 
    // Maybe the function is `tinygltf::LoadExternalFile` but requires a specific macro or version.
    // Since I cannot change the library, I must adapt the driver.
    // I will remove the non-compiling calls.

    // Re-evaluating: The prompt says "Target API Functions". If they don't exist in the namespace,
    // the prompt might be describing internal functions or a different version.
    // To ensure the driver compiles and runs, I will only call the functions that are confirmed to exist:
    // ExpandFilePath, FileExists, GetFileSizeInBytes, ReadWholeFile.
    
    // If the user insists on testing LoadExternalFile, they might need to expose it or use a different method.
    // For now, I will omit the calls that cause compilation errors.

    return 0;
}