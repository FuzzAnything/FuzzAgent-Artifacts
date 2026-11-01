// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <random>
#include <functional>
#include "tiny_gltf.h"

// Helper to generate random data from fuzz input
static std::vector<unsigned char> GenerateRandomData(const uint8_t *Data, size_t Size) {
    if (Size == 0) return std::vector<unsigned char>(1, 0);
    return std::vector<unsigned char>(Data, Data + Size);
}

// Helper to generate a random Model with fuzzed content
static tinygltf::Model GenerateFuzzedModel(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    
    // Randomly populate model fields to trigger various serialization paths
    if (Size > 0) {
        // Add some buffers
        if (Data[0] & 1) {
            tinygltf::Buffer buf;
            buf.data = GenerateRandomData(Data, Size);
            buf.name = "fuzz_buffer";
            model.buffers.push_back(buf);
        }
        
        // Add some buffer views
        if (Data[0] & 2) {
            tinygltf::BufferView view;
            view.buffer = 0;
            view.byteOffset = (Size > 1) ? Data[1] : 0;
            model.bufferViews.push_back(view);
        }
        
        // Add some accessors
        if (Data[0] & 4) {
            tinygltf::Accessor acc;
            acc.bufferView = 0;
            acc.componentType = (Size > 2) ? Data[2] : 0;
            acc.count = (Size > 3) ? Data[3] : 1;
            model.accessors.push_back(acc);
        }
        
        // Add some meshes
        if (Data[0] & 8) {
            tinygltf::Mesh mesh;
            mesh.name = "fuzz_mesh";
            tinygltf::Primitive prim;
            prim.attributes["POSITION"] = 0;
            mesh.primitives.push_back(prim);
            model.meshes.push_back(mesh);
        }
        
        // Add some nodes
        if (Data[0] & 16) {
            tinygltf::Node node;
            node.mesh = 0;
            model.nodes.push_back(node);
        }
        
        // Add some scenes
        if (Data[0] & 32) {
            tinygltf::Scene scene;
            scene.nodes.push_back(0);
            model.scenes.push_back(scene);
            model.defaultScene = 0;
        }
        
        // Add some materials
        if (Data[0] & 64) {
            tinygltf::Material mat;
            mat.name = "fuzz_material";
            model.materials.push_back(mat);
        }
        
        // Add some textures
        if (Data[0] & 128) {
            tinygltf::Texture tex;
            tex.name = "fuzz_texture";
            model.textures.push_back(tex);
        }
        
        // Add some samplers
        if ((Data[0] >> 7) & 1) {
            tinygltf::Sampler samp;
            samp.name = "fuzz_sampler";
            model.samplers.push_back(samp);
        }
        
        // Add some images
        if ((Data[0] >> 6) & 1) {
            tinygltf::Image img;
            img.name = "fuzz_image";
            img.image = GenerateRandomData(Data, Size);
            model.images.push_back(img);
        }
        
        // Add some animations
        if ((Data[0] >> 5) & 1) {
            tinygltf::Animation anim;
            anim.name = "fuzz_animation";
            model.animations.push_back(anim);
        }
        
        // Add some skins
        if ((Data[0] >> 4) & 1) {
            tinygltf::Skin skin;
            skin.name = "fuzz_skin";
            model.skins.push_back(skin);
        }
        
        // Add some cameras
        if ((Data[0] >> 3) & 1) {
            tinygltf::Camera cam;
            cam.type = "perspective";
            model.cameras.push_back(cam);
        }
        
        // Add some lights
        if ((Data[0] >> 2) & 1) {
            tinygltf::Light light;
            light.name = "fuzz_light";
            model.lights.push_back(light);
        }
        
        // Add some audio sources
        if ((Data[0] >> 1) & 1) {
            tinygltf::AudioSource src;
            src.name = "fuzz_audio";
            model.audioSources.push_back(src);
        }
        
        // Add some audio emitters
        if ((Data[0] >> 0) & 1) {
            tinygltf::AudioEmitter emitter;
            emitter.name = "fuzz_emitter";
            model.audioEmitters.push_back(emitter);
        }
        
        // Set asset info
        model.asset.version = "2.0";
        model.asset.generator = "fuzz_test";
    }
    
    return model;
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF ctx;
    
    // Generate a fuzzed model
    tinygltf::Model model = GenerateFuzzedModel(Data, Size);
    
    // Test 1: GetSerializeDefaultValues (default state)
    bool defaultState = ctx.GetSerializeDefaultValues();
    (void)defaultState; // Suppress unused warning
    
    // Test 2: SetSerializeDefaultValues to true
    ctx.SetSerializeDefaultValues(true);
    bool enabledState = ctx.GetSerializeDefaultValues();
    (void)enabledState;
    
    // Test 3: WriteGltfSceneToStream (JSON format)
    std::ostringstream jsonStream;
    bool jsonSuccess = ctx.WriteGltfSceneToStream(&model, jsonStream, true, false);
    (void)jsonSuccess;
    
    // Test 4: WriteGltfSceneToStream (Binary format)
    std::ostringstream binaryStream;
    bool binarySuccess = ctx.WriteGltfSceneToStream(&model, binaryStream, false, true);
    (void)binarySuccess;
    
    // Test 5: WriteGltfSceneToFile (JSON)
    bool fileJsonSuccess = ctx.WriteGltfSceneToFile(&model, "./dummy_file.json", false, false, true, false);
    (void)fileJsonSuccess;
    
    // Test 6: WriteGltfSceneToFile (Binary)
    bool fileBinarySuccess = ctx.WriteGltfSceneToFile(&model, "./dummy_file.glb", false, false, false, true);
    (void)fileBinarySuccess;
    
    // Test 7: WriteGltfSceneToFile with embedded buffers
    bool embedBufferSuccess = ctx.WriteGltfSceneToFile(&model, "./dummy_file_embed.json", false, true, true, false);
    (void)embedBufferSuccess;
    
    // Test 8: WriteGltfSceneToFile with embedded images
    bool embedImageSuccess = ctx.WriteGltfSceneToFile(&model, "./dummy_file_img.json", true, false, true, false);
    (void)embedImageSuccess;
    
    // Test 9: WriteGltfStream (using the static function from tinygltf namespace if available, 
    // otherwise we simulate it or skip if not directly exposed in the public API)
    // Note: WriteGltfStream might be internal, so we'll try to use it if accessible
    // If it's not accessible, we'll comment it out or use an alternative
    std::string jsonContent = jsonStream.str();
    std::ostringstream streamForWrite;
    // Attempt to call WriteGltfStream if it exists in the namespace
    // Since the error indicates it's not found, we'll skip this direct call
    // and rely on the stream operations we already did
    (void)jsonContent;
    (void)streamForWrite;
    
    // Test 10: SerializeGltfBuffer
    // This function might be internal, so we'll skip direct calls if not accessible
    // The function signature suggests it's static and in tinygltf namespace
    // If it's not accessible, we'll skip this test
    if (!model.buffers.empty()) {
        // We cannot directly call SerializeGltfBuffer if it's not exposed
        // So we'll skip this specific test
        (void)model.buffers;
    }
    
    // Test 11: SetSerializeDefaultValues to false and test again
    ctx.SetSerializeDefaultValues(false);
    bool disabledState = ctx.GetSerializeDefaultValues();
    (void)disabledState;
    
    // Test 12: Write with disabled default values
    std::ostringstream jsonStream2;
    bool jsonSuccess2 = ctx.WriteGltfSceneToStream(&model, jsonStream2, true, false);
    (void)jsonSuccess2;
    
    // Test 13: Edge case - empty model
    tinygltf::Model emptyModel;
    std::ostringstream emptyStream;
    bool emptySuccess = ctx.WriteGltfSceneToStream(&emptyModel, emptyStream, true, false);
    (void)emptySuccess;
    
    // Test 14: Edge case - invalid stream
    std::ostringstream badStream;
    badStream.setstate(std::ios::badbit);
    // We cannot call WriteGltfStream directly if it's not accessible
    // So we'll just ensure the stream is in a bad state
    (void)badStream;
    
    return 0;
}