// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include "tiny_gltf.h"

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF loader;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Step 1: Prepare environment and write data to dummy files
  // We create two files: one for binary (.glb) and one for ASCII (.gltf)
  // The fuzzer input is used to populate the content of these files.
  // We duplicate the input or use it as-is to ensure we have data for both if needed.
  
  const std::string binary_filename = "./dummy_file.glb";
  const std::string ascii_filename = "./dummy_file.gltf";

  {
    std::ofstream out_bin(binary_filename, std::ios::binary);
    if (out_bin.is_open()) {
      out_bin.write(reinterpret_cast<const char*>(Data), Size);
      out_bin.close();
    }
  }

  {
    std::ofstream out_ascii(ascii_filename, std::ios::binary);
    if (out_ascii.is_open()) {
      out_ascii.write(reinterpret_cast<const char*>(Data), Size);
      out_ascii.close();
    }
  }

  // Step 2: Invoke LoadBinaryFromFile
  // This is the first function in the required sequence.
  // We ignore the return value for fuzzing purposes to ensure execution continues
  // even if parsing fails, allowing us to test subsequent states.
  loader.LoadBinaryFromFile(&model, &err, &warn, binary_filename);

  // Step 3: Invoke LoadASCIIFromFile
  // This is the second function in the required sequence.
  // We reuse the model object to potentially accumulate state or overwrite it.
  loader.LoadASCIIFromFile(&model, &err, &warn, ascii_filename);

  // Step 4: Invoke Accessor::ByteStride twice
  // We need to construct an Accessor and a BufferView to call ByteStride.
  // We attempt to use data from the loaded model if available, 
  // but also test with default constructed objects to cover edge cases.

  tinygltf::Accessor accessor;
  tinygltf::BufferView bufferView;

  // First invocation: Use default constructed objects (edge case: empty/invalid data)
  accessor.ByteStride(bufferView);

  // Second invocation: Try to use data from the loaded model if available
  // This explores states where the model was successfully parsed.
  if (!model.accessors.empty()) {
    // Use the first accessor from the model
    accessor = model.accessors[0];
    
    // If the accessor has a valid bufferView index, try to use that bufferView
    if (accessor.bufferView >= 0 && static_cast<size_t>(accessor.bufferView) < model.bufferViews.size()) {
      bufferView = model.bufferViews[accessor.bufferView];
    } else {
      // If bufferView index is invalid, use default bufferView to test error handling
      bufferView = tinygltf::BufferView();
    }
    
    accessor.ByteStride(bufferView);
  } else {
    // If no accessors were loaded, invoke again with default objects to ensure coverage
    accessor.ByteStride(bufferView);
  }

  // Step 5: Cleanup
  // Remove the dummy files created during the test
  std::remove(binary_filename.c_str());
  std::remove(ascii_filename.c_str());

  return 0;
}