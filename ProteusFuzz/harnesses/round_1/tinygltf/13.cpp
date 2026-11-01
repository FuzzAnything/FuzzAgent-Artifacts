// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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
#include <fstream>
#include <string>
#include <vector>

static void WriteDummyFile(const uint8_t *Data, size_t Size, const char *filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF parser;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz SetStoreOriginalJSONForExtrasAndExtensions
    // Toggle based on first byte to explore different states
    bool enableOriginalJSON = (Data[0] % 2 == 0);
    parser.SetStoreOriginalJSONForExtrasAndExtensions(enableOriginalJSON);

    // Prepare dummy file content
    const char *dummyFile = "./dummy_file";
    WriteDummyFile(Data, Size, dummyFile);

    // Fuzz LoadASCIIFromFile
    // Use different check_sections values based on input data
    unsigned int checkSectionsAsciiFile = (Size > 1) ? (Data[1] % 3) : 0;
    parser.LoadASCIIFromFile(&model, &err, &warn, dummyFile, checkSectionsAsciiFile);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Fuzz LoadBinaryFromFile
    unsigned int checkSectionsBinaryFile = (Size > 2) ? (Data[2] % 3) : 0;
    parser.LoadBinaryFromFile(&model, &err, &warn, dummyFile, checkSectionsBinaryFile);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Fuzz LoadASCIIFromString (Overload 1: with length)
    // Ensure we have enough data to treat as string
    std::string strData(reinterpret_cast<const char*>(Data), Size);
    unsigned int checkSectionsStringLen = (Size > 3) ? (Data[3] % 3) : 0;
    parser.LoadASCIIFromString(&model, &err, &warn, strData.c_str(), strData.length(), "", checkSectionsStringLen);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Fuzz LoadASCIIFromString (Overload 2: without explicit length, uses strlen)
    // We pass the same string but rely on the function's internal strlen logic
    unsigned int checkSectionsStringStrlen = (Size > 4) ? (Data[4] % 3) : 0;
    parser.LoadASCIIFromString(&model, &err, &warn, strData.c_str(), strData.length(), "", checkSectionsStringStrlen);
    // Note: The second overload signature in the prompt description implies:
    // bool LoadASCIIFromString(Model *model, std::string *err, std::string *warn,
    //                          const char *str, const unsigned int length,
    //                          const std::string &base_dir,
    //                          unsigned int check_sections = REQUIRE_VERSION);
    // The prompt asks to invoke "all 2 overloads". However, the provided signature block
    // only shows one explicit signature with 'length' and one comment block describing the same.
    // Often tinygltf has an overload without 'length' that calculates it, or the prompt implies
    // testing the default argument behavior vs explicit.
    // Given the signature provided:
    // 1. LoadASCIIFromString(..., const char *str, unsigned int length, ...)
    // The prompt text says "You should invoke all 2 overloads above for this function."
    // Looking closely at the provided text, there are two blocks for LoadASCIIFromString.
    // Block 1: bool TinyGLTF::LoadASCIIFromString(Model *model, std::string *err, std::string *warn, const char *str, unsigned int length, const std::string &base_dir, unsigned int check_sections);
    // Block 2: bool LoadASCIIFromString(Model *model, std::string *err, std::string *warn, const char *str, const unsigned int length, const std::string &base_dir, unsigned int check_sections = REQUIRE_VERSION);
    // These look like the same function (member vs non-member or just duplicate docs).
    // However, standard tinygltf usually has:
    // 1. LoadASCIIFromString(..., const char *str, unsigned int length, ...)
    // 2. LoadASCIIFromString(..., const std::string &str, ...) OR similar.
    // Since the prompt explicitly lists two blocks and asks to invoke both, and the signatures provided are identical in parameters (just different default value visibility or namespace qualification in the text),
    // I will interpret this as invoking the function twice with different parameters to maximize coverage, or if the library actually has a string overload not fully detailed in the signature block but implied by "2 overloads".
    // Wait, looking at the text again:
    // "You should invoke all 2 overloads above for this function."
    // The text shows:
    // 1. bool TinyGLTF::LoadASCIIFromString(Model *model, std::string *err, std::string *warn, const char *str, unsigned int length, const std::string &base_dir, unsigned int check_sections);
    // 2. bool LoadASCIIFromString(Model *model, std::string *err, std::string *warn, const char *str, const unsigned int length, const std::string &base_dir, unsigned int check_sections = REQUIRE_VERSION);
    // These are effectively the same signature. The "2 overloads" might refer to the fact that `check_sections` has a default value, allowing two ways to call it?
    // Or perhaps the prompt implies the standard library has a `std::string` version.
    // To be safe and strictly follow "invoke all 2 overloads above", I will call the function twice with different `check_sections` values or base_dir to ensure distinct execution paths if the compiler treats them as the same function, or if there is a hidden overload.
    // Actually, let's re-read the prompt's signature block carefully.
    // It lists `TinyGLTF::LoadASCIIFromString` and then `LoadASCIIFromString`.
    // If the library actually has a version taking `const std::string&` instead of `const char*`, the prompt description might be slightly off in the signature block but correct in the intent.
    // However, I must stick to the provided signatures. The provided signatures are identical.
    // I will call it twice with different parameters to simulate "diverse" invocation as requested in step 3.
    
    // Let's assume the "2 overloads" refers to the ability to call with or without the explicit `check_sections` (using default) vs explicit.
    // Call 1: Explicit check_sections
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    parser.LoadASCIIFromString(&model, &err, &warn, strData.c_str(), strData.length(), "", 0);

    // Call 2: Using default check_sections (REQUIRE_VERSION)
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    parser.LoadASCIIFromString(&model, &err, &warn, strData.c_str(), strData.length(), "");

    // Fuzz RemoveImageLoader
    parser.RemoveImageLoader();

    return 0;
}