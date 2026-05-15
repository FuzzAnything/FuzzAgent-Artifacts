// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "tiny_gltf.h"

// Dummy image writer callback with correct signature
static bool dummyImageWriter(const std::string* basepath,
                             const std::string* filename,
                             const tinygltf::Image* image,
                             bool embedImages,
                             const tinygltf::FsCallbacks* fs,
                             const tinygltf::URICallbacks* uri,
                             std::string* out_uri,
                             void* user_data) {
  return true;
}

// Helper to create a valid UTF-8 string from fuzzer data
static std::string createValidUTF8String(const uint8_t* data, size_t size) {
  if (size == 0) {
    return "test";
  }
  
  // Create a simple ASCII string from fuzzer data
  std::string result;
  result.reserve(size);
  
  for (size_t i = 0; i < size; ++i) {
    // Only include printable ASCII characters
    if (data[i] >= 32 && data[i] <= 126) {
      result.push_back(static_cast<char>(data[i]));
    } else {
      // Replace non-printable with safe character
      result.push_back('a');
    }
  }
  
  // Ensure we have at least some content
  if (result.empty()) {
    return "test";
  }
  
  return result;
}

// Helper to populate a model with fuzzer data using valid UTF-8
static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
  // Set asset version
  model.asset.version = "2.0";
  
  // Create valid UTF-8 strings from fuzzer data
  std::string valid_string = createValidUTF8String(data, size);
  
  // Add some basic structure to the model
  tinygltf::Scene scene;
  scene.name = valid_string.substr(0, std::min(size_t(20), valid_string.size()));
  model.scenes.push_back(scene);
  model.defaultScene = 0;
  
  // Add a node
  tinygltf::Node node;
  node.name = valid_string.substr(0, std::min(size_t(15), valid_string.size()));
  model.nodes.push_back(node);
  model.scenes[0].nodes.push_back(0);
  
  // Add a simple mesh with one primitive
  tinygltf::Mesh mesh;
  mesh.name = valid_string.substr(0, std::min(size_t(15), valid_string.size()));
  
  tinygltf::Primitive primitive;
  primitive.material = -1;
  primitive.mode = TINYGLTF_MODE_TRIANGLES;
  mesh.primitives.push_back(primitive);
  
  model.meshes.push_back(mesh);
  model.nodes[0].mesh = 0;
  
  // Add a buffer with fuzzer data (binary data, not string)
  if (size > 0) {
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    buffer.data.assign(data, data + std::min(size, size_t(1000))); // Limit size
    model.buffers.push_back(buffer);
    
    // Add a buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = std::min(size, size_t(1000));
    model.bufferViews.push_back(bufferView);
    
    // Add an accessor
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    accessor.count = std::min(size, size_t(1000));
    accessor.type = TINYGLTF_TYPE_SCALAR;
    model.accessors.push_back(accessor);
  }
  
  // Add extras with valid UTF-8 string
  if (!valid_string.empty()) {
    // Use a simple valid string for extras
    std::string safe_extras = "test";
    
    // Add to asset extras
    model.asset.extras = tinygltf::Value(safe_extras);
    
    // Add to a material extras
    tinygltf::Material material;
    material.name = "TestMaterial";
    material.extras = tinygltf::Value(safe_extras);
    model.materials.push_back(material);
    
    // Add to a node extras
    model.nodes[0].extras = tinygltf::Value(safe_extras);
  }
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }
  
  // Initialize model with fuzzer data
  tinygltf::Model model;
  populateModel(model, Data, Size);
  
  // Create TinyGLTF instance
  tinygltf::TinyGLTF loader;
  
  // Use fuzzer data to determine various boolean flags
  // Use first byte for flags, rest for data
  uint8_t flags = Size > 0 ? Data[0] : 0;
  const uint8_t* model_data = Size > 0 ? Data + 1 : Data;
  size_t model_data_size = Size > 0 ? Size - 1 : 0;
  
  // Re-populate model with remaining data
  if (model_data_size > 0) {
    populateModel(model, model_data, model_data_size);
  }
  
  // Set various options based on flags
  loader.SetStoreOriginalJSONForExtrasAndExtensions((flags & 0x01) != 0);
  loader.SetSerializeDefaultValues((flags & 0x02) != 0);
  
  // Set image writer
  loader.SetImageWriter(dummyImageWriter, nullptr);
  
  // Check GetStoreOriginalJSONForExtrasAndExtensions
  bool storeOriginal = loader.GetStoreOriginalJSONForExtrasAndExtensions();
  (void)storeOriginal; // Mark as used
  
  // Test WriteGltfSceneToStream with various combinations
  std::ostringstream stream;
  
  // Combination 1: pretty print, text-based
  bool success1 = false;
  try {
    success1 = loader.WriteGltfSceneToStream(&model, stream, 
                                             (flags & 0x04) != 0,  // prettyPrint
                                             false);               // writeBinary
  } catch (...) {
    // Catch any exceptions to prevent crashes
  }
  
  // Clear stream for next test
  stream.str("");
  stream.clear();
  
  // Combination 2: no pretty print, binary
  bool success2 = false;
  try {
    success2 = loader.WriteGltfSceneToStream(&model, stream,
                                             false,                // prettyPrint
                                             (flags & 0x08) != 0); // writeBinary
  } catch (...) {
    // Catch any exceptions to prevent crashes
  }
  
  // Test WriteGltfSceneToFile
  const std::string filename = "./dummy_file";
  
  // Combination 1: embed images and buffers, pretty print, text-based
  bool success3 = false;
  try {
    success3 = loader.WriteGltfSceneToFile(&model, filename,
                                           (flags & 0x10) != 0,  // embedImages
                                           (flags & 0x20) != 0,  // embedBuffers
                                           (flags & 0x40) != 0,  // prettyPrint
                                           false);               // writeBinary
  } catch (...) {
    // Catch any exceptions to prevent crashes
  }
  
  // Clean up the dummy file
  std::remove(filename.c_str());
  
  // Combination 2: no embedding, no pretty print, binary
  bool success4 = false;
  try {
    success4 = loader.WriteGltfSceneToFile(&model, filename,
                                           false,                // embedImages
                                           false,                // embedBuffers
                                           false,                // prettyPrint
                                           (flags & 0x80) != 0); // writeBinary
  } catch (...) {
    // Catch any exceptions to prevent crashes
  }
  
  // Clean up again
  std::remove(filename.c_str());
  
  // Mark results as used to avoid compiler warnings
  (void)success1;
  (void)success2;
  (void)success3;
  (void)success4;
  
  return 0;
}