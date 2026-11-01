// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Dummy callback functions matching the expected signatures
static bool DummyURIEncode(const std::string &in_uri, const std::string &base_dir, std::string *out_uri,
                           void *user_data) {
  (void)user_data;
  (void)base_dir;
  if (!out_uri) return false;
  *out_uri = in_uri;
  return true;
}

static bool DummyURIDecode(const std::string &in_uri, std::string *out_uri,
                           void *user_data) {
  (void)user_data;
  if (!out_uri) return false;
  *out_uri = in_uri;
  return true;
}

static bool DummyWriteImageData(const std::string *imagepath,
                                const std::string *base_dir,
                                const tinygltf::Image *image,
                                bool embedImages,
                                const tinygltf::FsCallbacks *fs,
                                const tinygltf::URICallbacks *uri_cb,
                                std::string *out_uri,
                                void *user_data) {
  (void)base_dir;
  (void)embedImages;
  (void)fs;
  (void)uri_cb;
  (void)out_uri;
  (void)user_data;
  if (!image || !imagepath) return false;
  
  if (imagepath->empty()) return true;

  std::ofstream ofs(*imagepath, std::ios::binary);
  if (!ofs) return false;
  if (!image->image.empty()) {
    ofs.write(reinterpret_cast<const char*>(image->image.data()),
              image->image.size());
  }
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Safe access to input data with bounds checking
  bool embedImages = (Size > 0 && Data[0] % 2) == 0;
  bool embedBuffers = (Size > 1 && Data[1] % 2) == 0;
  bool prettyPrint = (Size > 2 && Data[2] % 2) == 0;
  bool writeBinary = (Size > 3 && Data[3] % 2) == 0;
  bool useCustomCallbacks = (Size > 4 && Data[4] % 2) == 0;

  tinygltf::TinyGLTF ctx;
  std::string err;

  // Fuzz SetURICallbacks
  tinygltf::URICallbacks uri_cb;
  if (useCustomCallbacks) {
    uri_cb.encode = DummyURIEncode;
    uri_cb.decode = DummyURIDecode;
    uri_cb.user_data = const_cast<void*>(static_cast<const void*>(Data));
    ctx.SetURICallbacks(uri_cb, &err);
  } else {
    // Test with null callbacks to trigger error handling
    uri_cb.encode = nullptr;
    uri_cb.decode = nullptr;
    uri_cb.user_data = nullptr;
    ctx.SetURICallbacks(uri_cb, &err);
  }

  // Fuzz SetImageWriter
  ctx.SetImageWriter(DummyWriteImageData, const_cast<void*>(static_cast<const void*>(Data)));

  // Prepare a minimal model
  tinygltf::Model model;
  model.asset.version = "2.0";

  // Add some images to test Write functions
  tinygltf::Image img;
  img.name = "test_image";
  img.width = 64;
  // Fill image data with fuzzed input (limited to avoid huge allocations)
  size_t img_size = (Size > 1024) ? 1024 : Size;
  img.image.resize(img_size);
  for (size_t i = 0; i < img_size; ++i) {
    img.image[i] = Data[i];
  }
  model.images.push_back(img);

  // Add a buffer
  tinygltf::Buffer buf;
  buf.name = "test_buffer";
  buf.data.resize(Size);
  for (size_t i = 0; i < Size; ++i) {
    buf.data[i] = Data[i];
  }
  model.buffers.push_back(buf);

  // Fuzz URIDecode directly (standard signature)
  std::string test_uri;
  if (Size > 5) {
    test_uri.assign(reinterpret_cast<const char*>(Data + 5), Size - 5);
  } else {
    test_uri = "test%20uri";
  }
  std::string decoded_uri;
  bool decode_result = tinygltf::URIDecode(test_uri, &decoded_uri, nullptr);
  (void)decode_result;

  // Fuzz WriteGltfSceneToFile
  std::string filename = "./dummy_file.gltf";
  if (writeBinary) {
    filename = "./dummy_file.glb";
  }
  bool write_file_result = ctx.WriteGltfSceneToFile(
      &model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);

  // Fuzz WriteGltfSceneToStream
  std::ostringstream stream;
  bool write_stream_result = ctx.WriteGltfSceneToStream(
      &model, stream, prettyPrint, writeBinary);

  // Cleanup: Remove dummy file if created
  std::remove(filename.c_str());

  return 0;
}