// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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

// The signature must match LoadImageDataFunction exactly.
// Based on the error, the library expects pointers to std::string for uri and mime_type.
static bool CustomLoadImageData(tinygltf::Image* image, int index,
                                std::string* uri, std::string* mime_type,
                                int width, int height, const unsigned char* data,
                                int data_length, void* user_data) {
  (void)user_data;
  (void)index;
  (void)uri;
  (void)mime_type;
  (void)width;
  (void)height;
  (void)data;
  (void)data_length;

  if (image) {
    image->width = 1;
    image->height = 1;
    image->image.resize(4, 0);
    image->as_is = true;
  }
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;

  // Use input data to drive configuration flags
  bool flag1 = (Data[0] % 2) != 0;
  bool flag2 = (Size > 1 && (Data[1] % 2) != 0);
  bool flag3 = (Size % 2) != 0;
  bool flag4 = (Size % 3) != 0;
  bool flag5 = (Size % 5) != 0;

  // Test SetPreserveImageChannels and GetPreserveImageChannels
  ctx.SetPreserveImageChannels(flag1);
  bool preserve_state = ctx.GetPreserveImageChannels();
  (void)preserve_state;

  // Test SetImagesAsIs and GetImagesAsIs
  ctx.SetImagesAsIs(flag2);
  bool images_as_is_state = ctx.GetImagesAsIs();
  (void)images_as_is_state;

  // Test SetImageLoader with custom callback and nullptr
  if (flag3) {
    ctx.SetImageLoader(CustomLoadImageData, const_cast<void*>(reinterpret_cast<const void*>(Data)));
  } else {
    ctx.SetImageLoader(nullptr, nullptr);
  }

  // Prepare a dummy file for WriteGltfSceneToFile
  const std::string filename = "./dummy_file.gltf";
  
  // Populate model with some fuzzed data to ensure WriteGltfSceneToFile has content
  if (Size > 10) {
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    // Add a dummy scene
    tinygltf::Scene scene;
    scene.name = "FuzzScene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;

    // Add a dummy buffer with fuzzed data
    tinygltf::Buffer buffer;
    buffer.data.assign(Data, Data + Size);
    model.buffers.push_back(buffer);
  }

  // Test WriteGltfSceneToFile with various combinations
  bool write_result = ctx.WriteGltfSceneToFile(
      &model,
      filename,
      flag4, // embedImages
      flag5, // embedBuffers
      true,  // prettyPrint
      false  // writeBinary
  );
  (void)write_result;

  // Cleanup: remove the dummy file if it exists
  std::remove(filename.c_str());

  // Additional state exploration: toggle flags and re-check
  ctx.SetPreserveImageChannels(!flag1);
  ctx.SetImagesAsIs(!flag2);
  
  // Verify state changes
  bool new_preserve = ctx.GetPreserveImageChannels();
  bool new_as_is = ctx.GetImagesAsIs();
  (void)new_preserve;
  (void)new_as_is;

  return 0;
}