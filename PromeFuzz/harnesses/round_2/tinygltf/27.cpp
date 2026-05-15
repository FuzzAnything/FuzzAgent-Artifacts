// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static bool dummyFileExists(const std::string &abs_filename, void *) {
  // Simple dummy implementation for fuzzing
  return abs_filename == "./dummy_file";
}

static std::string dummyExpandFilePath(const std::string &filepath, void *) {
  return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char> *out,
                               std::string *err, const std::string &filepath,
                               void *) {
  if (filepath == "./dummy_file") {
    std::ifstream file(filepath, std::ios::binary);
    if (file) {
      file.seekg(0, std::ios::end);
      size_t size = file.tellg();
      file.seekg(0, std::ios::beg);
      out->resize(size);
      file.read(reinterpret_cast<char *>(out->data()), size);
      return true;
    }
  }
  if (err) *err = "File not found";
  return false;
}

static bool dummyWriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents,
                                void *user_data) {
  if (filepath == "./dummy_file") {
    std::ofstream file(filepath, std::ios::binary);
    if (file) {
      file.write(reinterpret_cast<const char *>(contents.data()),
                 contents.size());
      return true;
    }
  }
  if (err) *err = "Write failed";
  return false;
}

static bool dummyGetFileSizeInBytes(size_t *size, std::string *err,
                                    const std::string &filepath,
                                    void *user_data) {
  if (filepath == "./dummy_file") {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (file) {
      *size = file.tellg();
      return true;
    }
  }
  if (err) *err = "File not found";
  return false;
}

static void populateModel(tinygltf::Model *model, const uint8_t *data,
                          size_t size) {
  // Create a minimal valid model with some data from fuzzer input
  model->asset.version = "2.0";
  
  // Add a buffer with some data
  tinygltf::Buffer buffer;
  buffer.name = "buffer0";
  buffer.data.assign(data, data + std::min(size, size_t(1000)));
  model->buffers.push_back(buffer);
  
  // Add a buffer view
  tinygltf::BufferView bufferView;
  bufferView.buffer = 0;
  bufferView.byteOffset = 0;
  bufferView.byteLength = std::min(size, size_t(1000));
  model->bufferViews.push_back(bufferView);
  
  // Add an accessor
  tinygltf::Accessor accessor;
  accessor.bufferView = 0;
  accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
  accessor.type = TINYGLTF_TYPE_VEC3;
  accessor.count = std::min(size / 12, size_t(10));
  model->accessors.push_back(accessor);
  
  // Add a node
  tinygltf::Node node;
  node.name = "node0";
  model->nodes.push_back(node);
  
  // Add a scene
  tinygltf::Scene scene;
  scene.nodes.push_back(0);
  model->scenes.push_back(scene);
  model->defaultScene = 0;
  
  // Add extras string (limited to valid UTF-8 range)
  std::string extras_str;
  for (size_t i = 0; i < std::min(size, size_t(100)); ++i) {
    if (data[i] >= 32 && data[i] < 127) {
      extras_str.push_back(static_cast<char>(data[i]));
    } else {
      extras_str.push_back('?');
    }
  }
  model->asset.extras = tinygltf::Value(extras_str);
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Write dummy file for filesystem operations
  std::ofstream dummy_file("./dummy_file", std::ios::binary);
  if (dummy_file) {
    dummy_file.write(reinterpret_cast<const char *>(Data),
                     std::min(Size, size_t(1000)));
    dummy_file.close();
  }

  // Test DecodeDataURI
  {
    std::vector<unsigned char> out;
    std::string mime_type;
    std::string in_str(reinterpret_cast<const char *>(Data),
                       std::min(Size, size_t(1000)));
    size_t reqBytes = Size % 1000;
    bool checkSize = (Size % 2) == 0;
    tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
  }

  // Test SetFsCallbacks
  {
    tinygltf::TinyGLTF loader;
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = &dummyFileExists;
    callbacks.ExpandFilePath = &dummyExpandFilePath;
    callbacks.ReadWholeFile = &dummyReadWholeFile;
    callbacks.WriteWholeFile = &dummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = &dummyGetFileSizeInBytes;
    callbacks.user_data = nullptr;
    
    std::string err;
    loader.SetFsCallbacks(callbacks, &err);
  }

  // Test FileExists
  {
    std::string filename(reinterpret_cast<const char *>(Data),
                         std::min(Size, size_t(200)));
    tinygltf::FileExists(filename, nullptr);
  }

  // Test LoadASCIIFromString
  {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err, warn;
    std::string base_dir = "./";
    unsigned int check_sections = 1; // TINYGLTF_REQUIRE_VERSION
    
    // Create a minimal glTF JSON string
    std::string gltf_json = R"({
      "asset": { "version": "2.0" },
      "scene": 0,
      "scenes": [ { "nodes": [0] } ],
      "nodes": [ { "mesh": 0 } ],
      "meshes": [ { "primitives": [ { "attributes": { "POSITION": 0 } } ] } ],
      "accessors": [ { "bufferView": 0, "componentType": 5126, "type": "VEC3", "count": 3 } ],
      "bufferViews": [ { "buffer": 0, "byteLength": 36 } ],
      "buffers": [ { "byteLength": 36 } ]
    })";
    
    // Insert some fuzzer data into the JSON (sanitized)
    if (Size > 0) {
      size_t insert_pos = gltf_json.find("\"asset\"");
      if (insert_pos != std::string::npos) {
        std::string extra_data;
        for (size_t i = 0; i < std::min(Size, size_t(50)); ++i) {
          if (Data[i] >= 32 && Data[i] < 127 && Data[i] != '\"' && Data[i] != '\\') {
            extra_data.push_back(static_cast<char>(Data[i]));
          }
        }
        gltf_json.insert(insert_pos, "\"extras\": \"" + extra_data + "\",");
      }
    }
    
    loader.LoadASCIIFromString(&model, &err, &warn, gltf_json.c_str(),
                               gltf_json.length(), base_dir, check_sections);
  }

  // Test WriteGltfSceneToStream
  {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    populateModel(&model, Data, Size);
    
    std::ostringstream stream;
    bool prettyPrint = (Size % 3) == 0;
    bool writeBinary = (Size % 5) == 0;
    loader.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
  }

  // Test IsDataURI
  {
    std::string in_str(reinterpret_cast<const char *>(Data),
                       std::min(Size, size_t(1000)));
    tinygltf::IsDataURI(in_str);
  }

  // Clean up dummy file
  std::remove("./dummy_file");
  
  return 0;
}