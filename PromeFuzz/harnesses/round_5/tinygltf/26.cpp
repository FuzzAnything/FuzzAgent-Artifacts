// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
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

static void WriteDummyFile(const uint8_t *data, size_t size) {
  std::ofstream file("./dummy_file", std::ios::binary);
  if (file) {
    file.write(reinterpret_cast<const char*>(data), size);
  }
}

static void RemoveDummyFile() {
  std::remove("./dummy_file");
}

static bool dummyFileExists(const std::string&, void*) { return false; }
static std::string dummyExpandFilePath(const std::string& path, void*) { return path; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return false; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return false; }
static bool dummyGetFileSizeInBytes(size_t*, std::string*, const std::string&, void*) { return false; }

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  WriteDummyFile(Data, Size);

  // Test GetFileSizeInBytes
  {
    size_t filesize;
    std::string err;
    bool ret = tinygltf::GetFileSizeInBytes(&filesize, &err, "./dummy_file", nullptr);
  }

  // Test WriteGltfSceneToFile
  {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Initialize some model data
    model.asset.version = "2.0";
    
    // Add a simple scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model.scenes.push_back(scene);
    
    // Add a node
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    
    // Add a mesh
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    model.meshes.push_back(mesh);
    
    // Add a buffer
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    buffer.data.assign(Data, Data + std::min(Size, static_cast<size_t>(100)));
    model.buffers.push_back(buffer);
    
    // Test with different combinations of parameters
    bool embedImages = (Data[0] & 0x01) != 0;
    bool embedBuffers = (Data[0] & 0x02) != 0;
    bool prettyPrint = (Data[0] & 0x04) != 0;
    bool writeBinary = (Data[0] & 0x08) != 0;
    
    try {
      bool ret = loader.WriteGltfSceneToFile(&model, "./dummy_file",
                                            embedImages, embedBuffers,
                                            prettyPrint, writeBinary);
    } catch (...) {
      // Catch any exceptions from invalid UTF-8 in JSON
    }
  }

  // Test WriteWholeFile
  {
    std::string err;
    std::vector<unsigned char> contents;
    
    if (Size > 0) {
      contents.assign(Data, Data + Size);
    }
    
    if (!contents.empty()) {
      bool ret = tinygltf::WriteWholeFile(&err, "./dummy_file", contents, nullptr);
    }
  }

  // Test FileExists
  {
    bool exists = tinygltf::FileExists("./dummy_file", nullptr);
  }

  // Test ExpandFilePath
  {
    std::string filepath(reinterpret_cast<const char*>(Data), 
                        std::min(Size, static_cast<size_t>(100)));
    std::string expanded = tinygltf::ExpandFilePath(filepath, nullptr);
  }

  // Test SetFsCallbacks
  {
    tinygltf::TinyGLTF loader;
    tinygltf::FsCallbacks callbacks;
    
    callbacks.FileExists = dummyFileExists;
    callbacks.ExpandFilePath = dummyExpandFilePath;
    callbacks.ReadWholeFile = dummyReadWholeFile;
    callbacks.WriteWholeFile = dummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    callbacks.user_data = nullptr;
    
    std::string err;
    bool ret = loader.SetFsCallbacks(callbacks, &err);
    
    // Test with null callbacks (should fail)
    tinygltf::FsCallbacks null_callbacks = {};
    bool ret2 = loader.SetFsCallbacks(null_callbacks, &err);
  }

  RemoveDummyFile();
  return 0;
}