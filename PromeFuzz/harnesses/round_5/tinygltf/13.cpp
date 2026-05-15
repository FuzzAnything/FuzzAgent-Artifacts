// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

static bool dummyFileExists(const std::string&, void*) { return false; }
static std::string dummyExpandFilePath(const std::string&, void*) { return ""; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return false; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return false; }
static bool dummyGetFileSize(unsigned long*, std::string*, const std::string&, void*) { return false; }

static bool dummyURIEncode(const std::string&, const std::string&, std::string*, void*) { return false; }
static bool dummyURIDecode(const std::string&, std::string*, void*) { return false; }

static tinygltf::FsCallbacks makeDummyFsCallbacks() {
    tinygltf::FsCallbacks fs;
    fs.FileExists = dummyFileExists;
    fs.ExpandFilePath = dummyExpandFilePath;
    fs.ReadWholeFile = dummyReadWholeFile;
    fs.WriteWholeFile = dummyWriteWholeFile;
    fs.GetFileSizeInBytes = dummyGetFileSize;
    fs.user_data = nullptr;
    return fs;
}

static tinygltf::URICallbacks makeDummyURICallbacks() {
    tinygltf::URICallbacks uri;
    uri.encode = dummyURIEncode;
    uri.decode = dummyURIDecode;
    uri.user_data = nullptr;
    return uri;
}

static tinygltf::Image createTestImage(const uint8_t* data, size_t size) {
    tinygltf::Image image;
    image.width = size > 0 ? static_cast<int>(data[0] % 64 + 1) : 1;
    image.height = size > 1 ? static_cast<int>(data[1] % 64 + 1) : 1;
    
    size_t pixel_count = static_cast<size_t>(image.width) * static_cast<size_t>(image.height) * 4;
    image.image.resize(std::min(pixel_count, size));
    for (size_t i = 0; i < image.image.size() && i < size; ++i) {
        image.image[i] = data[i % size];
    }
    
    image.as_is = size > 2 ? (data[2] % 2 == 0) : false;
    return image;
}

static tinygltf::Model createTestModel(const uint8_t* data, size_t size) {
    tinygltf::Model model;
    
    model.asset.version = "2.0";
    model.asset.generator = "Fuzzer";
    
    if (size > 0) {
        tinygltf::Scene scene;
        scene.name = "TestScene";
        model.scenes.push_back(scene);
        model.defaultScene = 0;
        
        tinygltf::Node node;
        model.nodes.push_back(node);
        model.scenes[0].nodes.push_back(0);
        
        tinygltf::Buffer buffer;
        buffer.data.resize(std::min(size, static_cast<size_t>(1024)));
        for (size_t i = 0; i < buffer.data.size(); ++i) {
            buffer.data[i] = data[i % size];
        }
        model.buffers.push_back(buffer);
        
        tinygltf::Image image = createTestImage(data, size);
        model.images.push_back(image);
        
        tinygltf::Material material;
        material.name = "TestMaterial";
        model.materials.push_back(material);
        
        tinygltf::Mesh mesh;
        mesh.name = "TestMesh";
        model.meshes.push_back(mesh);
    }
    
    return model;
}

static std::string createTestString(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        char c = static_cast<char>(data[i]);
        if (c >= 32 && c <= 126) {
            result.push_back(c);
        } else {
            result.push_back('a' + (c % 26));
        }
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t* payload = Data + 1;
    size_t payload_size = Size - 1;
    
    try {
        switch (selector) {
            case 0: {
                tinygltf::Image image = createTestImage(payload, payload_size);
                std::string basepath = "./";
                std::string filename = "dummy_image.png";
                bool embedImages = payload_size > 0 ? (payload[0] % 2 == 0) : false;
                tinygltf::FsCallbacks fs = makeDummyFsCallbacks();
                tinygltf::URICallbacks uri = makeDummyURICallbacks();
                std::string out_uri;
                
                tinygltf::WriteImageData(&basepath, &filename, &image, embedImages, &fs, &uri, &out_uri, nullptr);
                break;
            }
            
            case 1: {
                tinygltf::Model model = createTestModel(payload, payload_size);
                tinygltf::TinyGLTF loader;
                
                bool embedImages = payload_size > 0 ? (payload[0] % 2 == 0) : false;
                bool embedBuffers = payload_size > 1 ? (payload[1] % 2 == 0) : false;
                bool prettyPrint = payload_size > 2 ? (payload[2] % 2 == 0) : false;
                bool writeBinary = payload_size > 3 ? (payload[3] % 2 == 0) : false;
                
                loader.WriteGltfSceneToFile(&model, "./dummy_file", embedImages, embedBuffers, prettyPrint, writeBinary);
                
                std::remove("./dummy_file");
                break;
            }
            
            case 2: {
                std::string mime_type;
                std::vector<unsigned char> out;
                
                std::string data_uri = createTestString(payload, std::min(payload_size, static_cast<size_t>(1024)));
                if (data_uri.empty()) data_uri = "data:text/plain;base64,SGVsbG8=";
                
                size_t reqBytes = payload_size > 0 ? static_cast<size_t>(payload[0]) * 10 : 0;
                bool checkSize = payload_size > 1 ? (payload[1] % 2 == 0) : false;
                
                tinygltf::DecodeDataURI(&out, mime_type, data_uri, reqBytes, checkSize);
                break;
            }
            
            case 3: {
                tinygltf::TinyGLTF loader;
                tinygltf::URICallbacks uri_callbacks = makeDummyURICallbacks();
                std::string err;
                
                loader.SetURICallbacks(uri_callbacks, &err);
                break;
            }
            
            case 4: {
                std::string test_string = createTestString(payload, std::min(payload_size, static_cast<size_t>(1024)));
                if (test_string.empty()) test_string = "data:text/plain;base64,SGVsbG8=";
                
                tinygltf::IsDataURI(test_string);
                break;
            }
            
            case 5: {
                std::string test_string = createTestString(payload, std::min(payload_size, static_cast<size_t>(1024)));
                if (test_string.empty()) test_string = "Hello%20World%21";
                
                std::string out_uri;
                tinygltf::URIDecode(test_string, &out_uri, nullptr);
                break;
            }
        }
    } catch (...) {
    }
    
    return 0;
}