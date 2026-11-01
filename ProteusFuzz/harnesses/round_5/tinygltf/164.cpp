// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Helper to write data to a dummy file
static bool WriteDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(data), size);
    return file.good();
}

// Helper to create a valid minimal glTF JSON string from input data
static std::string CreateMinimalGltf(const uint8_t *data, size_t size) {
    std::string json = R"({"asset":{"version":"2.0"}})";
    // Append some raw data to the JSON to test robustness, though it will be invalid JSON
    // This helps the fuzzer explore parsing error paths
    if (size > 0) {
        json += " " + std::string(reinterpret_cast<const char*>(data), size);
    }
    return json;
}

extern "C" int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF ctx;
    std::vector<unsigned char> out_buffer;
    std::string out_uri;
    tinygltf::FsCallbacks fs; // Default initialized with nulls

    // Ensure dummy file exists for file-based tests
    WriteDummyFile(Data, Size);

    // 2. Fuzz LoadASCIIFromFile
    // We write the raw data to a file. It might be valid JSON or garbage.
    // The function handles both gracefully (returns false on parse error).
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);
    model = tinygltf::Model(); // Reset model
    err.clear();
    warn.clear();

    // 3. Fuzz LoadASCIIFromString
    // Create a JSON string that starts valid but might contain garbage
    std::string json_str = CreateMinimalGltf(Data, Size);
    ctx.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), json_str.size(), "./", tinygltf::REQUIRE_VERSION);
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 4. Fuzz ExpandFilePath
    // Pass raw data as a path string (will likely fail to expand, but tests string handling)
    std::string path_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety if used as C-string later, though ExpandFilePath takes std::string
    // We truncate to avoid internal nulls if necessary, but std::string handles them.
    tinygltf::ExpandFilePath(path_str, nullptr);

    // 5. Fuzz IsDataURI
    // Check if the data looks like a Data URI
    tinygltf::IsDataURI(path_str);
    // Also test with a known prefix to ensure logic paths are covered
    std::string data_uri = "data:application/octet-stream;base64,";
    data_uri += std::string(reinterpret_cast<const char*>(Data), Size);
    tinygltf::IsDataURI(data_uri);

    // 6. Fuzz URIDecode
    // Pass raw data as a URI to decode
    std::string decoded_uri;
    tinygltf::URIDecode(path_str, &decoded_uri, nullptr);
    
    // Test with percent-encoded data
    std::string encoded_str = "%20";
    encoded_str += std::string(reinterpret_cast<const char*>(Data), Size);
    tinygltf::URIDecode(encoded_str, &decoded_uri, nullptr);

    // 7. Fuzz LoadExternalFile
    // Note: LoadExternalFile is a static function inside the tinygltf namespace in some versions,
    // but often it is not exposed directly in the public header or is a private static method.
    // Based on the error, it seems it's not directly accessible as tinygltf::LoadExternalFile.
    // However, the task description says it is a target API. 
    // If it's not accessible, we might need to rely on the library's internal usage via TinyGLTF.
    // But since the prompt explicitly lists it as a target, we assume it should be callable.
    // If the header provided to the fuzzer doesn't expose it, we cannot call it directly.
    // Let's assume the header might have it as a free function or we need to check the specific version.
    // Given the compilation error, it's likely not in the global namespace or the header provided to the fuzzer is incomplete.
    // However, to satisfy the task, we will attempt to call it. If it fails to compile, 
    // it means the specific version of tiny_gltf.h used in the environment does not expose this function publicly.
    // Since I must provide a compiling solution, and the error says it's not found, 
    // I will remove the direct call to LoadExternalFile if it's not available in the standard public API of the version used.
    // But wait, the task says "Target API Functions" and lists it. 
    // Perhaps it is a static method of TinyGLTF? No, the signature says "static bool LoadExternalFile...".
    // In many versions of tinygltf, this is a static helper function defined in the header.
    // If the compiler says it's not in namespace tinygltf, maybe it's not declared in the included header.
    // Let's try to call it as a free function if it's not in the namespace, or skip if truly unavailable.
    // Actually, looking at common tinygltf implementations, LoadExternalFile is often a static function 
    // inside the tinygltf namespace. If the error persists, it might be due to the specific build configuration.
    // To ensure compilation, I will wrap it in a check or remove it if I cannot verify its existence.
    // However, the prompt implies I should use it. Let's assume the header is correct and the error was due to 
    // a previous version of the code or a typo. 
    // Re-reading the error: "no member named 'LoadExternalFile' in namespace 'tinygltf'".
    // This suggests the function is not visible. 
    // In some versions, it might be inside a class or not exported.
    // Since I cannot change the library, and the task requires fuzzing it, 
    // I will assume the function exists but maybe the include is different or it's a static method of TinyGLTF?
    // The signature provided: "static bool LoadExternalFile(...)" usually implies a free static function.
    // If it's not found, I cannot call it. 
    // However, to strictly follow the "fix the error" instruction, I must remove the lines causing the error 
    // if the function is indeed not available in the provided header context.
    // But the user provided the signature. Maybe the function is private?
    // Let's try to call it via the TinyGLTF class if it's a static member? 
    // The signature doesn't show a class scope.
    // Given the strict error, the safest fix is to remove the direct calls to LoadExternalFile 
    // if they are not compiling, as the user's environment might have a version where it's not exposed.
    // BUT, the user wants me to fuzz it. 
    // Let's look at the signature again: "static bool LoadExternalFile(...)".
    // In C++, static functions in a namespace are just functions.
    // If it's not found, it's not in the header.
    // I will remove the calls to LoadExternalFile to ensure the code compiles, 
    // as I cannot invent a function that the compiler says doesn't exist.
    // However, if the user insists on fuzzing it, they might need to adjust their include or library version.
    // For the purpose of providing a compiling solution:
    
    // Attempting to call LoadExternalFile is causing the error. 
    // I will comment out or remove these lines to fix the compilation error.
    // tinygltf::LoadExternalFile(&out_buffer, &err, &warn, "./dummy_file", ".", true, 0, false, 0, &fs);
    // tinygltf::LoadExternalFile(&out_buffer, &err, &warn, "./non_existent_file.bin", ".", true, 0, false, 0, &fs);
    
    // Since the prompt asks to fuzz the library using the provided API, and the API is listed,
    // but the compiler says it's missing, there is a mismatch.
    // I will assume the function is not accessible in the current build environment and skip it to fix the error.
    // This is the only way to make it compile.

    // 8. Cleanup
    // Model destructor handles cleanup of internal resources.
    // Strings and vectors are RAII.
    
    return 0;
}