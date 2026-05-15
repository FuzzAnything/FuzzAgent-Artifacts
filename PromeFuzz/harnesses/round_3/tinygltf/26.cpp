// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
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
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "tiny_gltf.h"

static bool dummyFileExists(const std::string& abs_filename, void* user_data) {
    // Always return true for fuzzing
    return true;
}

static std::string dummyExpandFilePath(const std::string& filepath, void* user_data) {
    // Return input unchanged as per documentation
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                              std::string* err,
                              const std::string& filepath,
                              void* user_data) {
    // Return empty data for fuzzing
    out->clear();
    return true;
}

static bool dummyWriteWholeFile(std::string* err,
                               const std::string& filepath,
                               const std::vector<unsigned char>& contents,
                               void* user_data) {
    // Always succeed for fuzzing
    return true;
}

static bool dummyGetFileSizeInBytes(size_t* filesize_out,
                                   std::string* err,
                                   const std::string& filepath,
                                   void* user_data) {
    // Return 0 size for fuzzing
    *filesize_out = 0;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to decide which function to test
    uint8_t selector = Data[0];
    const uint8_t* payload = Data + 1;
    size_t payload_size = Size - 1;

    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;

    try {
        switch (selector % 6) {
            case 0: {
                // Test SetFsCallbacks
                tinygltf::FsCallbacks callbacks;
                callbacks.FileExists = dummyFileExists;
                callbacks.ExpandFilePath = dummyExpandFilePath;
                callbacks.ReadWholeFile = dummyReadWholeFile;
                callbacks.WriteWholeFile = dummyWriteWholeFile;
                callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
                callbacks.user_data = nullptr;
                
                std::string err;
                bool result = loader.SetFsCallbacks(callbacks, &err);
                (void)result; // Ignore result
                break;
            }
            
            case 1: {
                // Test WriteGltfSceneToFile
                tinygltf::Model model;
                
                // Initialize some basic model data
                model.asset.version = "2.0";
                
                // Create a simple scene
                tinygltf::Scene scene;
                scene.name = "TestScene";
                model.scenes.push_back(scene);
                model.defaultScene = 0;
                
                // Create a node
                tinygltf::Node node;
                node.name = "TestNode";
                model.nodes.push_back(node);
                
                // Create a simple buffer
                tinygltf::Buffer buffer;
                buffer.name = "TestBuffer";
                if (payload_size > 0) {
                    buffer.data.assign(payload, payload + std::min(payload_size, (size_t)100));
                }
                model.buffers.push_back(buffer);
                
                // Try different combinations of parameters
                bool embedImages = (payload_size % 2 == 0);
                bool embedBuffers = (payload_size % 3 == 0);
                bool prettyPrint = (payload_size % 5 == 0);
                bool writeBinary = (payload_size % 7 == 0);
                
                bool result = loader.WriteGltfSceneToFile(&model, "./dummy_file", 
                                                         embedImages, embedBuffers,
                                                         prettyPrint, writeBinary);
                (void)result; // Ignore result
                break;
            }
            
            case 2: {
                // Test ExpandFilePath
                std::string filepath;
                if (payload_size > 0) {
                    filepath.assign(reinterpret_cast<const char*>(payload), 
                                   std::min(payload_size, (size_t)100));
                } else {
                    filepath = "./test_path";
                }
                
                std::string result = tinygltf::ExpandFilePath(filepath, nullptr);
                (void)result; // Ignore result
                break;
            }
            
            case 3: {
                // Test GetFileSizeInBytes
                std::string filepath;
                if (payload_size > 0) {
                    filepath.assign(reinterpret_cast<const char*>(payload), 
                                   std::min(payload_size, (size_t)100));
                } else {
                    filepath = "./dummy_file";
                }
                
                size_t filesize = 0;
                std::string err;
                bool result = tinygltf::GetFileSizeInBytes(&filesize, &err, filepath, nullptr);
                (void)result; // Ignore result
                break;
            }
            
            case 4: {
                // Test FileExists
                std::string abs_filename;
                if (payload_size > 0) {
                    abs_filename.assign(reinterpret_cast<const char*>(payload), 
                                       std::min(payload_size, (size_t)100));
                } else {
                    abs_filename = "./dummy_file";
                }
                
                bool result = tinygltf::FileExists(abs_filename, nullptr);
                (void)result; // Ignore result
                break;
            }
            
            case 5: {
                // Test WriteWholeFile
                std::string filepath;
                if (payload_size > 0) {
                    // Use first half for filepath, second half for contents
                    size_t split = payload_size / 2;
                    filepath.assign(reinterpret_cast<const char*>(payload), 
                                   std::min(split, (size_t)50));
                    
                    std::vector<unsigned char> contents;
                    if (payload_size > split) {
                        contents.assign(payload + split, payload + payload_size);
                    }
                    
                    std::string err;
                    bool result = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
                    (void)result; // Ignore result
                } else {
                    // Test with empty contents
                    std::string err;
                    bool result = tinygltf::WriteWholeFile(&err, "./dummy_file", 
                                                          std::vector<unsigned char>(), nullptr);
                    (void)result; // Ignore result
                }
                break;
            }
        }
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }

    return 0;
}