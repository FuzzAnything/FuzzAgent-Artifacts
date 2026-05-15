// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

#include "tiny_gltf.h"

// Dummy filesystem callbacks for testing with correct signatures
static bool dummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& filename, void* user_data) {
    return filename;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filename,
                               void* user_data) {
    out->push_back('t');
    out->push_back('e');
    out->push_back('s');
    out->push_back('t');
    return true;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    return true;
}

static bool dummyGetFileSize(unsigned long* out,
                             std::string* err,
                             const std::string& filename,
                             void* user_data) {
    *out = 4;
    return true;
}

// Dummy URI encode callback with correct signature
static bool dummyURIEncode(const std::string& in,
                           const std::string& basepath,
                           std::string* out,
                           void* user_data) {
    *out = in;
    return true;
}

// Dummy URI decode callback with correct signature
static bool dummyURIDecode(const std::string& in,
                           std::string* out,
                           void* user_data) {
    *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        // Use first byte to determine which function to test
        uint8_t selector = Data[0] % 6;
        const uint8_t* fuzz_data = Data + 1;
        size_t fuzz_size = Size - 1;

        switch (selector) {
            case 0: { // Test URIDecode
                std::string in_uri(reinterpret_cast<const char*>(fuzz_data), 
                                  std::min(fuzz_size, (size_t)1000));
                std::string out_uri;
                bool result = tinygltf::URIDecode(in_uri, &out_uri, nullptr);
                (void)result; // Ignore result
                break;
            }

            case 1: { // Test DecodeDataURI
                std::string data_uri(reinterpret_cast<const char*>(fuzz_data), 
                                    std::min(fuzz_size, (size_t)1000));
                std::vector<unsigned char> out;
                std::string mime_type;
                
                // Use random values for reqBytes and checkSize from fuzz data
                size_t reqBytes = 0;
                bool checkSize = false;
                if (fuzz_size >= 1) {
                    reqBytes = static_cast<size_t>(fuzz_data[0]) * 10;
                    checkSize = (fuzz_data[0] % 2) == 0;
                }
                
                bool result = tinygltf::DecodeDataURI(&out, mime_type, data_uri, 
                                                     reqBytes, checkSize);
                (void)result; // Ignore result
                break;
            }

            case 2: { // Test WriteImageData
                tinygltf::Image image;
                image.name = "test_image";
                image.width = 16;
                image.height = 16;
                image.as_is = (fuzz_size > 0 && (fuzz_data[0] % 2) == 0);
                
                // Fill image data with fuzz data (limited size)
                size_t data_size = std::min(fuzz_size, (size_t)256);
                image.image.assign(fuzz_data, fuzz_data + data_size);
                
                std::string basepath = "./";
                std::string filename = "test.png";
                bool embedImages = (fuzz_size > 1 && (fuzz_data[1] % 2) == 0);
                std::string out_uri;
                
                // Use default callbacks
                tinygltf::FsCallbacks fs_callbacks = {
                    dummyFileExists,
                    dummyExpandFilePath,
                    dummyReadWholeFile,
                    dummyWriteWholeFile,
                    dummyGetFileSize,
                    nullptr
                };
                
                tinygltf::URICallbacks uri_callbacks = {
                    dummyURIEncode,
                    dummyURIDecode,
                    nullptr
                };
                
                bool result = tinygltf::WriteImageData(&basepath, &filename, &image,
                                                      embedImages, &fs_callbacks,
                                                      &uri_callbacks, &out_uri, nullptr);
                (void)result; // Ignore result
                break;
            }

            case 3: { // Test WriteGltfSceneToFile
                tinygltf::TinyGLTF loader;
                tinygltf::Model model;
                
                // Initialize model with some basic data
                model.asset.version = "2.0";
                model.asset.generator = "Fuzzer";
                
                // Create a simple scene
                tinygltf::Scene scene;
                scene.name = "TestScene";
                model.scenes.push_back(scene);
                model.defaultScene = 0;
                
                // Create a simple node
                tinygltf::Node node;
                node.name = "TestNode";
                model.nodes.push_back(node);
                
                // Add node to scene
                model.scenes[0].nodes.push_back(0);
                
                // Create a simple buffer with fuzz data
                tinygltf::Buffer buffer;
                buffer.name = "TestBuffer";
                size_t buffer_size = std::min(fuzz_size, (size_t)1000);
                buffer.data.assign(fuzz_data, fuzz_data + buffer_size);
                model.buffers.push_back(buffer);
                
                // Set parameters based on fuzz data
                bool embedImages = (fuzz_size > 0 && (fuzz_data[0] % 2) == 0);
                bool embedBuffers = (fuzz_size > 1 && (fuzz_data[1] % 2) == 0);
                bool prettyPrint = (fuzz_size > 2 && (fuzz_data[2] % 2) == 0);
                bool writeBinary = (fuzz_size > 3 && (fuzz_data[3] % 2) == 0);
                
                // Write to dummy file
                std::string filename = "./dummy_file";
                bool result = loader.WriteGltfSceneToFile(&model, filename,
                                                         embedImages, embedBuffers,
                                                         prettyPrint, writeBinary);
                (void)result; // Ignore result
                
                // Clean up the dummy file
                std::remove(filename.c_str());
                break;
            }

            case 4: { // Test SetFsCallbacks
                tinygltf::TinyGLTF loader;
                
                tinygltf::FsCallbacks callbacks = {
                    dummyFileExists,
                    dummyExpandFilePath,
                    dummyReadWholeFile,
                    dummyWriteWholeFile,
                    dummyGetFileSize,
                    nullptr
                };
                
                std::string err;
                bool result = loader.SetFsCallbacks(callbacks, &err);
                (void)result; // Ignore result
                break;
            }

            case 5: { // Test SetURICallbacks
                tinygltf::TinyGLTF loader;
                
                tinygltf::URICallbacks callbacks = {
                    dummyURIEncode,
                    dummyURIDecode,
                    nullptr
                };
                
                std::string err;
                bool result = loader.SetURICallbacks(callbacks, &err);
                (void)result; // Ignore result
                break;
            }
        }
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }

    return 0;
}