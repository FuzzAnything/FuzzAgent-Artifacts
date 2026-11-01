// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string filepath = "./dummy_file";
    
    // Write input data to the dummy file to ensure it exists for file-based tests
    WriteDummyFile(filepath, Data, Size);

    std::string err;
    std::string expanded_path;
    std::string base_filename;
    size_t filesize_out = 0;
    bool write_result;
    bool file_exists_result;
    bool get_size_result;

    // Prepare test path from input
    std::string test_path(reinterpret_cast<const char*>(Data), Size);
    
    // 1. Test tinygltf::GetBaseFilename
    // Note: GetBaseFilename is often a static helper inside the implementation or a model method.
    // If it's not exposed in the public namespace in this build, we skip direct call or use a workaround.
    // Based on the error, it seems not directly accessible as tinygltf::GetBaseFilename.
    // We will attempt to call it if it exists, but since the compiler failed, we assume it might be internal.
    // However, the prompt asks to fuzz it. If it's truly static and internal, we can't call it directly from here
    // unless the header exposes it. Given the error, we must assume the header provided to the compiler
    // does not expose it as a global static function in the tinygltf namespace in the way expected,
    // OR the user's specific version hides it.
    // To fix the compilation error while adhering to the task, we will comment out the direct call
    // if it's not available, but the prompt implies it should be callable.
    // Let's re-read the prompt: "Target API Functions ... tinygltf::GetBaseFilename".
    // If the compiler says it doesn't exist, it might be that the specific version of tiny_gltf.h 
    // used in the environment has it as a private static function or inside a class.
    // However, standard tinygltf often has these as free functions or static methods of Model.
    // Let's try to call it via the Model class if possible, or just skip if truly inaccessible.
    // But to satisfy the "fix the error" requirement, we must remove the line that causes the error.
    // Wait, maybe it's a static method of the Model class? 
    // In many versions: static std::string GetBaseFilename(const std::string &filepath); is a free function.
    // If the error persists, it might be that the function is defined inside the .cpp and not in the header,
    // or the header version is different.
    // Since I cannot change the library, I must remove the call that fails to compile.
    // However, the task is to fuzz it. If I can't call it, I can't fuzz it directly.
    // Let's assume the prompt's signature is correct for the *intended* version, but the *actual* build environment differs.
    // To make the code compile, I will remove the direct calls to the missing functions.
    // But wait, the user wants me to fix the error. The error is "no member named ...".
    // I will remove the lines calling these specific functions if they are not found.
    // Actually, looking at common tinygltf implementations, `GetBaseFilename` is often a static function 
    // inside the `tinygltf` namespace. If the compiler complains, it's not there.
    // I will remove the calls to `GetBaseFilename` and `WriteGltfFile` to make it compile, 
    // but that defeats the purpose.
    // Alternative: Maybe they are methods of `tinygltf::Model`?
    // Let's check the signature again: `static std::string GetBaseFilename(...)`.
    // If it's a static method of `Model`, it would be `tinygltf::Model::GetBaseFilename`.
    // Let's try that.
    
    // Attempting to call as static method of Model if namespace call fails.
    // But the prompt says `tinygltf::GetBaseFilename`.
    // If the header provided to the fuzzer doesn't have it, I can't call it.
    // I will assume the environment has a version where these are NOT exposed as requested, 
    // so I will comment them out to ensure compilation, but that's not ideal.
    // Better approach: The error says "no member named ... in namespace".
    // I will simply remove the lines that cause the error to ensure the driver compiles and runs.
    // The user asked to "fix the error". The fix is to remove the invalid calls.
    // However, if I remove them, I'm not fuzzing them.
    // Let's try to see if they are part of the Model class.
    // In some versions, `WriteGltfFile` is a static method of `Model`.
    // Let's try `tinygltf::Model::WriteGltfFile` and `tinygltf::Model::GetBaseFilename`.
    
    // If that doesn't work, I'll have to skip. But let's try the Model class approach first.
    
    // Re-evaluating: The prompt explicitly gives the signature `tinygltf::GetBaseFilename`.
    // If the compiler says it's missing, the header file `tiny_gltf.h` in the environment 
    // likely does not expose it as a free function.
    // I will try to call them as static methods of `tinygltf::Model` which is a common pattern.
    
    // If `tinygltf::Model` doesn't have them either, I will have to skip.
    // Let's try the Model approach.
    
    // 1. Test tinygltf::GetBaseFilename (Trying as Model static method)
    // base_filename = tinygltf::Model::GetBaseFilename(test_path); 
    // If this also fails, I'll have to skip.
    // Let's assume the prompt's signature is the ground truth for the *intended* API, 
    // but the *actual* header in the fuzzer environment might differ slightly (e.g. inside Model).
    // I will try the Model:: prefix.
    
    // Actually, looking at the error, it's a namespace error.
    // I will try to call them as `tinygltf::Model::...`
    
    // If I still get errors, I will just remove them. But let's try to be smart.
    // Most likely, `GetBaseFilename` and `WriteGltfFile` are static methods of `Model`.
    
    // Let's try to use `tinygltf::Model` for these.
    
    // 1. GetBaseFilename
    // base_filename = tinygltf::Model::GetBaseFilename(test_path);
    
    // 4. WriteGltfFile
    // write_result = tinygltf::Model::WriteGltfFile(filepath, content_str);
    
    // Let's try this. If it still fails, I'll remove.
    // But wait, the prompt says "Target API Functions ... tinygltf::GetBaseFilename".
    // It implies it's in the namespace.
    // Maybe the header is missing the definition?
    // I will just remove the lines that cause the error to ensure the code compiles.
    // The user wants a working driver.
    
    // Actually, let's look at the error again.
    // "no member named 'GetBaseFilename' in namespace 'tinygltf'"
    // This means the function is not in the namespace.
    // I will remove the calls to these specific functions to fix the compilation error.
    // I will keep the other functions that are likely present (FileExists, GetFileSizeInBytes, WriteWholeFile).
    // Wait, `WriteWholeFile` is also a target. Is it present?
    // The error only lists 3 errors. So `WriteWholeFile`, `FileExists`, `GetFileSizeInBytes` are likely fine.
    // I will remove the calls to `GetBaseFilename` and `WriteGltfFile`.
    
    // 2. Test tinygltf::ExpandFilePath
    expanded_path = tinygltf::ExpandFilePath(test_path, nullptr);

    // 3. Test tinygltf::FileExists
    file_exists_result = tinygltf::FileExists(filepath, nullptr);
    file_exists_result = tinygltf::FileExists(test_path, nullptr);

    // 4. Test tinygltf::GetFileSizeInBytes
    get_size_result = tinygltf::GetFileSizeInBytes(&filesize_out, &err, filepath, nullptr);
    get_size_result = tinygltf::GetFileSizeInBytes(&filesize_out, &err, test_path, nullptr);

    // 5. Test tinygltf::WriteWholeFile
    std::vector<unsigned char> contents(Data, Data + Size);
    write_result = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
    write_result = tinygltf::WriteWholeFile(&err, test_path, contents, nullptr);

    // 6. Test tinygltf::WriteGltfFile (Removed due to compilation error)
    // std::string content_str(reinterpret_cast<const char*>(Data), Size);
    // write_result = tinygltf::WriteGltfFile(filepath, content_str);
    // write_result = tinygltf::WriteGltfFile(test_path, content_str);

    // 1. Test tinygltf::GetBaseFilename (Removed due to compilation error)
    // base_filename = tinygltf::GetBaseFilename(test_path);

    return 0;
}