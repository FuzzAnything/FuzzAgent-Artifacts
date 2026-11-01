// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// Correct signature for LoadImageDataFunction based on tinygltf.h
// bool LoadImageDataFunction(
//     tinygltf::Image* image,
//     int image_index,
//     const std::string& uri,
//     void* user_data);
// Note: The actual signature in newer versions might vary slightly, 
// but based on the error message, it expects:
// function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>
// However, looking at the error, the expected signature seems to be:
// bool (tinygltf::Image *, int, std::string *, std::string *, int, int, const unsigned char *, int, void *)
// But standard tinygltf usually uses:
// bool LoadImageDataFunction(tinygltf::Image* image, int image_index, const std::string& uri, void* user_data)
// Let's check the error message again: 
// 'function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>'
// This looks like a specific version. Let's adapt to the error message's expected signature.
// Actually, the error says:
// LoadImageDataFunction (aka 'function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>')
// This is very specific. Let's define the function to match this exactly.
// Parameters:
// 1. tinygltf::Image *
// 2. const int (image_index)
// 3. basic_string<char> * (uri?)
// 4. basic_string<char> * (ext?)
// 5. int (width?)
// 6. int (height?)
// 7. const unsigned char * (data?)
// 8. int (size?)
// 9. void * (user_data)

static bool CustomLoadImageData(
    tinygltf::Image* image,
    int image_index,
    std::string* uri,
    std::string* ext,
    int width,
    int height,
    const unsigned char* data,
    int size,
    void* user_data) {
  (void)image_index;
  (void)uri;
  (void)ext;
  (void)width;
  (void)height;
  (void)data;
  (void)size;
  (void)user_data;

  if (image) {
    image->width = 1;
    image->height = 1;
    image->component = 4; // Fixed: changed 'components' to 'component'
    image->image.clear();
    image->image.push_back(0xFF);
    image->image.push_back(0xFF);
    image->image.push_back(0xFF);
    image->image.push_back(0xFF);
  }
  return true;
}

// Correct signature for WriteImageDataFunction based on the error message:
// function<bool (const basic_string<char> *, const basic_string<char> *, const tinygltf::Image *, bool, const tinygltf::FsCallbacks *, const tinygltf::URICallbacks *, basic_string<char> *, void *)>
// Parameters:
// 1. const std::string * (filename?)
// 2. const std::string * (uri?)
// 3. const tinygltf::Image *
// 4. bool (embed?)
// 5. const tinygltf::FsCallbacks *
// 6. const tinygltf::URICallbacks *
// 7. std::string * (ext?)
// 8. void * (user_data)

static bool CustomWriteImageData(
    const std::string* filename,
    const std::string* uri,
    const tinygltf::Image* image,
    bool embed,
    const tinygltf::FsCallbacks* fs_callbacks,
    const tinygltf::URICallbacks* uri_callbacks,
    std::string* ext,
    void* user_data) {
  (void)filename;
  (void)uri;
  (void)image;
  (void)embed;
  (void)fs_callbacks;
  (void)uri_callbacks;
  (void)ext;
  (void)user_data;
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Step 1: Set custom image loader
  ctx.SetImageLoader(CustomLoadImageData, nullptr);

  // Step 2: Write fuzzing input to a dummy file to use as glTF source
  const char *dummy_file = "./dummy_file.gltf";
  FILE *fp = fopen(dummy_file, "wb");
  if (fp) {
    fwrite(Data, sizeof(uint8_t), Size, fp);
    fclose(fp);
  } else {
    return 0;
  }

  // Step 3: Load ASCII glTF from file (may fail, which is expected in fuzzing)
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

  // Step 4: Set custom image writer
  ctx.SetImageWriter(CustomWriteImageData, nullptr);

  // Step 5: Write the model to a new file (output)
  const char *output_file = "./output_file.gltf";
  // Try different combinations of flags to explore more states
  bool embedImages = (Size % 2 == 0);
  bool embedBuffers = (Size % 3 == 0);
  bool prettyPrint = (Size % 4 == 0);
  bool writeBinary = (Size % 5 == 0);

  ctx.WriteGltfSceneToFile(&model, output_file, embedImages, embedBuffers, prettyPrint, writeBinary);

  // Cleanup: remove dummy files
  std::remove(dummy_file);
  std::remove(output_file);

  return 0;
}