// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
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

// Helper to convert fuzzer input to a Model with randomized content
static tinygltf::Model CreateModelFromInput(const uint8_t *Data, size_t Size) {
  tinygltf::Model model;
  
  // Set asset version based on input
  if (Size > 0) {
    model.asset.version = (Data[0] % 2 == 0) ? "2.0" : "1.0";
  }

  // Add some buffers
  if (Size > 1) {
    tinygltf::Buffer buffer;
    buffer.name = "buffer_" + std::to_string(Data[1]);
    buffer.data.resize(Size > 10 ? 10 : Size, Data[1]);
    model.buffers.push_back(buffer);
  }

  // Add some buffer views
  if (Size > 2) {
    tinygltf::BufferView view;
    view.buffer = 0;
    view.byteOffset = Data[2];
    view.name = "view_" + std::to_string(Data[2]);
    model.bufferViews.push_back(view);
  }

  // Add some accessors
  if (Size > 3) {
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.byteOffset = Data[3];
    accessor.name = "accessor_" + std::to_string(Data[3]);
    accessor.minValues = {static_cast<double>(Data[3])};
    model.accessors.push_back(accessor);
  }

  // Add some meshes
  if (Size > 4) {
    tinygltf::Mesh mesh;
    mesh.name = "mesh_" + std::to_string(Data[4]);
    tinygltf::Primitive prim;
    prim.attributes["POSITION"] = 0;
    mesh.primitives.push_back(prim);
    model.meshes.push_back(mesh);
  }

  // Add some nodes
  if (Size > 5) {
    tinygltf::Node node;
    node.name = "node_" + std::to_string(Data[5]);
    node.mesh = 0;
    model.nodes.push_back(node);
  }

  // Add a scene
  if (Size > 6) {
    tinygltf::Scene scene;
    scene.name = "scene_" + std::to_string(Data[6]);
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;
  }

  // Add some materials
  if (Size > 7) {
    tinygltf::Material mat;
    mat.name = "mat_" + std::to_string(Data[7]);
    model.materials.push_back(mat);
  }

  // Add some textures
  if (Size > 8) {
    tinygltf::Texture tex;
    tex.name = "tex_" + std::to_string(Data[8]);
    model.textures.push_back(tex);
  }

  // Add some samplers
  if (Size > 9) {
    tinygltf::Sampler sampler;
    sampler.name = "sampler_" + std::to_string(Data[9]);
    model.samplers.push_back(sampler);
  }

  // Add some images
  if (Size > 10) {
    tinygltf::Image img;
    img.name = "img_" + std::to_string(Data[10]);
    img.width = Data[10];
    img.height = Data[10];
    img.image.resize(10, Data[10]);
    model.images.push_back(img);
  }

  // Add some cameras
  if (Size > 11) {
    tinygltf::Camera cam;
    cam.type = (Data[11] % 2 == 0) ? "perspective" : "orthographic";
    if (cam.type == "perspective") {
      cam.perspective.aspectRatio = static_cast<double>(Data[11]);
    } else {
      cam.orthographic.xmag = static_cast<double>(Data[11]);
    }
    model.cameras.push_back(cam);
  }

  // Add some lights
  if (Size > 12) {
    tinygltf::Light light;
    light.name = "light_" + std::to_string(Data[12]);
    light.intensity = static_cast<double>(Data[12]);
    light.color = {1.0, 1.0, 1.0};
    model.lights.push_back(light);
  }

  // Add some audio sources
  if (Size > 13) {
    tinygltf::AudioSource source;
    source.name = "audio_" + std::to_string(Data[13]);
    model.audioSources.push_back(source);
  }

  // Add some audio emitters
  if (Size > 14) {
    tinygltf::AudioEmitter emitter;
    emitter.name = "emitter_" + std::to_string(Data[14]);
    model.audioEmitters.push_back(emitter);
  }

  // Add some skins
  if (Size > 15) {
    tinygltf::Skin skin;
    skin.name = "skin_" + std::to_string(Data[15]);
    model.skins.push_back(skin);
  }

  // Add some animations
  if (Size > 16) {
    tinygltf::Animation anim;
    anim.name = "anim_" + std::to_string(Data[16]);
    model.animations.push_back(anim);
  }

  return model;
}

// Custom file system callbacks for testing matching tinygltf signatures
static bool DummyFileExists(const std::string &path, void *user_data) { 
  (void)path; (void)user_data;
  return false; 
}

static std::string DummyExpandFilePath(const std::string &path, void *user_data) { 
  (void)path; (void)user_data;
  return ""; 
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data) {
  (void)out; (void)err; (void)filename; (void)user_data;
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data) {
  (void)err; (void)filename; (void)contents; (void)user_data;
  return false;
}

