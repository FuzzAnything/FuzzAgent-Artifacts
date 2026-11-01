// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
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
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

// Helper function to generate a random Model based on input data
static tinygltf::Model GenerateRandomModel(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    
    // Seed random generator with input data if available
    if (Size > 0) {
        std::srand(static_cast<unsigned int>(Data[0]) ^ (Size % 256));
    } else {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    // Randomly populate model fields to explore different states
    model.asset.version = "2.0";
    
    // Add random scenes
    if (Size > 10 && Data[1] % 2 == 0) {
        tinygltf::Scene scene;
        scene.name = "fuzz_scene";
        model.scenes.push_back(scene);
    }

    // Add random nodes
    if (Size > 20 && Data[2] % 3 == 0) {
        tinygltf::Node node;
        node.name = "fuzz_node";
        model.nodes.push_back(node);
    }

    // Add random meshes
    if (Size > 30 && Data[3] % 2 == 0) {
        tinygltf::Mesh mesh;
        mesh.name = "fuzz_mesh";
        tinygltf::Primitive prim;
        prim.attributes["POSITION"] = 0;
        mesh.primitives.push_back(prim);
        model.meshes.push_back(mesh);
    }

    // Add random materials
    if (Size > 40 && Data[4] % 2 == 0) {
        tinygltf::Material mat;
        mat.name = "fuzz_material";
        model.materials.push_back(mat);
    }

    // Add random textures
    if (Size > 50 && Data[5] % 2 == 0) {
        tinygltf::Texture tex;
        tex.name = "fuzz_texture";
        model.textures.push_back(tex);
    }

    // Add random samplers
    if (Size > 60 && Data[6] % 2 == 0) {
        tinygltf::Sampler sampler;
        sampler.name = "fuzz_sampler";
        model.samplers.push_back(sampler);
    }

    // Add random cameras
    if (Size > 70 && Data[7] % 2 == 0) {
        tinygltf::Camera cam;
        cam.type = "perspective";
        model.cameras.push_back(cam);
    }

    // Add random skins
    if (Size > 80 && Data[8] % 2 == 0) {
        tinygltf::Skin skin;
        skin.name = "fuzz_skin";
        model.skins.push_back(skin);
    }

    // Add random animations
    if (Size > 90 && Data[9] % 2 == 0) {
        tinygltf::Animation anim;
        anim.name = "fuzz_anim";
        model.animations.push_back(anim);
    }

    // Add random buffers
    if (Size > 100 && Data[10] % 2 == 0) {
        tinygltf::Buffer buf;
        buf.name = "fuzz_buffer";
        buf.data.resize(Size % 1024);
        for (size_t i = 0; i < buf.data.size(); ++i) {
            buf.data[i] = Data[i % Size];
        }
        model.buffers.push_back(buf);
    }

    // Add random buffer views
    if (Size > 110 && Data[11] % 2 == 0) {
        tinygltf::BufferView bv;
        bv.name = "fuzz_buffer_view";
        bv.buffer = 0;
        model.bufferViews.push_back(bv);
    }

    // Add random accessors
    if (Size > 120 && Data[12] % 2 == 0) {
        tinygltf::Accessor acc;
        acc.name = "fuzz_accessor";
        acc.bufferView = 0;
        model.accessors.push_back(acc);
    }

    // Add random images
    if (Size > 130 && Data[13] % 2 == 0) {
        tinygltf::Image img;
        img.name = "fuzz_image";
        img.width = Size % 100;
        img.image.resize(Size % 1024);
        for (size_t i = 0; i < img.image.size(); ++i) {
            img.image[i] = Data[i % Size];
        }
        model.images.push_back(img);
    }

    // Add random lights
    if (Size > 140 && Data[14] % 2 == 0) {
        tinygltf::Light light;
        light.name = "fuzz_light";
        model.lights.push_back(light);
    }

    // Add random audio sources
    if (Size > 150 && Data[15] % 2 == 0) {
        tinygltf::AudioSource source;
        source.name = "fuzz_audio_source";
        model.audioSources.push_back(source);
    }

    // Add random audio emitters
    if (Size > 160 && Data[16] % 2 == 0) {
        tinygltf::AudioEmitter emitter;
        emitter.name = "fuzz_audio_emitter";
        model.audioEmitters.push_back(emitter);
    }

    return model;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model = GenerateRandomModel(Data, Size);

    // Test 1: WriteGltfSceneToFile with various flags
    bool embedImages = (Size > 1 && Data[1] % 2 == 0);
    bool embedBuffers = (Size > 2 && Data[2] % 2 == 0);
    bool prettyPrint = (Size > 3 && Data[3] % 2 == 0);
    bool writeBinary = (Size > 4 && Data[4] % 2 == 0);
    
    (void)ctx.WriteGltfSceneToFile(&model, "./dummy_file_scene.gltf", 
                                   embedImages, embedBuffers, prettyPrint, writeBinary);

    // Test 2: WriteGltfSceneToStream
    std::ostringstream stream2;
    (void)ctx.WriteGltfSceneToStream(&model, stream2, prettyPrint, writeBinary);

    // Test 3: GetSerializeDefaultValues and SetSerializeDefaultValues
    bool serializeDefaults = (Size > 5 && Data[5] % 2 == 0);
    ctx.SetSerializeDefaultValues(serializeDefaults);
    (void)ctx.GetSerializeDefaultValues();

    // Test 4: WriteGltfSceneToFile with modified model
    tinygltf::Model model2 = model;
    if (Size > 6) {
        // Correctly set extras using the Value constructor
        tinygltf::Value::Object obj;
        obj["fuzz_key"] = tinygltf::Value(static_cast<double>(Data[6]));
        model2.asset.extras = tinygltf::Value(std::move(obj));
    }
    (void)ctx.WriteGltfSceneToFile(&model2, "./dummy_file_scene2.gltf", 
                                   !embedImages, !embedBuffers, !prettyPrint, !writeBinary);

    // Test 5: WriteGltfSceneToStream with empty model
    tinygltf::Model emptyModel;
    std::ostringstream stream4;
    (void)ctx.WriteGltfSceneToStream(&emptyModel, stream4, true, false);

    // Test 6: WriteGltfSceneToFile with binary flag
    (void)ctx.WriteGltfSceneToFile(&model, "./dummy_file_binary.gltf", 
                                   false, false, false, true);

    // Test 7: WriteGltfSceneToFile with all flags true
    (void)ctx.WriteGltfSceneToFile(&model, "./dummy_file_all_true.gltf", 
                                   true, true, true, true);

    // Test 8: WriteGltfSceneToFile with all flags false
    (void)ctx.WriteGltfSceneToFile(&model, "./dummy_file_all_false.gltf", 
                                   false, false, false, false);

    // Test 9: WriteGltfSceneToStream with large content
    std::ostringstream stream5;
    (void)ctx.WriteGltfSceneToStream(&model, stream5, false, false);

    // Test 10: WriteGltfSceneToStream with binary output
    std::ostringstream stream6;
    (void)ctx.WriteGltfSceneToStream(&model, stream6, false, true);

    // Test 11: SetSerializeDefaultValues multiple times
    ctx.SetSerializeDefaultValues(true);
    ctx.SetSerializeDefaultValues(false);
    ctx.SetSerializeDefaultValues(true);
    (void)ctx.GetSerializeDefaultValues();

    // Test 12: WriteGltfSceneToFile with special names (Safe access)
    tinygltf::Model specialModel = model;
    // Ensure scenes vector is not empty before accessing index 0
    if (!specialModel.scenes.empty()) {
        specialModel.scenes[0].name = "fuzz_special_scene";
    } else {
        // If empty, add a scene to test modification
        tinygltf::Scene scene;
        scene.name = "fuzz_special_scene";
        specialModel.scenes.push_back(scene);
    }
    (void)ctx.WriteGltfSceneToFile(&specialModel, "./dummy_file_special.gltf", 
                                   false, false, true, false);

    return 0;
}