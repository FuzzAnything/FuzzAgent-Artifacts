// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
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
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // 1. Prepare the environment
  tinygltf::TinyGLTF parser;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Write fuzz data to a dummy file
  const std::string filename = "./dummy_file";
  {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
      return 0;
    }
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();
  }

  // 2. Invoke target function: LoadASCIIFromFile
  // Note: We ignore the return value for fuzzing stability, as parsing failures are expected.
  parser.LoadASCIIFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);

  // 3. Explore program states by invoking the specified API chain
  // We attempt to access various parts of the loaded model to trigger the Value API calls.
  
  // Helper lambda to safely traverse and invoke the API chain on a given Value
  auto traverse_and_test = [](const tinygltf::Value &val) {
    // tinygltf::Value::IsObject
    if (val.IsObject()) {
      // tinygltf::Value::Get (Object overload) - try to get a common key
      // We try "extras" as it's common in glTF
      const tinygltf::Value &obj_val = val.Get("extras");
      
      // tinygltf::Value::IsArray
      if (obj_val.IsArray()) {
        // tinygltf::Value::Get (Array overload) - index 0
        const tinygltf::Value &arr_val_0 = obj_val.Get(0);
        
        // tinygltf::Value::Get (Array overload) - index 1
        const tinygltf::Value &arr_val_1 = obj_val.Get(1);
        
        // tinygltf::Value::Get (Array overload) - index 2
        const tinygltf::Value &arr_val_2 = obj_val.Get(2);
        
        // tinygltf::Value::Get (Array overload) - index 3
        const tinygltf::Value &arr_val_3 = obj_val.Get(3);
        
        // tinygltf::Value::GetNumberAsDouble
        // We attempt to cast, even if the type might not be a number, 
        // to test internal handling or potential UB if not guarded (though spec says check type).
        // In a real safe driver, we'd check IsNumber(), but the task asks to invoke these specific functions.
        // To avoid immediate crash on non-numbers if the implementation doesn't guard, 
        // we rely on the fact that fuzzing often hits undefined behavior or asserts.
        // However, to keep the fuzzer running, we only call if it looks like a number or we accept the crash.
        // The prompt implies invoking the functions. Let's invoke them.
        (void)arr_val_0.GetNumberAsDouble();
        (void)arr_val_1.GetNumberAsDouble();
        (void)arr_val_2.GetNumberAsDouble();
        (void)arr_val_3.GetNumberAsDouble();
      }
    }
  };

  // Test on model extras
  traverse_and_test(model.extras);

  // Test on assets
  traverse_and_test(model.asset.extras);

  // Test on scenes
  for (const auto &scene : model.scenes) {
    traverse_and_test(scene.extras);
  }

  // Test on nodes
  for (const auto &node : model.nodes) {
    traverse_and_test(node.extras);
  }

  // Test on meshes
  for (const auto &mesh : model.meshes) {
    traverse_and_test(mesh.extras);
    for (const auto &prim : mesh.primitives) {
      traverse_and_test(prim.extras);
    }
  }

  // Test on materials
  for (const auto &mat : model.materials) {
    traverse_and_test(mat.extras);
    traverse_and_test(mat.pbrMetallicRoughness.extras);
    traverse_and_test(mat.normalTexture.extras);
    traverse_and_test(mat.occlusionTexture.extras);
    traverse_and_test(mat.emissiveTexture.extras);
  }

  // Test on cameras
  for (const auto &cam : model.cameras) {
    traverse_and_test(cam.extras);
    traverse_and_test(cam.perspective.extras);
    traverse_and_test(cam.orthographic.extras);
  }

  // Test on lights
  for (const auto &light : model.lights) {
    traverse_and_test(light.extras);
    traverse_and_test(light.spot.extras);
  }

  // Test on audio emitters
  for (const auto &emitter : model.audioEmitters) {
    traverse_and_test(emitter.extras);
    traverse_and_test(emitter.positional.extras);
  }

  // Test on audio sources
  for (const auto &source : model.audioSources) {
    traverse_and_test(source.extras);
  }

  // Test on samplers
  for (const auto &sampler : model.samplers) {
    traverse_and_test(sampler.extras);
  }

  // Test on skins
  for (const auto &skin : model.skins) {
    traverse_and_test(skin.extras);
  }

  // Test on images
  for (const auto &img : model.images) {
    traverse_and_test(img.extras);
  }

  // Test on textures
  for (const auto &tex : model.textures) {
    traverse_and_test(tex.extras);
  }

  // Test on buffers
  for (const auto &buf : model.buffers) {
    traverse_and_test(buf.extras);
  }

  // Test on buffer views
  for (const auto &bview : model.bufferViews) {
    traverse_and_test(bview.extras);
  }

  // Test on accessors
  for (const auto &acc : model.accessors) {
    traverse_and_test(acc.extras);
    traverse_and_test(acc.sparse.extras);
    traverse_and_test(acc.sparse.indices.extras);
    traverse_and_test(acc.sparse.values.extras);
  }

  // Test on animations
  for (const auto &anim : model.animations) {
    traverse_and_test(anim.extras);
    for (const auto &ch : anim.channels) {
      traverse_and_test(ch.extras);
    }
    for (const auto &sampler : anim.samplers) {
      traverse_and_test(sampler.extras);
    }
  }

  return 0;
}