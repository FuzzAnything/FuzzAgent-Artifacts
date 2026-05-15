#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_004 {
// Harness 004: Equality comparison operator fuzzer for tinygltf
// Targets: Material::operator==, Camera::operator==, Animation::operator==, Node::operator==
// Primitive::operator==, Accessor::operator== and other comparison operators
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and tinygltf
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for operation selection
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create two TinyGLTF contexts and models for comparison
    tinygltf::TinyGLTF ctx1, ctx2;
    tinygltf::Model model1, model2;
    std::string err1, warn1, err2, warn2;
    
    // Consume operation selector (0-5 for different test paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Path 0: Test with fuzzer-generated GLTF ASCII data
    if (operation == 0) {
        // Consume GLTF content from fuzzer input
        size_t gltf_size = fdp.ConsumeIntegralInRange<size_t>(10, fdp.remaining_bytes());
        std::string gltf_content = fdp.ConsumeBytesAsString(gltf_size);
        
        if (!gltf_content.empty()) {
            // Load same content into both models
            bool load_success1 = ctx1.LoadASCIIFromString(&model1, &err1, &warn1,
                                                         gltf_content.c_str(),
                                                         static_cast<unsigned int>(gltf_content.size()),
                                                         "");
            bool load_success2 = ctx2.LoadASCIIFromString(&model2, &err2, &warn2,
                                                         gltf_content.c_str(),
                                                         static_cast<unsigned int>(gltf_content.size()),
                                                         "");
            
            if (load_success1 && load_success2) {
                // Compare entire models (should be equal)
                bool models_equal = (model1 == model2);
                (void)models_equal;
                
                // Compare individual components if they exist
                // Materials
                if (!model1.materials.empty() && !model2.materials.empty()) {
                    bool materials_equal = (model1.materials[0] == model2.materials[0]);
                    (void)materials_equal;
                }
                
                // Cameras  
                if (!model1.cameras.empty() && !model2.cameras.empty()) {
                    bool cameras_equal = (model1.cameras[0] == model2.cameras[0]);
                    (void)cameras_equal;
                }
            }
        }
    }
    // Path 1: Test with fuzzer-generated binary GLB data
    else if (operation == 1) {
        // Consume binary data from fuzzer input
        size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(10, fdp.remaining_bytes());
        std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_size);
        
        if (!binary_data.empty()) {
            bool load_success1 = ctx1.LoadBinaryFromMemory(&model1, &err1, &warn1,
                                                          binary_data.data(),
                                                          static_cast<unsigned int>(binary_data.size()),
                                                          "");
            bool load_success2 = ctx2.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                          binary_data.data(),
                                                          static_cast<unsigned int>(binary_data.size()),
                                                          "");
            
            if (load_success1 && load_success2) {
                // Compare models (should be equal)
                bool models_equal = (model1 == model2);
                (void)models_equal;
                
                // Compare individual components if they exist
                // Animations
                if (!model1.animations.empty() && !model2.animations.empty()) {
                    bool animations_equal = (model1.animations[0] == model2.animations[0]);
                    (void)animations_equal;
                }
                
                // Nodes
                if (!model1.nodes.empty() && !model2.nodes.empty()) {
                    bool nodes_equal = (model1.nodes[0] == model2.nodes[0]);
                    (void)nodes_equal;
                }
            }
        }
    }
    // Path 2: Test inequality by modifying objects
    else if (operation == 2) {
        // Create default objects and modify one
        tinygltf::Material mat1, mat2;
        bool initial_equal = (mat1 == mat2);
        (void)initial_equal;
        
        // Modify mat2
        mat2.name = "modified";
        bool after_mod_equal = (mat1 == mat2);
        (void)after_mod_equal;
        
        // Test with cameras
        tinygltf::Camera cam1, cam2;
        bool cams_equal = (cam1 == cam2);
        (void)cams_equal;
        
        cam2.name = "modified_camera";
        bool cams_modified_equal = (cam1 == cam2);
        (void)cams_modified_equal;
        
        // Test with animations
        tinygltf::Animation anim1, anim2;
        bool anims_equal = (anim1 == anim2);
        (void)anims_equal;
        
        anim2.name = "modified_animation";
        bool anims_modified_equal = (anim1 == anim2);
        (void)anims_modified_equal;
        
        // Test with nodes
        tinygltf::Node node1, node2;
        bool nodes_equal = (node1 == node2);
        (void)nodes_equal;
        
        node2.name = "modified_node";
        bool nodes_modified_equal = (node1 == node2);
        (void)nodes_modified_equal;
    }
    // Path 3: Test Primitive and Accessor operators (from search results)
    else if (operation == 3) {
        // Create primitives
        tinygltf::Primitive prim1, prim2;
        bool prims_equal = (prim1 == prim2);
        (void)prims_equal;
        
        // Modify prim1
        prim1.mode = 4; // TINYGLTF_MODE_TRIANGLES
        prim2.mode = 1; // TINYGLTF_MODE_LINE
        bool prims_diff_equal = (prim1 == prim2);
        (void)prims_diff_equal;
        
        // Create accessors (if operator exists)
        // tinygltf::Accessor acc1, acc2;
        // bool accs_equal = (acc1 == acc2);
        // (void)accs_equal;
        
        // Test other operators: Buffer, Mesh
        tinygltf::Buffer buf1, buf2;
        bool bufs_equal = (buf1 == buf2);
        (void)bufs_equal;
        
        tinygltf::Mesh mesh1, mesh2;
        bool meshes_equal = (mesh1 == mesh2);
        (void)meshes_equal;
        
        // Test TextureInfo operators
        tinygltf::TextureInfo texinfo1, texinfo2;
        bool texinfos_equal = (texinfo1 == texinfo2);
        (void)texinfos_equal;
        
        // Test PbrMetallicRoughness
        tinygltf::PbrMetallicRoughness pbr1, pbr2;
        bool pbrs_equal = (pbr1 == pbr2);
        (void)pbrs_equal;
    }
    // Path 4: Test with mixed data - create objects with some content
    else if (operation == 4) {
        // Create materials with different properties
        tinygltf::Material mat1, mat2;
        
        // Make them different
        mat1.name = "material1";
        mat2.name = "material2";
        
        bool mats_diff = (mat1 == mat2);
        (void)mats_diff;
        
        // Make them equal again
        mat2.name = "material1";
        bool mats_equal_again = (mat1 == mat2);
        (void)mats_equal_again;
        
        // Test camera equality with perspective/orthographic
        tinygltf::Camera cam1, cam2;
        cam1.type = "perspective";
        cam2.type = "perspective";
        bool cams_same_type = (cam1 == cam2);
        (void)cams_same_type;
        
        cam2.type = "orthographic";
        bool cams_diff_type = (cam1 == cam2);
        (void)cams_diff_type;
        
        // Test animation with channels/samplers
        tinygltf::Animation anim1, anim2;
        anim1.name = "animation";
        anim2.name = "animation";
        bool anims_same_name = (anim1 == anim2);
        (void)anims_same_name;
        
        // Add a channel to anim1 only
        tinygltf::AnimationChannel channel;
        channel.sampler = 0;
        channel.target_node = 0;
        anim1.channels.push_back(channel);
        bool anims_diff_channels = (anim1 == anim2);
        (void)anims_diff_channels;
        
        // Add same channel to anim2
        anim2.channels.push_back(channel);
        bool anims_same_channels = (anim1 == anim2);
        (void)anims_same_channels;
    }
    // Path 5: Deep test - create complex objects and compare
    else {
        // Use remaining input for more comprehensive testing
        std::string remaining_data = fdp.ConsumeRemainingBytesAsString();
        
        if (remaining_data.size() > 10) {
            // Try to parse as ASCII
            bool ascii_success = ctx1.LoadASCIIFromString(&model1, &err1, &warn1,
                                                         remaining_data.c_str(),
                                                         static_cast<unsigned int>(remaining_data.size()),
                                                         "");
            // Also try as binary
            std::vector<uint8_t> binary_vec(remaining_data.begin(), remaining_data.end());
            bool binary_success = ctx2.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                           binary_vec.data(),
                                                           static_cast<unsigned int>(binary_vec.size()),
                                                           "");
            
            // Test whatever succeeds
            if (ascii_success || binary_success) {
                // Test default comparison operators for various types
                // This ensures we hit all the operator== implementations
                
                // Material comparison
                tinygltf::Material default_mat1, default_mat2;
                bool default_mats_equal = (default_mat1 == default_mat2);
                (void)default_mats_equal;
                
                // Camera comparison
                tinygltf::Camera default_cam1, default_cam2;
                bool default_cams_equal = (default_cam1 == default_cam2);
                (void)default_cams_equal;
                
                // Animation comparison
                tinygltf::Animation default_anim1, default_anim2;
                bool default_anims_equal = (default_anim1 == default_anim2);
                (void)default_anims_equal;
                
                // Node comparison
                tinygltf::Node default_node1, default_node2;
                bool default_nodes_equal = (default_node1 == default_node2);
                (void)default_nodes_equal;
                
                // Primitive comparison
                tinygltf::Primitive default_prim1, default_prim2;
                bool default_prims_equal = (default_prim1 == default_prim2);
                (void)default_prims_equal;
                
                // Mesh comparison
                tinygltf::Mesh default_mesh1, default_mesh2;
                bool default_meshes_equal = (default_mesh1 == default_mesh2);
                (void)default_meshes_equal;
                
                // Buffer comparison
                tinygltf::Buffer default_buf1, default_buf2;
                bool default_bufs_equal = (default_buf1 == default_buf2);
                (void)default_bufs_equal;
                
                // Accessor comparison (if operator exists)
                // tinygltf::Accessor default_acc1, default_acc2;
                // bool default_accs_equal = (default_acc1 == default_acc2);
                // (void)default_accs_equal;
                
                // Texture comparison
                tinygltf::Texture default_tex1, default_tex2;
                bool default_texts_equal = (default_tex1 == default_tex2);
                (void)default_texts_equal;
                
                // Sampler comparison
                tinygltf::Sampler default_samp1, default_samp2;
                bool default_samps_equal = (default_samp1 == default_samp2);
                (void)default_samps_equal;
                
                // TextureInfo comparison
                tinygltf::TextureInfo default_texinfo1, default_texinfo2;
                bool default_texinfos_equal = (default_texinfo1 == default_texinfo2);
                (void)default_texinfos_equal;
                
                // PbrMetallicRoughness comparison
                tinygltf::PbrMetallicRoughness default_pbr1, default_pbr2;
                bool default_pbrs_equal = (default_pbr1 == default_pbr2);
                (void)default_pbrs_equal;
                
                // Test Model comparison if both loaded
                if (ascii_success && binary_success) {
                    bool loaded_models_equal = (model1 == model2);
                    (void)loaded_models_equal;
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_004
