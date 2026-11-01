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
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

// Define the correct function pointer types matching tinygltf expectations
// Based on the error, the encode function likely expects two const string references for input and output
// or the struct definition in the specific version of tinygltf.h expects a specific signature.
// Looking at the error: 'bool (const std::string &, const std::string &, std::string *, void *)'
// Wait, the error message says: 'no known conversion from 'bool (const std::string &, std::string *, void *)' ... to 'function<bool (const std::string &, const std::string &, std::string *, void *)>'
// This implies the expected signature for encode is: bool(const std::string& in, const std::string& out, std::string* result, void* user_data) ?? 
// That seems odd for an encode function. Usually it's bool(const std::string& in, std::string* out, void* user_data).
// Let's re-read the error carefully.
// Error: 'bool (const std::string &, std::string *, void *)' (my function)
// Expected: 'function<bool (const std::string &, const std::string &, std::string *, void *)>'
// This suggests the URICallbacks struct in this specific build of tinygltf expects a different signature for 'encode'.
// However, standard tinygltf URICallbacks usually are:
// typedef bool (*URIEncodeFunction)(const std::string& in_uri, std::string* out_uri, void* user_data);
// typedef bool (*URIDecodeFunction)(const std::string& in_uri, std::string* out_uri, void* user_data);
//
// The error message is extremely specific: 'function<bool (const std::string &, const std::string &, std::string *, void *)>'
// This implies the struct member 'encode' is a std::function expecting 4 arguments: (const string&, const string&, string*, void*).
// This is highly unusual. Let's look at the provided context again.
// The prompt says: "URIDecode decodes URI-encoded strings... Call it with an input URI and output pointer; user_data is unused."
// Signature: bool URIDecode(const std::string &in_uri, std::string *out_uri, void *user_data);
//
// But the error says the target type for 'encode' is: bool (const std::string &, const std::string &, std::string *, void *)
// This looks like a mismatch in the version of tinygltf.h being used or a specific modification.
// Let's assume the error message is the ground truth for the required signature in this environment.
// Required signature for encode: bool(const std::string& in, const std::string& ???, std::string* out, void* user_data)
// Maybe the first two are input and output? Or maybe it's (const string& in, const string& out_hint, string* out, void* data)?
// Actually, looking at common tinygltf issues, sometimes the signature is:
// bool (*URIEncodeFunction)(const std::string& in_uri, std::string* out_uri, void* user_data);
// But the error explicitly says the target expects: (const std::string &, const std::string &, std::string *, void *)
// This is 4 arguments.
// Let's try to match the signature exactly as the compiler demands.
// The compiler says: 'function<bool (const std::string &, const std::string &, std::string *, void *)>'
// So I must define my function with 4 parameters: const string&, const string&, string*, void*.
// Perhaps the second const string& is a base path or something?
// Let's define the function to match this signature exactly.

static bool CustomURIEncode(const std::string &in_uri, const std::string &unused_hint, std::string *out_uri, void *user_data) {
    (void)user_data;
    (void)unused_hint;
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool CustomURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    (void)user_data;
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file to simulate file-based operations
    const std::string dummy_file = "./dummy_file";
    std::ofstream ofs(dummy_file, std::ios::binary);
    if (!ofs) return 0;
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. LoadASCIIFromFile (First call)
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

    // 2. LoadASCIIFromFile (Second call)
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

    // 3. SetURICallbacks
    tinygltf::URICallbacks callbacks;
    // Assigning the function pointer to the std::function member
    // The signature must match exactly what the compiler expects based on the error
    callbacks.encode = CustomURIEncode;
    callbacks.decode = CustomURIDecode;
    callbacks.user_data = nullptr;
    ctx.SetURICallbacks(callbacks, &err);

    // 4. WriteGltfSceneToFile
    ctx.WriteGltfSceneToFile(&model, dummy_file, false, false, true, false);

    // 5. LoadASCIIFromFile (Third call)
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

    // 6. URIDecode (First call)
    std::string out_uri;
    // Use input data as URI string, ensuring it's null-terminated or safe
    std::string input_uri(reinterpret_cast<const char*>(Data), Size);
    tinygltf::URIDecode(input_uri, &out_uri, nullptr);

    // 7. URIDecode (Second call)
    // Try with a modified string or empty string to explore states
    std::string out_uri2;
    std::string input_uri2 = input_uri;
    if (!input_uri2.empty()) {
        input_uri2 += "%20";
    }
    tinygltf::URIDecode(input_uri2, &out_uri2, nullptr);

    return 0;
}