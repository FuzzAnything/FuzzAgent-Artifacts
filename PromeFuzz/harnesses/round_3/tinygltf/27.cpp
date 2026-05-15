// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
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
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

// Dummy filesystem callbacks for testing
static bool dummyFileExists(const std::string& filename, void* user_data) {
    return filename == "./dummy_file";
}

static std::string dummyExpandFilePath(const std::string& path, void* user_data) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                               const std::string& filename, void* user_data) {
    if (filename != "./dummy_file") {
        if (err) *err = "File not found";
        return false;
    }
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        if (err) *err = "Failed to open file";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    return true;
}

static bool dummyWriteWholeFile(std::string* err, const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    // Not used in loading, but required for callback struct
    if (err) *err = "Write not implemented";
    return false;
}

static bool dummyGetFileSizeInBytes(unsigned long* filesize, std::string* err,
                                    const std::string& filename, void* user_data) {
    if (filename != "./dummy_file") {
        if (err) *err = "File not found";
        return false;
    }
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        if (err) *err = "Failed to open file";
        return false;
    }
    *filesize = file.tellg();
    return true;
}

// Dummy URI callbacks for testing - corrected signatures based on compiler error
static bool dummyURIEncode(const std::string& in, const std::string& base, std::string* out, void* user_data) {
    if (out) *out = in;
    return true;
}

static bool dummyURIDecode(const std::string& in, std::string* out, void* user_data) {
    if (out) *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for file-based functions
    std::ofstream outfile("./dummy_file", std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
    }

    // Test IsDataURI with fuzzer input as string
    if (Size > 0) {
        std::string data_str(reinterpret_cast<const char*>(Data), Size);
        bool is_data_uri = tinygltf::IsDataURI(data_str);
        (void)is_data_uri; // Use result to avoid unused variable warning
    }

    // Test global FileExists function
    bool file_exists = tinygltf::FileExists("./dummy_file", nullptr);
    (void)file_exists;

    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;

    // Test SetFsCallbacks with dummy callbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = dummyFileExists;
    fs_callbacks.ExpandFilePath = dummyExpandFilePath;
    fs_callbacks.ReadWholeFile = dummyReadWholeFile;
    fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    std::string fs_err;
    bool fs_set = loader.SetFsCallbacks(fs_callbacks, &fs_err);
    (void)fs_set;

    // Test SetURICallbacks with dummy callbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = dummyURIEncode;
    uri_callbacks.decode = dummyURIDecode;
    uri_callbacks.user_data = nullptr;

    std::string uri_err;
    bool uri_set = loader.SetURICallbacks(uri_callbacks, &uri_err);
    (void)uri_set;

    // Test LoadASCIIFromFile
    tinygltf::Model model1;
    std::string err1, warn1;
    loader.LoadASCIIFromFile(&model1, &err1, &warn1, "./dummy_file");

    // Test LoadASCIIFromString
    tinygltf::Model model2;
    std::string err2, warn2;
    if (Size > 0) {
        loader.LoadASCIIFromString(&model2, &err2, &warn2,
                                  reinterpret_cast<const char*>(Data), Size,
                                  "./");
    } else {
        // Handle empty input case
        const char* empty_str = "";
        loader.LoadASCIIFromString(&model2, &err2, &warn2,
                                  empty_str, 0,
                                  "./");
    }

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}