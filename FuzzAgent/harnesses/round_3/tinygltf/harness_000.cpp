#include <cstdint>
#include <cstring>
#include <string>
#include <sstream>
#include "tiny_gltf.h"

namespace harness_000 {
/*
 * Fuzzing harness for tinygltf library (GLTF 2.0 loader/serializer)
 * Based on existing tinygltf fuzzing patterns
 * Targets: LoadASCIIFromString, LoadBinaryFromMemory, WriteGltfSceneToStream
 */


// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION
static void fuzz_ascii(const uint8_t *data, size_t size) {
  tinygltf::Model model;
  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  const char *str = reinterpret_cast<const char *>(data);

  bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, str,
                                     static_cast<unsigned int>(size), /* base_dir */ "");
  (void)ret;
}

static void fuzz_binary(const uint8_t *data, size_t size) {
  tinygltf::Model model;
  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn, data,
                                      static_cast<unsigned int>(size),
                                      /* base_dir */ "");
  (void)ret;
}

static void fuzz_ascii_then_write(const uint8_t *data, size_t size) {
  tinygltf::Model model;
  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  const char *str = reinterpret_cast<const char *>(data);
  
  bool load_ret = ctx.LoadASCIIFromString(&model, &err, &warn, str,
                                         static_cast<unsigned int>(size),
                                         /* base_dir */ "");
  
  // If loading succeeded, test serialization
  if (load_ret && !model.accessors.empty()) {
    std::stringstream stream;
    bool write_ret = ctx.WriteGltfSceneToStream(&model, stream,
                                                /* prettyPrint */ true,
                                                /* writeBinary */ false);
    (void)write_ret;
  }
}

static void fuzz_binary_then_write(const uint8_t *data, size_t size) {
  tinygltf::Model model;
  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  bool load_ret = ctx.LoadBinaryFromMemory(&model, &err, &warn, data,
                                          static_cast<unsigned int>(size),
                                          /* base_dir */ "");
  
  // If loading succeeded, test serialization
  if (load_ret && !model.accessors.empty()) {
    std::stringstream stream;
    bool write_ret = ctx.WriteGltfSceneToStream(&model, stream,
                                                /* prettyPrint */ true,
                                                /* writeBinary */ false);
    (void)write_ret;
  }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size == 0) return 0;

  // Use first byte as selector, rest as payload (similar to existing harness)
  uint8_t selector = data[0] % 4;
  const uint8_t *payload = data + 1;
  size_t payload_size = size - 1;

  if (payload_size == 0) return 0;

  switch (selector) {
    case 0:
      fuzz_ascii(payload, payload_size);
      break;
    case 1:
      fuzz_binary(payload, payload_size);
      break;
    case 2:
      fuzz_ascii_then_write(payload, payload_size);
      break;
    case 3:
      fuzz_binary_then_write(payload, payload_size);
      break;
  }

  return 0;
}

} // namespace harness_000
