// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
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

static bool CustomLoadImageData(tinygltf::Image* image, int index,
                                std::string* uri,
                                std::string* mime_type,
                                int width, int height,
                                const unsigned char* data, int size,
                                void* user_data) {
  if (!image) return false;
  
  image->width = width > 0 ? width : 1;
  image->height = height > 0 ? height : 1;
  image->component = 3;
  image->bits = 8;
  
  if (size > 0 && data != nullptr) {
    image->image.assign(data, data + size);
  } else {
    image->image.resize(3, 0);
  }
  
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;

  // 1. SetImagesAsIs
  bool flag1 = (Size > 0 && Data[0] % 2) == 1;
  ctx.SetImagesAsIs(flag1);

  // 2. GetImagesAsIs
  bool check1 = ctx.GetImagesAsIs();
  (void)check1;

  // 3. SetPreserveImageChannels
  bool flag2 = (Size > 0 && (Data[0] >> 1) % 2) == 1;
  ctx.SetPreserveImageChannels(flag2);

  // 4. GetPreserveImageChannels
  bool check2 = ctx.GetPreserveImageChannels();
  (void)check2;

  // 5. SetImageLoader
  bool useCustomLoader = (Size > 1 && (Data[1] % 2) == 1);
  if (useCustomLoader) {
    ctx.SetImageLoader(CustomLoadImageData, nullptr);
  } else {
    ctx.SetImageLoader(nullptr, nullptr);
  }

  // 6. WriteGltfSceneToFile
  std::string filename = "./dummy_file.gltf";
  
  bool embedImages = (Size > 2 && (Data[2] % 2) == 1);
  bool embedBuffers = (Size > 3 && (Data[3] % 2) == 1);
  bool prettyPrint = (Size > 4 && (Data[4] % 2) == 1);
  bool writeBinary = (Size > 5 && (Data[5] % 2) == 1);

  bool writeSuccess = ctx.WriteGltfSceneToFile(
      &model, 
      filename, 
      embedImages, 
      embedBuffers, 
      prettyPrint, 
      writeBinary
  );
  (void)writeSuccess;

  // Cleanup
  std::remove(filename.c_str());

  return 0;
}