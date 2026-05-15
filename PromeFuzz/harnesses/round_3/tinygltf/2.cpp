// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
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

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
  std::ofstream file("./dummy_file", std::ios::binary);
  if (file) {
    file.write(reinterpret_cast<const char*>(Data), Size);
  }
}

static void RemoveDummyFile() {
  std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
  // Write fuzzing data to dummy file
  WriteDummyFile(Data, Size);
  
  // Initialize required objects
  tinygltf::TinyGLTF loader;
  tinygltf::Model model;
  std::string err;
  std::string warn;
  
  // Call LoadBinaryFromFile
  bool binary_result = loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
  
  // Reset model for ASCII load
  model = tinygltf::Model();
  err.clear();
  warn.clear();
  
  // Call LoadASCIIFromFile
  bool ascii_result = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
  
  // Prepare for ByteStride calls
  if (!model.accessors.empty() && !model.bufferViews.empty()) {
    // Create Accessor object using default constructor
    tinygltf::Accessor accessor;
    
    // First ByteStride call - try with first buffer view if available
    if (model.accessors[0].bufferView >= 0 && 
        model.accessors[0].bufferView < static_cast<int>(model.bufferViews.size())) {
      const tinygltf::BufferView& bufferView = model.bufferViews[model.accessors[0].bufferView];
      int stride1 = accessor.ByteStride(bufferView);
      (void)stride1; // Use result to avoid unused variable warning
    }
    
    // Second ByteStride call - try with different buffer view if available
    if (model.bufferViews.size() > 1) {
      // Use second buffer view
      const tinygltf::BufferView& bufferView2 = model.bufferViews[1 % model.bufferViews.size()];
      int stride2 = accessor.ByteStride(bufferView2);
      (void)stride2; // Use result to avoid unused variable warning
    } else if (!model.bufferViews.empty()) {
      // Use first buffer view again
      const tinygltf::BufferView& bufferView = model.bufferViews[0];
      int stride2 = accessor.ByteStride(bufferView);
      (void)stride2; // Use result to avoid unused variable warning
    }
  }
  
  // Clean up
  RemoveDummyFile();
  
  return 0;
}