// Corrected signature: err is a pointer to string
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) {
  (void)size; (void)err; (void)filename; (void)user_data;
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model = CreateModelFromInput(Data, Size);

  // Test 1: WriteGltfSceneToStream with JSON
  {
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, true, false);
  }

  // Test 2: WriteGltfSceneToStream with Binary
  {
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, false, true);
  }

  // Test 3: WriteGltfSceneToFile with JSON
  {
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.json", false, false, true, false);
  }

  // Test 4: WriteGltfSceneToFile with Binary
  {
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.glb", false, false, false, true);
  }

  // Test 5: WriteGltfSceneToFile with embedded images
  {
    ctx.WriteGltfSceneToFile(&model, "./dummy_file_embed.json", true, false, true, false);
  }

  // Test 6: WriteGltfSceneToFile with embedded buffers
  {
    ctx.WriteGltfSceneToFile(&model, "./dummy_file_embed.glb", false, true, false, true);
  }

  // Test 7: SetSerializeDefaultValues and write
  {
    ctx.SetSerializeDefaultValues(true);
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, true, false);
    ctx.SetSerializeDefaultValues(false);
  }

  // Test 8: WriteGltfStream (static function in tinygltf namespace)
  // Note: In some versions of tinygltf, this might be internal or named differently.
  // If it doesn't exist in the specific version, we skip or mock it.
  // Based on error, it seems WriteGltfStream is not exposed in this version's namespace.
  // We will comment out or remove direct calls to non-existent static functions.
  // Instead, we rely on the class methods which internally might use similar logic.
  // However, if the task requires testing it, we assume it exists in the specific build.
  // Since the compiler says it doesn't exist, we skip direct calls to these static functions
  // unless they are available in the specific tiny_gltf.h included.
  // Given the error, we assume they are not available as public static functions in this build.
  // We will remove the direct calls to tinygltf::WriteGltfStream and tinygltf::WriteGltfFile
  // as they are not found in the namespace in this specific environment.
  // The class methods WriteGltfSceneToFile and WriteGltfSceneToStream cover the functionality.

  // Test 9: WriteGltfFile (static function in tinygltf namespace)
  // Skipped due to compilation error indicating it's not in namespace.

  // Test 10: SetFsCallbacks with dummy callbacks
  {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = DummyFileExists;
    callbacks.ExpandFilePath = DummyExpandFilePath;
    callbacks.ReadWholeFile = DummyReadWholeFile;
    callbacks.WriteWholeFile = DummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    callbacks.user_data = nullptr;
    std::string err;
    ctx.SetFsCallbacks(callbacks, &err);
  }

  // Test 11: WriteGltfSceneToStream with invalid model (nullptr)
  // SKIPPED: This triggers a SEGV in tinygltf.h when accessing model->accessors.size()
  // The library does not handle nullptr gracefully in SerializeGltfModel.
  // {
  //   std::ostringstream stream;
  //   ctx.WriteGltfSceneToStream(nullptr, stream, true, false);
  // }

  // Test 12: WriteGltfSceneToFile with invalid model (nullptr)
  // SKIPPED: Same issue as above.
  // {
  //   ctx.WriteGltfSceneToFile(nullptr, "./dummy_file_null.json", false, false, true, false);
  // }

  // Test 13: WriteGltfStream with bad stream
  // Skipped as function not found.

  // Test 14: WriteGltfFile with invalid path
  // Skipped as function not found.

  // Test 15: SetFsCallbacks with invalid callbacks
  {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = nullptr;
    callbacks.ExpandFilePath = nullptr;
    callbacks.ReadWholeFile = nullptr;
    callbacks.WriteWholeFile = nullptr;
    callbacks.GetFileSizeInBytes = nullptr;
    callbacks.user_data = nullptr;
    std::string err;
    ctx.SetFsCallbacks(callbacks, &err);
  }

  // Test 16: WriteGltfSceneToStream with prettyPrint false
  {
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, false, false);
  }

  // Test 17: WriteGltfSceneToFile with prettyPrint false
  {
    ctx.WriteGltfSceneToFile(&model, "./dummy_file_nopretty.json", false, false, false, false);
  }

  // Test 18: WriteGltfSceneToStream with empty model
  {
    tinygltf::Model emptyModel;
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&emptyModel, stream, true, false);
  }

  // Test 19: WriteGltfSceneToFile with empty model
  {
    tinygltf::Model emptyModel;
    ctx.WriteGltfSceneToFile(&emptyModel, "./dummy_file_empty.json", false, false, true, false);
  }

  // Test 20: WriteGltfStream with empty content
  // Skipped.

  // Test 21: WriteGltfFile with empty content
  // Skipped.

  // Test 22: SetSerializeDefaultValues multiple times
  {
    ctx.SetSerializeDefaultValues(true);
    ctx.SetSerializeDefaultValues(false);
    ctx.SetSerializeDefaultValues(true);
  }

  // Test 23: WriteGltfSceneToStream with binary and prettyPrint
  {
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, true, true);
  }

  // Test 24: WriteGltfSceneToFile with binary and prettyPrint
  {
    ctx.WriteGltfSceneToFile(&model, "./dummy_file_bin_pretty.glb", false, false, true, true);
  }

  // Test 25: WriteGltfSceneToStream with invalid stream
  {
    std::ostringstream stream;
    stream.clear(std::ios::badbit);
    ctx.WriteGltfSceneToStream(&model, stream, true, false);
  }

  // Test 26: WriteGltfSceneToFile with invalid filename
  {
    ctx.WriteGltfSceneToFile(&model, "", false, false, true, false);
  }

  // Test 27: WriteGltfStream with very long content
  // Skipped.

  // Test 28: WriteGltfFile with very long content
  // Skipped.

  // Test 29: SetFsCallbacks with user_data
  {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = DummyFileExists;
    callbacks.ExpandFilePath = DummyExpandFilePath;
    callbacks.ReadWholeFile = DummyReadWholeFile;
    callbacks.WriteWholeFile = DummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    callbacks.user_data = reinterpret_cast<void*>(const_cast<uint8_t*>(Data));
    std::string err;
    ctx.SetFsCallbacks(callbacks, &err);
  }

  // Test 30: WriteGltfSceneToStream with model containing only default values
  {
    tinygltf::Model defaultModel;
    std::ostringstream stream;
    ctx.SetSerializeDefaultValues(true);
    ctx.WriteGltfSceneToStream(&defaultModel, stream, true, false);
    ctx.SetSerializeDefaultValues(false);
  }

  return 0;
}