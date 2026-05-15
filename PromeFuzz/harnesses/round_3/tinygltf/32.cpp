// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
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
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

// Dummy filesystem callbacks for fuzzing
static bool dummyFileExists(const std::string& abs_filename, void* user_data) {
    // Always return true for the dummy file, false otherwise
    return abs_filename == "./dummy_file";
}

static std::string dummyExpandFilePath(const std::string& filepath, void* user_data) {
    // Return as-is as per documentation
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filepath,
                               void* user_data) {
    // Read from dummy file if requested
    if (filepath == "./dummy_file") {
        std::ifstream file(filepath, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            out->resize(size);
            file.read(reinterpret_cast<char*>(out->data()), size);
            return true;
        }
    }
    return false;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filepath,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    // Check if contents is empty as per documentation
    if (contents.empty()) {
        return true;
    }
    
    // Only write to dummy file
    if (filepath == "./dummy_file") {
        std::ofstream file(filepath, std::ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
            return file.good();
        }
    }
    return false;
}

static bool dummyGetFileSizeInBytes(size_t* filesize_out,
                                    std::string* err,
                                    const std::string& filepath,
                                    void* user_data) {
    // Return size for dummy file
    if (filepath == "./dummy_file") {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (file) {
            *filesize_out = file.tellg();
            return true;
        }
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    std::ofstream dummy_file("./dummy_file", std::ios::binary);
    if (dummy_file) {
        dummy_file.write(reinterpret_cast<const char*>(Data), Size);
    }
    dummy_file.close();
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Test SetFsCallbacks
    {
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandFilePath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        std::string err;
        bool result = loader.SetFsCallbacks(callbacks, &err);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test WriteGltfSceneToFile with various parameters
    {
        tinygltf::Model model;
        
        // Initialize basic model structure
        model.asset.version = "2.0";
        
        // Add some minimal data to model
        tinygltf::Scene scene;
        scene.name = "TestScene";
        model.scenes.push_back(scene);
        model.defaultScene = 0;
        
        // Add a node
        tinygltf::Node node;
        node.name = "TestNode";
        model.nodes.push_back(node);
        
        // Add a mesh
        tinygltf::Mesh mesh;
        mesh.name = "TestMesh";
        model.meshes.push_back(mesh);
        
        // Test with different combinations of parameters
        for (int i = 0; i < 4; ++i) {
            bool embedImages = (i & 1) != 0;
            bool embedBuffers = (i & 2) != 0;
            
            try {
                bool result = loader.WriteGltfSceneToFile(
                    &model,
                    "./dummy_file",
                    embedImages,
                    embedBuffers,
                    true,  // prettyPrint
                    false  // writeBinary
                );
                (void)result;
                
                // Also test binary format
                result = loader.WriteGltfSceneToFile(
                    &model,
                    "./dummy_file",
                    embedImages,
                    embedBuffers,
                    false, // prettyPrint
                    true   // writeBinary
                );
                (void)result;
            } catch (...) {
                // Catch any exceptions from invalid UTF-8 in JSON library
            }
        }
    }
    
    // Test ExpandFilePath
    {
        std::string test_path;
        if (Size > 0) {
            test_path.assign(reinterpret_cast<const char*>(Data), 
                           std::min(Size, static_cast<size_t>(100)));
        } else {
            test_path = "./dummy_file";
        }
        
        std::string expanded = tinygltf::ExpandFilePath(test_path, nullptr);
        (void)expanded;
    }
    
    // Test GetFileSizeInBytes
    {
        size_t file_size = 0;
        std::string err;
        bool result = tinygltf::GetFileSizeInBytes(&file_size, &err, "./dummy_file", nullptr);
        (void)result;
    }
    
    // Test FileExists
    {
        bool exists = tinygltf::FileExists("./dummy_file", nullptr);
        (void)exists;
        
        // Also test with non-existent file
        exists = tinygltf::FileExists("./non_existent_file", nullptr);
        (void)exists;
    }
    
    // Test WriteWholeFile - FIXED: Check if contents is empty before calling
    {
        std::string err;
        std::vector<unsigned char> contents;
        
        // Create content from fuzzer data
        if (Size > 0) {
            contents.assign(Data, Data + Size);
        }
        
        // Only call WriteWholeFile if contents is non-empty
        if (!contents.empty()) {
            bool result = tinygltf::WriteWholeFile(&err, "./dummy_file", contents, nullptr);
            (void)result;
        }
        
        // Test with empty contents - skip the write operation entirely as per documentation
        // We don't call WriteWholeFile with empty contents to avoid the at(0) error
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}