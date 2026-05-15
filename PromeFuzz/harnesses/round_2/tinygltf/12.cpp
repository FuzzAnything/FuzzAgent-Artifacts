// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static void removeDummyFile() {
    std::remove("./dummy_file");
}

static bool dummyFileExists(const std::string &filename, void *user_data) {
    return filename == "./dummy_file";
}

static std::string dummyExpandPath(const std::string &path, void *user_data) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *user_data) {
    if (filename == "./dummy_file") {
        std::ifstream file(filename, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            out->resize(size);
            file.read(reinterpret_cast<char*>(out->data()), size);
            return true;
        }
    }
    if (err) *err = "File not found";
    return false;
}

static bool dummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &data,
                                void *user_data) {
    if (err) *err = "Write not supported";
    return false;
}

static bool dummyGetFileSize(size_t *size, std::string *err,
                             const std::string &filename, void *user_data) {
    if (filename == "./dummy_file") {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (file) {
            *size = file.tellg();
            return true;
        }
    }
    if (err) *err = "File not found";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for file-based operations
    writeDummyFile(Data, Size);
    
    // Test ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
    }
    
    // Test DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string input(reinterpret_cast<const char*>(Data), Size);
        
        // Test with various parameters
        tinygltf::DecodeDataURI(&out, mime_type, input, 0, false);
        tinygltf::DecodeDataURI(&out, mime_type, input, Size, true);
        tinygltf::DecodeDataURI(&out, mime_type, input, Size/2, true);
        tinygltf::DecodeDataURI(&out, mime_type, "data:application/octet-stream;base64," + input, 0, false);
    }
    
    // Test GetFileSizeInBytes
    {
        size_t filesize;
        std::string err;
        tinygltf::GetFileSizeInBytes(&filesize, &err, "./dummy_file", nullptr);
    }
    
    // Test TinyGLTF member functions
    {
        tinygltf::TinyGLTF loader;
        
        // Test SetMaxExternalFileSize
        loader.SetMaxExternalFileSize(Size);
        loader.SetMaxExternalFileSize(Size * 2);
        
        // Test SetFsCallbacks
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandPath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSize;
        callbacks.user_data = nullptr;
        
        std::string err;
        loader.SetFsCallbacks(callbacks, &err);
        
        // Test LoadASCIIFromFile with various parameters
        tinygltf::Model model;
        std::string warn;
        
        // Try with different check_sections values
        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 
                                tinygltf::REQUIRE_VERSION);
        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 
                                tinygltf::REQUIRE_ALL);
        
        // Test with null error/warning pointers
        loader.LoadASCIIFromFile(&model, nullptr, nullptr, "./dummy_file");
    }
    
    // Test related data type constructors
    {
        tinygltf::Model model;
        tinygltf::Asset asset;
        tinygltf::AudioSource audioSource;
        tinygltf::AudioEmitter audioEmitter;
        tinygltf::Light light;
        tinygltf::Scene scene;
        tinygltf::Camera camera;
        tinygltf::Sampler sampler;
        tinygltf::Skin skin;
        tinygltf::Image image;
        tinygltf::Texture texture;
        tinygltf::Mesh mesh;
        tinygltf::Material material;
        tinygltf::BufferView bufferView;
        tinygltf::Buffer buffer;
        tinygltf::Animation animation;
        tinygltf::Accessor accessor;
        
        // Test Value constructor with binary data
        std::vector<unsigned char> binaryData(Data, Data + (Size > 100 ? 100 : Size));
        tinygltf::Value value(std::move(binaryData));
    }
    
    // Clean up
    removeDummyFile();
    
    return 0;
}