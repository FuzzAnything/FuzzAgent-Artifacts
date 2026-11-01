// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
  FILE *f = fopen("./dummy_file", "wb");
  if (f) {
    fwrite(Data, 1, Size, f);
    fclose(f);
  }
}

// Helper function to run the sequence on a specific Value
static void runSequence(const tinygltf::Value &val) {
  if (val.IsObject()) {
    // Get object value (using string key "test")
    const tinygltf::Value &objVal = val.Get("test");
    
    if (objVal.IsArray()) {
      // Get array element by index
      const tinygltf::Value &arrVal = objVal.Get(0);
      
      if (arrVal.IsArray()) {
         const tinygltf::Value &next = arrVal.Get(0);
         if (next.IsArray()) {
           const tinygltf::Value &next2 = next.Get(0);
           // next2.GetNumberAsDouble()
           double d1 = next2.GetNumberAsDouble();
           
           const tinygltf::Value &next3 = next2.Get(0);
           double d2 = next3.GetNumberAsDouble();
         }
      }
    }
  }
}

// Helper to iterate ExtensionMap and run sequence on values
static void runSequenceOnExtensions(const tinygltf::ExtensionMap &extMap) {
  for (const auto &kv : extMap) {
    runSequence(kv.second);
  }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // 1. Prepare the environment
  WriteDummyFile(Data, Size);

  tinygltf::TinyGLTF parser;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Invoke LoadASCIIFromFile
  bool success = parser.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file",
                                          tinygltf::REQUIRE_VERSION);

  // If loading fails, we can still try to fuzz the Value API with empty/default values
  if (!success) {
    tinygltf::Value dummyVal(0);
    runSequence(dummyVal);
    return 0;
  }

  // 2. Explore program states by invoking the target function sequence
  // Iterate over nodes
  for (const auto &node : model.nodes) {
    runSequence(node.extras);
    runSequenceOnExtensions(node.extensions);
  }

  // Iterate over materials
  for (const auto &mat : model.materials) {
    runSequence(mat.extras);
    runSequenceOnExtensions(mat.extensions);
  }

  // Iterate over scenes
  for (const auto &scene : model.scenes) {
    runSequence(scene.extras);
    runSequenceOnExtensions(scene.extensions);
  }

  // Iterate over buffers
  for (const auto &buf : model.buffers) {
    runSequence(buf.extras);
    runSequenceOnExtensions(buf.extensions);
  }

  // Iterate over images
  for (const auto &img : model.images) {
    runSequence(img.extras);
    runSequenceOnExtensions(img.extensions);
  }

  // Iterate over cameras
  for (const auto &cam : model.cameras) {
    runSequence(cam.extras);
    runSequenceOnExtensions(cam.extensions);
  }

  // Iterate over skins
  for (const auto &skin : model.skins) {
    runSequence(skin.extras);
    runSequenceOnExtensions(skin.extensions);
  }

  // Iterate over animations
  for (const auto &anim : model.animations) {
    runSequence(anim.extras);
    runSequenceOnExtensions(anim.extensions);
  }

  // Iterate over accessors
  for (const auto &acc : model.accessors) {
    runSequence(acc.extras);
    runSequenceOnExtensions(acc.extensions);
  }

  return 0;
}