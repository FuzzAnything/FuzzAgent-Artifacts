// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
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
#include <iomanip>

static void WriteDataToFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Match the signature expected by tinygltf::URIEncodeFunction
// Usually: bool (const std::string &in_uri, std::string *out_uri, void *user_data)
// However, the error suggests the library expects an extra const std::string& argument for the output or input.
// Based on the error message: 'function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'
// It seems the encode function signature in this version of tinygltf might be different or the typedef is specific.
// Let's try to match the standard tinygltf signature first, but if the error implies a specific signature, we adapt.
// The error says: conversion from 'bool (const std::string &, std::string *, void *)' to 'function<bool (const std::string &, const std::string &, std::string *, void *)>'
// This implies the library expects: bool(const std::string&, const std::string&, std::string*, void*)
// Wait, the error message shows the target type has 4 arguments: const string, const string, string*, void*.
// Let's look at the error again: 'no known conversion from ... to ... function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'
// This is strange for URIEncode. Usually it's (in, out, user_data).
// Let's re-read the error carefully.
// The target type is: function<bool (const std::string &, const std::string &, std::string *, void *)>
// This looks like a typo in my assumption or a specific version of tinygltf.
// However, standard tinygltf URICallbacks usually define:
// typedef bool (*URIEncodeFunction)(const std::string &in_uri, std::string *out_uri, void *user_data);
// typedef bool (*URIDecodeFunction)(const std::string &in_uri, std::string *out_uri, void *user_data);
// The error message suggests the library's typedef for encode is different.
// Let's try to define the function with the signature that matches the error's expectation if possible, 
// OR simply cast if the types are compatible but the compiler is strict about std::function assignment.
// Actually, the error says the target function has 4 arguments. 
// Let's assume the user's tinygltf version has a specific signature.
// If the error is: function<bool (const std::string &, const std::string &, std::string *, void *)>
// Then the function should be: bool(const std::string &, const std::string &, std::string *, void *)
// But standard tinygltf doesn't have that. 
// Let's look at the error again. 
// "candidate function not viable: no known conversion from 'bool (const std::string &, std::string *, void *)' ... to 'function<bool (const std::string &, const std::string &, std::string *, void *)>'"
// This implies the library expects 4 arguments.
// Maybe the first two are input and output? No, output is a pointer.
// Let's try to define the dummy function with the signature that the compiler complains is missing, 
// which is: bool(const std::string &, const std::string &, std::string *, void *)
// But wait, the error says the target is `function<bool (const std::string &, const std::string &, std::string *, void *)>`.
// This is very specific. Let's define the dummy function to match this exact signature to satisfy the compiler.
// However, if the library actually calls it with 3 arguments, this will crash.
// But the goal is to compile the fuzz driver.
// Let's try to define the function with the signature that matches the error's target type.

static bool DummyURIEncode(const std::string &in_uri, const std::string &unused, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

static bool DummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    std::string dummy_file = "./dummy_file";

    // Split input data to create diverse test scenarios
    size_t split1 = (Size / 3);
    size_t split2 = (Size / 3) * 2;

    // 1. LoadASCIIFromFile (First call)
    WriteDataToFile(dummy_file, Data, split1);
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);

    // 2. LoadASCIIFromFile (Second call with different data)
    WriteDataToFile(dummy_file, Data + split1, split2 - split1);
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);

    // 3. SetURICallbacks
    tinygltf::URICallbacks callbacks;
    // Assigning the function pointer to the std::function member
    // The signature must match exactly what the library expects.
    // Based on the error, the encode function expects 4 arguments.
    callbacks.encode = DummyURIEncode;
    callbacks.decode = DummyURIDecode;
    callbacks.user_data = nullptr;
    ctx.SetURICallbacks(callbacks, &err);

    // 4. WriteGltfSceneToFile
    ctx.WriteGltfSceneToFile(&model, dummy_file, true, true, true, false);

    // 5. LoadASCIIFromFile (Third call)
    WriteDataToFile(dummy_file, Data + split2, Size - split2);
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);

    // 6. URIDecode (First call)
    std::string out_uri;
    std::string input_uri(reinterpret_cast<const char*>(Data), Size);
    tinygltf::URIDecode(input_uri, &out_uri, nullptr);

    // 7. URIDecode (Second call with modified input)
    std::string input_uri2(reinterpret_cast<const char*>(Data + 1), Size > 1 ? Size - 1 : 0);
    tinygltf::URIDecode(input_uri2, &out_uri, nullptr);

    return 0;
}