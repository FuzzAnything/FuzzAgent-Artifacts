// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::LoadExternalFile at tiny_gltf.h:2485:13 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::ParseBuffer at tiny_gltf.h:4523:13 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::ParseImage at tiny_gltf.h:4292:13 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
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

// Define TINYGLTF_IMPLEMENTATION before including the header to get the implementation
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_TEXTUR
#include "tiny_gltf.h"

// Static helper to write fuzz data to a dummy file
static void WriteDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Static helper to create FsCallbacks with dummy implementations
static void SetupFsCallbacks(tinygltf::FsCallbacks *fs) {
    fs->FileExists = [](const std::string &filepath, void *user_data) -> bool {
        std::ifstream f(filepath);
        return f.good();
    };
    
    fs->ExpandFilePath = [](const std::string &filepath, void *user_data) -> std::string {
        return filepath;
    };
    
    fs->ReadWholeFile = [](std::vector<unsigned char> *out, std::string *err,
                           const std::string &filepath, void *user_data) -> bool {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            if (err) *err = "File not found";
            return false;
        }
        out->assign((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
        return true;
    };
    
    fs->WriteWholeFile = [](std::string *err, const std::string &filepath,
                            const std::vector<unsigned char> &contents, void *user_data) -> bool {
        std::ofstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            if (err) *err = "Cannot write file";
            return false;
        }
        file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
        return true;
    };
    
    fs->GetFileSizeInBytes = [](size_t *filesize_out, std::string *err,
                                const std::string &filepath, void *userdata) -> bool {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            if (err) *err = "File not found";
            return false;
        }
        *filesize_out = static_cast<size_t>(file.tellg());
        return true;
    };
    
    fs->user_data = nullptr;
}

// Static helper to create URICallbacks with dummy implementations
static void SetupURICallbacks(tinygltf::URICallbacks *uri_cb) {
    uri_cb->encode = [](const std::string &in_str, const std::string &base, std::string *out, void *user_data) -> bool {
        *out = in_str;
        return true;
    };
    
    uri_cb->decode = [](const std::string &in_str, std::string *out, void *user_data) -> bool {
        *out = in_str;
        return true;
    };
    uri_cb->user_data = nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize structures
    tinygltf::Image image;
    tinygltf::Buffer buffer;
    tinygltf::FsCallbacks fs;
    tinygltf::URICallbacks uri_cb;
    std::string err;
    std::string warn;
    
    SetupFsCallbacks(&fs);
    SetupURICallbacks(&uri_cb);

    // 1. Fuzz GetFileSizeInBytes via FsCallbacks
    WriteDummyFile(Data, Size);
    
    size_t filesize = 0;
    std::string err_size;
    bool size_result = fs.GetFileSizeInBytes(&filesize, &err_size, "./dummy_file", nullptr);
    (void)size_result;

    // 2. Fuzz TinyGLTF class methods
    tinygltf::TinyGLTF model;
    if (Size >= 4) {
        size_t max_size = 0;
        std::memcpy(&max_size, Data, 4);
        if (max_size == 0) max_size = 1;
        model.SetMaxExternalFileSize(max_size);
    }
    size_t current_max = model.GetMaxExternalFileSize();
    (void)current_max;

    // 3. Fuzz LoadExternalFile (in tinygltf namespace as per error correction)
    std::vector<unsigned char> out_buffer;
    std::string err_load;
    std::string warn_load;
    
    bool load_result = tinygltf::LoadExternalFile(
        &out_buffer, &err_load, &warn_load, "./dummy_file", 
        "", true, 0, false, 
        model.GetMaxExternalFileSize(), &fs
    );
    (void)load_result;

    // 4. Fuzz ParseBuffer (in tinygltf namespace as per error correction)
    nlohmann::json buffer_json;
    buffer_json["uri"] = "./dummy_file";
    buffer_json["byteLength"] = static_cast<int>(Size);
    
    bool parse_buffer_result = tinygltf::ParseBuffer(
        &buffer, &err, buffer_json, false, &fs, &uri_cb, 
        "", model.GetMaxExternalFileSize(), false, nullptr, 0
    );
    (void)parse_buffer_result;

    // 5. Fuzz ParseImage (in tinygltf namespace as per error correction)
    nlohmann::json image_json;
    if (Size > 0 && Data[0] % 2 == 0) {
        image_json["uri"] = "./dummy_file";
    } else {
        image_json["bufferView"] = 0;
    }
    
    bool parse_image_result = tinygltf::ParseImage(
        &image, 0, &err, &warn, image_json, false, 
        "", model.GetMaxExternalFileSize(), &fs, &uri_cb
    );
    (void)parse_image_result;

    return 0;
}