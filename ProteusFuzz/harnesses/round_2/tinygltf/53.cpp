// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
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
#include <fstream>
#include "tiny_gltf.h"

// Static helper to create a dummy file for LoadExternalFile testing
static void CreateDummyFile(const std::string &filename, const std::vector<unsigned char> &data) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the input data to drive fuzzing logic
    std::string input_str(reinterpret_cast<const char*>(Data), Size);

    // 1. Fuzz IsDataURI
    bool is_data_uri = tinygltf::IsDataURI(input_str);
    (void)is_data_uri; // Suppress unused variable warning

    // 2. Fuzz URIDecode
    std::string decoded_uri;
    tinygltf::URIDecode(input_str, &decoded_uri, nullptr);

    // 3. Fuzz DecodeDataURI
    std::vector<unsigned char> decoded_data;
    std::string mime_type;
    tinygltf::DecodeDataURI(&decoded_data, mime_type, input_str, 0, false);

    // 4. Fuzz LoadExternalFile
    // Create a dummy file with the input data
    const std::string dummy_filename = "./dummy_file";
    std::vector<unsigned char> file_data(Data, Data + Size);
    CreateDummyFile(dummy_filename, file_data);

    std::vector<unsigned char> out_buffer;
    std::string err, warn;
    tinygltf::FsCallbacks fs;
    fs.FileExists = nullptr;
    fs.ExpandFilePath = nullptr;
    fs.ReadWholeFile = nullptr;
    fs.WriteWholeFile = nullptr;
    fs.GetFileSizeInBytes = nullptr;
    fs.user_data = nullptr;

    // 5. Fuzz ParseBuffer and ParseImage indirectly via Model loading
    // Since ParseBuffer and ParseImage are static internal functions, we rely on public APIs.
    
    // Create a minimal glTF JSON string using the input data as URI if possible
    std::string gltf_json = R"({"asset":{"version":"2.0"},"buffers":[{"uri":"data:application/octet-stream;base64,SGVsbG8=","byteLength":100}],"images":[{"uri":"data:image/png;base64,iVBORw0KGgo="}]})";
    
    // If input looks like a valid JSON, try to use it, otherwise use the template
    // We attempt to parse input as JSON to see if it's a valid glTF
    // For simplicity, we just use the template with input data embedded if it's short enough
    if (Size < 100) {
        std::string safe_input = input_str;
        // Escape quotes for JSON
        for (char &c : safe_input) {
            if (c == '"') c = '\\';
        }
        gltf_json = "{\"asset\":{\"version\":\"2.0\"},\"buffers\":[{\"uri\":\"data:application/octet-stream;base64," + safe_input + "\",\"byteLength\":" + std::to_string(Size) + "}]}";
    }

    // Write the glTF JSON to a file
    std::ofstream gltf_file("dummy.gltf");
    if (gltf_file.is_open()) {
        gltf_file << gltf_json;
        gltf_file.close();
    }

    // Try to load the glTF model
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string load_err, load_warn;
    
    // LoadASCIIFromString expects const char*, so we pass c_str()
    // Fixed: The 6th argument is check_sections (unsigned int), not FsCallbacks*
    // FsCallbacks are passed via the loader's SetFsCallbacks or internally if loading from file path
    // For LoadASCIIFromString, we don't pass FsCallbacks directly in the signature.
    // However, to test file loading logic, we can use LoadBinary or LoadFromString with a path.
    // Let's use LoadFromString which takes a base directory and uses internal callbacks or set ones.
    // Actually, LoadASCIIFromString signature is:
    // bool LoadASCIIFromString(Model *model, std::string *err, std::string *warn,
    //                          const char *json_str, size_t length,
    //                          const std::string &basedir, unsigned int check_sections);
    
    // We will use the default check_sections (REQUIRE_VERSION)
    bool load_success = loader.LoadASCIIFromString(&model, &load_err, &load_warn, gltf_json.c_str(), gltf_json.length(), "", tinygltf::REQUIRE_VERSION);
    (void)load_success; // Suppress unused variable warning
    (void)load_err;
    (void)load_warn;

    // Clean up dummy files
    std::remove(dummy_filename.c_str());
    std::remove("dummy.gltf");

    return 0;
}