#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_004 {
/*
 * Fuzzing harness for tinygltf library - targeting equality operators for glTF structs
 * This harness tests comparison operations between glTF models, materials, textures, and other structs.
 * 
 * Targets:
 * - Model::operator==
 * - Material::operator==
 * - Texture::operator==
 * - Accessor::operator==
 * - Animation::operator==
 * - Buffer::operator==
 * - BufferView::operator==
 * - Image::operator==
 * - Mesh::operator==
 * - Node::operator==
 * - Scene::operator==
 * - Camera::operator==
 * - Light::operator==
 * - Sampler::operator==
 * - Skin::operator==
 * - And other equality operators for glTF structs
 * 
 * Strategy: Creates pairs of glTF objects with variations based on fuzzer input,
 * then tests equality/inequality comparisons to exercise comparison logic branches.
 * Uses FuzzedDataProvider for structured input splitting.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Helper to create test strings from fuzzer input
static std::string consume_test_string(FuzzedDataProvider& fdp, size_t max_len = 256) {
    return fdp.ConsumeRandomLengthString(max_len);
}

// Helper to create test integer
static int consume_test_int(FuzzedDataProvider& fdp) {
    return fdp.ConsumeIntegral<int>();
}

// Helper to create test double
static double consume_test_double(FuzzedDataProvider& fdp) {
    return fdp.ConsumeFloatingPoint<double>();
}

// Helper to create test bool
static bool consume_test_bool(FuzzedDataProvider& fdp) {
    return fdp.ConsumeBool();
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to choose which equality tests to run
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    uint8_t test_type = selector % 16;  // 16 different test types
    
    // For each test type, we'll create two objects and compare them
    // We'll vary some properties based on fuzzer input to test both equality and inequality
    
    bool result = false;
    
    switch (test_type) {
        case 0: {
            // Test Model equality
            tinygltf::Model model1, model2;
            
            // Consume some properties to potentially differentiate models
            model1.defaultScene = consume_test_int(fdp);
            model2.defaultScene = consume_test_int(fdp);
            
            // Add some extensions
            if (consume_test_bool(fdp)) {
                model1.extensionsUsed.push_back(consume_test_string(fdp, 50));
            }
            if (consume_test_bool(fdp)) {
                model2.extensionsUsed.push_back(consume_test_string(fdp, 50));
            }
            
            // Test equality operator
            result = (model1 == model2);
            // Also test self-equality
            bool self_eq = (model1 == model1);
            (void)self_eq; // Use to avoid unused variable warning
            break;
        }
        
        case 1: {
            // Test Material equality
            tinygltf::Material mat1, mat2;
            
            // Set some properties
            mat1.name = consume_test_string(fdp, 100);
            mat2.name = consume_test_string(fdp, 100);
            
            // Set alpha mode
            std::string alpha_modes[] = {"OPAQUE", "MASK", "BLEND"};
            uint8_t alpha_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
            mat1.alphaMode = alpha_modes[alpha_idx];
            alpha_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
            mat2.alphaMode = alpha_modes[alpha_idx];
            
            // Set alpha cutoff
            mat1.alphaCutoff = consume_test_double(fdp);
            mat2.alphaCutoff = consume_test_double(fdp);
            
            // Set doubleSided
            mat1.doubleSided = consume_test_bool(fdp);
            mat2.doubleSided = consume_test_bool(fdp);
            
            result = (mat1 == mat2);
            bool self_eq = (mat1 == mat1);
            (void)self_eq;
            break;
        }
        
        case 2: {
            // Test Texture equality
            tinygltf::Texture tex1, tex2;
            
            tex1.name = consume_test_string(fdp, 100);
            tex2.name = consume_test_string(fdp, 100);
            
            tex1.source = consume_test_int(fdp);
            tex2.source = consume_test_int(fdp);
            
            tex1.sampler = consume_test_int(fdp);
            tex2.sampler = consume_test_int(fdp);
            
            result = (tex1 == tex2);
            bool self_eq = (tex1 == tex1);
            (void)self_eq;
            break;
        }
        
        case 3: {
            // Test Accessor equality
            tinygltf::Accessor acc1, acc2;
            
            acc1.name = consume_test_string(fdp, 100);
            acc2.name = consume_test_string(fdp, 100);
            
            acc1.bufferView = consume_test_int(fdp);
            acc2.bufferView = consume_test_int(fdp);
            
            acc1.componentType = consume_test_int(fdp);
            acc2.componentType = consume_test_int(fdp);
            
            acc1.count = consume_test_int(fdp);
            acc2.count = consume_test_int(fdp);
            
            // Set type from enum values
            int types[] = {
                TINYGLTF_TYPE_SCALAR,
                TINYGLTF_TYPE_VECTOR,
                TINYGLTF_TYPE_VEC2,
                TINYGLTF_TYPE_VEC3,
                TINYGLTF_TYPE_VEC4,
                TINYGLTF_TYPE_MATRIX,
                TINYGLTF_TYPE_MAT2,
                TINYGLTF_TYPE_MAT3,
                TINYGLTF_TYPE_MAT4
            };
            uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 9;
            acc1.type = types[type_idx];
            type_idx = fdp.ConsumeIntegral<uint8_t>() % 9;
            acc2.type = types[type_idx];
            
            result = (acc1 == acc2);
            bool self_eq = (acc1 == acc1);
            (void)self_eq;
            break;
        }
        
        case 4: {
            // Test Animation equality
            tinygltf::Animation anim1, anim2;
            
            anim1.name = consume_test_string(fdp, 100);
            anim2.name = consume_test_string(fdp, 100);
            
            // Add some channels
            tinygltf::AnimationChannel channel;
            channel.sampler = consume_test_int(fdp);
            channel.target_node = consume_test_int(fdp);
            channel.target_path = consume_test_string(fdp, 50);
            
            if (consume_test_bool(fdp)) {
                anim1.channels.push_back(channel);
            }
            if (consume_test_bool(fdp)) {
                anim2.channels.push_back(channel);
            }
            
            result = (anim1 == anim2);
            bool self_eq = (anim1 == anim1);
            (void)self_eq;
            break;
        }
        
        case 5: {
            // Test Buffer equality
            tinygltf::Buffer buf1, buf2;
            
            buf1.name = consume_test_string(fdp, 100);
            buf2.name = consume_test_string(fdp, 100);
            
            // Set some data
            std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(std::min(fdp.remaining_bytes(), size_t(100)));
            if (!buffer_data.empty()) {
                buf1.data = buffer_data;
            }
            
            buffer_data = fdp.ConsumeBytes<uint8_t>(std::min(fdp.remaining_bytes(), size_t(100)));
            if (!buffer_data.empty()) {
                buf2.data = buffer_data;
            }
            
            result = (buf1 == buf2);
            bool self_eq = (buf1 == buf1);
            (void)self_eq;
            break;
        }
        
        case 6: {
            // Test BufferView equality
            tinygltf::BufferView bv1, bv2;
            
            bv1.name = consume_test_string(fdp, 100);
            bv2.name = consume_test_string(fdp, 100);
            
            bv1.buffer = consume_test_int(fdp);
            bv2.buffer = consume_test_int(fdp);
            
            bv1.byteOffset = consume_test_int(fdp);
            bv2.byteOffset = consume_test_int(fdp);
            
            bv1.byteLength = consume_test_int(fdp);
            bv2.byteLength = consume_test_int(fdp);
            
            bv1.byteStride = consume_test_int(fdp);
            bv2.byteStride = consume_test_int(fdp);
            
            result = (bv1 == bv2);
            bool self_eq = (bv1 == bv1);
            (void)self_eq;
            break;
        }
        
        case 7: {
            // Test Image equality
            tinygltf::Image img1, img2;
            
            img1.name = consume_test_string(fdp, 100);
            img2.name = consume_test_string(fdp, 100);
            
            img1.uri = consume_test_string(fdp, 200);
            img2.uri = consume_test_string(fdp, 200);
            
            img1.bufferView = consume_test_int(fdp);
            img2.bufferView = consume_test_int(fdp);
            
            img1.mimeType = consume_test_string(fdp, 50);
            img2.mimeType = consume_test_string(fdp, 50);
            
            result = (img1 == img2);
            bool self_eq = (img1 == img1);
            (void)self_eq;
            break;
        }
        
        case 8: {
            // Test Mesh equality
            tinygltf::Mesh mesh1, mesh2;
            
            mesh1.name = consume_test_string(fdp, 100);
            mesh2.name = consume_test_string(fdp, 100);
            
            // Add a primitive
            tinygltf::Primitive prim;
            prim.mode = consume_test_int(fdp) % 7;  // 0-6 valid modes
            
            if (consume_test_bool(fdp)) {
                mesh1.primitives.push_back(prim);
            }
            if (consume_test_bool(fdp)) {
                mesh2.primitives.push_back(prim);
            }
            
            result = (mesh1 == mesh2);
            bool self_eq = (mesh1 == mesh1);
            (void)self_eq;
            break;
        }
        
        case 9: {
            // Test Node equality
            tinygltf::Node node1, node2;
            
            node1.name = consume_test_string(fdp, 100);
            node2.name = consume_test_string(fdp, 100);
            
            node1.mesh = consume_test_int(fdp);
            node2.mesh = consume_test_int(fdp);
            
            node1.camera = consume_test_int(fdp);
            node2.camera = consume_test_int(fdp);
            
            node1.skin = consume_test_int(fdp);
            node2.skin = consume_test_int(fdp);
            
            // Add some children
            if (consume_test_bool(fdp)) {
                node1.children.push_back(consume_test_int(fdp));
            }
            if (consume_test_bool(fdp)) {
                node2.children.push_back(consume_test_int(fdp));
            }
            
            result = (node1 == node2);
            bool self_eq = (node1 == node1);
            (void)self_eq;
            break;
        }
        
        case 10: {
            // Test Scene equality
            tinygltf::Scene scene1, scene2;
            
            scene1.name = consume_test_string(fdp, 100);
            scene2.name = consume_test_string(fdp, 100);
            
            // Add some nodes
            if (consume_test_bool(fdp)) {
                scene1.nodes.push_back(consume_test_int(fdp));
            }
            if (consume_test_bool(fdp)) {
                scene2.nodes.push_back(consume_test_int(fdp));
            }
            
            result = (scene1 == scene2);
            bool self_eq = (scene1 == scene1);
            (void)self_eq;
            break;
        }
        
        case 11: {
            // Test Camera equality
            tinygltf::Camera cam1, cam2;
            
            cam1.name = consume_test_string(fdp, 100);
            cam2.name = consume_test_string(fdp, 100);
            
            std::string cam_types[] = {"perspective", "orthographic"};
            uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 2;
            cam1.type = cam_types[type_idx];
            type_idx = fdp.ConsumeIntegral<uint8_t>() % 2;
            cam2.type = cam_types[type_idx];
            
            result = (cam1 == cam2);
            bool self_eq = (cam1 == cam1);
            (void)self_eq;
            break;
        }
        
        case 12: {
            // Test Light equality
            tinygltf::Light light1, light2;
            
            light1.name = consume_test_string(fdp, 100);
            light2.name = consume_test_string(fdp, 100);
            
            std::string light_types[] = {"directional", "point", "spot"};
            uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
            light1.type = light_types[type_idx];
            type_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
            light2.type = light_types[type_idx];
            
            result = (light1 == light2);
            bool self_eq = (light1 == light1);
            (void)self_eq;
            break;
        }
        
        case 13: {
            // Test Sampler equality
            tinygltf::Sampler samp1, samp2;
            
            samp1.name = consume_test_string(fdp, 100);
            samp2.name = consume_test_string(fdp, 100);
            
            samp1.magFilter = consume_test_int(fdp);
            samp2.magFilter = consume_test_int(fdp);
            
            samp1.minFilter = consume_test_int(fdp);
            samp2.minFilter = consume_test_int(fdp);
            
            samp1.wrapS = consume_test_int(fdp);
            samp2.wrapS = consume_test_int(fdp);
            
            samp1.wrapT = consume_test_int(fdp);
            samp2.wrapT = consume_test_int(fdp);
            
            result = (samp1 == samp2);
            bool self_eq = (samp1 == samp1);
            (void)self_eq;
            break;
        }
        
        case 14: {
            // Test Skin equality
            tinygltf::Skin skin1, skin2;
            
            skin1.name = consume_test_string(fdp, 100);
            skin2.name = consume_test_string(fdp, 100);
            
            skin1.inverseBindMatrices = consume_test_int(fdp);
            skin2.inverseBindMatrices = consume_test_int(fdp);
            
            skin1.skeleton = consume_test_int(fdp);
            skin2.skeleton = consume_test_int(fdp);
            
            // Add some joints
            if (consume_test_bool(fdp)) {
                skin1.joints.push_back(consume_test_int(fdp));
            }
            if (consume_test_bool(fdp)) {
                skin2.joints.push_back(consume_test_int(fdp));
            }
            
            result = (skin1 == skin2);
            bool self_eq = (skin1 == skin1);
            (void)self_eq;
            break;
        }
        
        case 15: {
            // Test Asset equality (simple struct)
            tinygltf::Asset asset1, asset2;
            
            asset1.version = consume_test_string(fdp, 50);
            asset2.version = consume_test_string(fdp, 50);
            
            asset1.generator = consume_test_string(fdp, 100);
            asset2.generator = consume_test_string(fdp, 100);
            
            asset1.copyright = consume_test_string(fdp, 200);
            asset2.copyright = consume_test_string(fdp, 200);
            
            result = (asset1 == asset2);
            bool self_eq = (asset1 == asset1);
            (void)self_eq;
            break;
        }
    }
    
    // Use result to avoid unused variable warning
    (void)result;
    
    return 0;
}

} // namespace harness_004
