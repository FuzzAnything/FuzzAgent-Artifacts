// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

// Define function pointer types matching the expected signatures in tinygltf
// Based on the error, the encode function expects an extra const std::string argument (the original URI)
// Signature: bool (const std::string& original, const std::string& encoded, std::string* out, void* user_data)
// However, standard tinygltf URICallbacks usually define:
// typedef bool (*URIEncodeFunction)(const std::string& in_uri, std::string* out_uri, void* user_data);
// Let's check the error message again: 
// 'no known conversion from 'bool (*)(const std::string &, std::string *, void *)' ... to 'function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'
// This implies the struct member expects a function with 4 arguments: (const string, const string, string*, void*)
// But the standard tinygltf header usually defines URICallbacks with 3 args. 
// The error suggests the specific version of tinygltf being used has a different signature for encode.
// Let's assume the signature is: bool (const std::string& in_uri, const std::string& /*unused?*/, std::string* out_uri, void* user_data)
// Or perhaps the error message is slightly misleading and it's actually:
// bool (const std::string& in_uri, std::string* out_uri, void* user_data)
// Wait, the error says: 'function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'
// That is 4 arguments. 
// Let's try to match the 4-argument signature for encode and 3-argument for decode (as decode usually is 3).
// Actually, looking at common tinygltf versions, URICallbacks is:
// struct URICallbacks {
//   URIEncodeFunction encode;
//   URIDecodeFunction decode;
//   void *user_data;
// };
// And the typedefs are usually:
// typedef bool (*URIEncodeFunction)(const std::string& in_uri, std::string* out_uri, void* user_data);
// typedef bool (*URIDecodeFunction)(const std::string& in_uri, std::string* out_uri, void* user_data);
// The error message explicitly says the target type is `function<bool (const string, const string, string*, void*)>`.
// This implies the `encode` member in the specific build expects 4 arguments.
// Let's define the dummy functions to match the compiler's expectation based on the error.
// The error says: conversion from `bool (*)(const std::string &, std::string *, void *)` to `function<bool (const string, const string, string*, void*)>`.
// So the target expects: (const string, const string, string*, void*).
// Let's define DummyURIEncode with 4 arguments.

static bool DummyURIEncode(const std::string &in_uri, const std::string & /*unused*/, std::string *out_uri, void *user_data) {
    if (!out_uri) return false;
    *out_uri = in_uri;
    return true;
}

static bool DummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (!out_uri) return false;
    *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Step 1: Prepare environment
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Write input data to a dummy file for loading
    const std::string filename = "./dummy_file";
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) return 0;
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();

    // 1. tinygltf::TinyGLTF::LoadASCIIFromFile
    ctx.LoadASCIIFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);

    // 2. tinygltf::TinyGLTF::LoadASCIIFromFile (again)
    ctx.LoadASCIIFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);

    // 3. tinygltf::TinyGLTF::SetURICallbacks
    tinygltf::URICallbacks callbacks;
    // Assigning function pointers to std::function members
    callbacks.encode = &DummyURIEncode;
    callbacks.decode = &DummyURIDecode;
    callbacks.user_data = nullptr;
    ctx.SetURICallbacks(callbacks, &err);

    // 4. tinygltf::TinyGLTF::WriteGltfSceneToFile
    // Write to a different dummy file to avoid overwriting input immediately
    const std::string output_filename = "./dummy_output_file";
    ctx.WriteGltfSceneToFile(&model, output_filename, true, true, true, false);

    // 5. tinygltf::TinyGLTF::LoadASCIIFromFile (third time)
    ctx.LoadASCIIFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);

    // 6. tinygltf::URIDecode
    std::string decoded_uri_1;
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    tinygltf::URIDecode(input_str, &decoded_uri_1, nullptr);

    // 7. tinygltf::URIDecode (again)
    std::string decoded_uri_2;
    // Use a substring or modified data for variety if possible, otherwise reuse
    if (input_str.length() > 1) {
        input_str = input_str.substr(1);
    }
    tinygltf::URIDecode(input_str, &decoded_uri_2, nullptr);

    // Step 2: Cleanup
    // No explicit cleanup needed for Model or TinyGLTF as they use RAII
    // Remove dummy files
    std::remove(filename.c_str());
    std::remove(output_filename.c_str());

    return 0;
}