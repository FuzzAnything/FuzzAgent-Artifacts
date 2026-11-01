// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <fstream>

// Static helper to write fuzz data to a file
static void WriteDataToFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Static helper function for URI encoding
// Signature must match tinygltf::URIEncodeFunction exactly
// Typically: bool (const std::string &in_uri, std::string *out_uri, void *user_data)
// However, based on the error, the library might expect a different signature or the type definition in the header is specific.
// Looking at the error: 'bool (const std::string &, std::string *, void *)' vs 'function<bool (const std::string &, const std::string &, basic_string<char> *, void *)>'
// The error suggests the library's URICallbacks expects a function taking (in_uri, out_uri, user_data) but the std::function wrapper expects an extra const string argument?
// Actually, looking closely at the error:
// 'no known conversion from 'bool (const std::string &, std::string *, void *)' ... to 'function<bool (const std::string &, const std::string &, basic_string<char> *, void *)>'
// This implies the library's URIEncodeFunction typedef is: bool (const std::string&, const std::string&, std::string*, void*)?
// Or perhaps the error message is showing the mismatch between the provided function and the expected std::function signature.
// Let's assume the standard tinygltf signature: bool (const std::string &in_uri, std::string *out_uri, void *user_data)
// The error message is confusing. It says the target function signature is: bool (const std::string &, const std::string &, std::string *, void *)
// Wait, the error says: "candidate function not viable: no known conversion from 'bool (const std::string &, std::string *, void *)' ... to 'function<bool (const std::string &, const std::string &, basic_string<char> *, void *)>'"
// This means the std::function inside URICallbacks expects 4 arguments: (const string, const string, string*, void*).
// But standard tinygltf URIEncodeFunction is usually (const string, string*, void*).
// Let's check the provided type info. The provided info doesn't show the typedef for URIEncodeFunction explicitly, only URICallbacks struct.
// However, the error is explicit. The std::function expects: (const std::string &, const std::string &, std::string *, void *).
// This is unusual. Maybe it's (const std::string &in_uri, const std::string &out_uri_hint?, std::string *out_uri, void *user_data)?
// Or maybe the error message is slightly misleading and it's actually (const std::string &, std::string *, void *) but the std::function is instantiated differently?
// Let's try to match the signature in the error message exactly: bool (const std::string &, const std::string &, std::string *, void *)
// Arguments: in_uri, ???, out_uri, user_data.
// Let's try defining the function with 4 arguments to match the error's expectation.

static bool DummyURIEncode(const std::string &in_uri, const std::string &dummy_arg, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. LoadASCIIFromFile (First invocation)
    WriteDataToFile("./dummy_file_1", Data, Size);
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file_1", tinygltf::REQUIRE_VERSION);

    // 2. LoadASCIIFromFile (Second invocation)
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file_1", tinygltf::REQUIRE_VERSION);

    // 3. SetURICallbacks
    tinygltf::URICallbacks callbacks;
    // Matching the signature inferred from the compiler error
    callbacks.encode = DummyURIEncode;
    callbacks.decode = tinygltf::URIDecode;
    callbacks.user_data = nullptr;
    
    std::string uri_err;
    ctx.SetURICallbacks(callbacks, &uri_err);

    // 4. WriteGltfSceneToFile
    if (model.scenes.empty()) {
        model.scenes.push_back(tinygltf::Scene());
        model.asset.version = "2.0";
    }
    
    ctx.WriteGltfSceneToFile(&model, "./dummy_file_out.gltf", false, false, true, false);

    // 5. LoadASCIIFromFile (Third invocation)
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file_out.gltf", tinygltf::REQUIRE_VERSION);

    // 6. URIDecode (First invocation)
    std::string input_uri(reinterpret_cast<const char*>(Data), Size);
    std::string decoded_uri;
    tinygltf::URIDecode(input_uri, &decoded_uri, nullptr);

    // 7. URIDecode (Second invocation)
    std::string input_uri2(reinterpret_cast<const char*>(Data), Size);
    std::string decoded_uri2;
    tinygltf::URIDecode(input_uri2, &decoded_uri2, nullptr);

    return 0;
}