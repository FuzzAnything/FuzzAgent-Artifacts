// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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

// Custom image loader callback for fuzzing matching the expected signature
static bool FuzzImageLoader(tinygltf::Image *image,
                            const int,
                            std::string *ext,
                            std::string *mime_type,
                            int,
                            int,
                            const unsigned char *bytes,
                            int size,
                            void *user_data) {
  (void)ext;
  (void)mime_type;
  (void)user_data;
  // Simulate loading by copying raw data if size is reasonable
  if (size > 0 && size < 1024 * 1024) {
    image->image.assign(bytes, bytes + size);
    image->width = 1;
    image->height = 1;
    image->as_is = true;
    return true;
  }
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;

  // Safe access to input data with bounds checking
  uint8_t op = 0;
  uint8_t flag1 = 0;
  uint8_t flag2 = 0;

  if (Size > 0) op = Data[0] % 10;
  if (Size > 1) flag1 = Data[1] % 2;
  if (Size > 2) flag2 = Data[2] % 2;

  switch (op) {
    case 0: {
      // Test SetImagesAsIs and GetImagesAsIs
      bool flag = flag1 != 0;
      ctx.SetImagesAsIs(flag);
      bool result = ctx.GetImagesAsIs();
      (void)result; // Suppress unused warning
      break;
    }
    case 1: {
      // Test SetPreserveImageChannels and GetPreserveImageChannels
      bool flag = flag1 != 0;
      ctx.SetPreserveImageChannels(flag);
      bool result = ctx.GetPreserveImageChannels();
      (void)result;
      break;
    }
    case 2: {
      // Test SetImageLoader
      ctx.SetImageLoader(FuzzImageLoader, nullptr);
      // Reset to default
      ctx.SetImageLoader(nullptr, nullptr);
      break;
    }
    case 3: {
      // Test WriteGltfSceneToFile with empty model
      // Write to a dummy file
      std::string filename = "./dummy_file";
      ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
      break;
    }
    case 4: {
      // Test WriteGltfSceneToFile with binary format
      std::string filename = "./dummy_file";
      ctx.WriteGltfSceneToFile(&model, filename, false, false, true, true);
      break;
    }
    case 5: {
      // Test WriteGltfSceneToFile with embedded images/buffers
      std::string filename = "./dummy_file";
      ctx.WriteGltfSceneToFile(&model, filename, true, true, true, false);
      break;
    }
    case 6: {
      // Populate model with some data before writing
      tinygltf::Asset asset;
      asset.version = "2.0";
      model.asset = asset;

      tinygltf::Scene scene;
      scene.name = "FuzzScene";
      model.scenes.push_back(scene);
      model.defaultScene = 0;

      std::string filename = "./dummy_file";
      ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
      break;
    }
    case 7: {
      // Test chaining SetImagesAsIs and SetPreserveImageChannels
      ctx.SetImagesAsIs(flag1 != 0);
      ctx.SetPreserveImageChannels(flag2 != 0);
      bool img_as_is = ctx.GetImagesAsIs();
      bool preserve_ch = ctx.GetPreserveImageChannels();
      (void)img_as_is;
      (void)preserve_ch;
      break;
    }
    case 8: {
      // Test SetImageLoader with user data
      int dummy_data = 42;
      ctx.SetImageLoader(FuzzImageLoader, &dummy_data);
      ctx.SetImageLoader(nullptr, nullptr);
      break;
    }
    case 9: {
      // Write model with pretty print disabled
      std::string filename = "./dummy_file";
      ctx.WriteGltfSceneToFile(&model, filename, false, false, false, false);
      break;
    }
    default:
      break;
  }

  return 0;
}