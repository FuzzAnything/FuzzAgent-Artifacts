#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace harness_004 {
/*
 * Fuzzing harness for tinygltf library equality operators
 * 
 * This harness targets 15+ equality operators with 500+ accumulated branches
 * completely uncovered as identified by coverage analysis:
 * - Material::operator== (26 branches, 98 accumulated, 0% covered)
 * - Camera::operator== (12 branches, 48 accumulated, 0% covered)
 * - Animation::operator== (10 branches, 42 accumulated, 0% covered)
 * - Node::operator== (28 branches, 40 accumulated, 0% covered)
 * - Accessor::operator== (22 branches, 34 accumulated, 0% covered)
 * - Mesh::operator== (10 branches, 34 accumulated, 0% covered)
 * - Plus BufferView::operator==, Buffer::operator==, Image::operator==,
 *   Sampler::operator==, Texture::operator==, Scene::operator==,
 *   Skin::operator==, Light::operator==, and others
 * 
 * The harness implements comprehensive equality testing through multiple strategies:
 * 1. Load glTF model and compare with itself via Model::operator==
 * 2. Create duplicate models via copy constructor/assignment and compare
 * 3. Test individual structure equality operators directly
 * 4. Test edge cases: empty structures, partial equality, inequality scenarios
 * 5. Test recursive equality through vector comparisons
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to choose testing strategy
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool store_original_json = fdp.ConsumeBool();
    
    // Consume remaining bytes as glTF data
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (gltf_data.empty()) return 0;
    
    tinygltf::Model model1;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Configure parsing options
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Try to load the model - ignore failures to allow invalid inputs
    std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                         gltf_data.size());
    
    bool load_success = ctx.LoadASCIIFromString(&model1, &err, &warn,
                                               json_str.c_str(),
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "",
                                               check_sections);
    
    // If ASCII loading fails, try binary loading
    if (!load_success) {
        load_success = ctx.LoadBinaryFromMemory(&model1, &err, &warn,
                                               gltf_data.data(),
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "",
                                               check_sections);
    }
    
    // If we couldn't load any model, still test with default/empty structures
    if (!load_success) {
        model1 = tinygltf::Model(); // Ensure we have a clean model
    }
    
    // Test different equality scenarios based on selector
    switch (selector % 6) {
        case 0: {
            // Strategy 1: Compare model with itself (should always be equal)
            bool self_equality = (model1 == model1);
            (void)self_equality; // Use result to prevent optimization
            
            // Test Model::operator== triggers all individual equality operators
            // through vector comparisons of accessors, animations, materials, etc.
            break;
        }
        
        case 1: {
            // Strategy 2: Create duplicate via copy constructor and compare
            tinygltf::Model model2 = model1; // Copy constructor
            bool copy_equality = (model1 == model2);
            (void)copy_equality;
            
            // Test assignment operator too
            tinygltf::Model model3;
            model3 = model1; // Assignment operator
            bool assign_equality = (model1 == model3);
            (void)assign_equality;
            break;
        }
        
        case 2: {
            // Strategy 3: Test individual structure equality operators
            // Create and compare Material objects
            tinygltf::Material mat1, mat2;
            bool mat_equality = (mat1 == mat2);
            (void)mat_equality;
            
            // Create and compare Camera objects
            tinygltf::Camera cam1, cam2;
            bool cam_equality = (cam1 == cam2);
            (void)cam_equality;
            
            // Create and compare Animation objects
            tinygltf::Animation anim1, anim2;
            bool anim_equality = (anim1 == anim2);
            (void)anim_equality;
            
            // Create and compare Node objects
            tinygltf::Node node1, node2;
            bool node_equality = (node1 == node2);
            (void)node_equality;
            
            // Create and compare Accessor objects
            tinygltf::Accessor acc1, acc2;
            bool acc_equality = (acc1 == acc2);
            (void)acc_equality;
            
            // Create and compare Mesh objects
            tinygltf::Mesh mesh1, mesh2;
            bool mesh_equality = (mesh1 == mesh2);
            (void)mesh_equality;
            
            // Test other equality operators too
            tinygltf::BufferView bv1, bv2;
            bool bv_equality = (bv1 == bv2);
            (void)bv_equality;
            
            tinygltf::Buffer buf1, buf2;
            bool buf_equality = (buf1 == buf2);
            (void)buf_equality;
            
            tinygltf::Image img1, img2;
            bool img_equality = (img1 == img2);
            (void)img_equality;
            
            tinygltf::Sampler samp1, samp2;
            bool samp_equality = (samp1 == samp2);
            (void)samp_equality;
            
            tinygltf::Texture tex1, tex2;
            bool tex_equality = (tex1 == tex2);
            (void)tex_equality;
            
            tinygltf::Scene scene1, scene2;
            bool scene_equality = (scene1 == scene2);
            (void)scene_equality;
            
            tinygltf::Skin skin1, skin2;
            bool skin_equality = (skin1 == skin2);
            (void)skin_equality;
            
            tinygltf::Light light1, light2;
            bool light_equality = (light1 == light2);
            (void)light_equality;
            break;
        }
        
        case 3: {
            // Strategy 4: Test edge cases - empty vectors
            tinygltf::Model empty_model1, empty_model2;
            bool empty_equality = (empty_model1 == empty_model2);
            (void)empty_equality;
            
            // Test with some fields populated
            tinygltf::Model partial_model1, partial_model2;
            
            // Add a material to both
            tinygltf::Material mat;
            partial_model1.materials.push_back(mat);
            partial_model2.materials.push_back(mat);
            
            bool partial_equality = (partial_model1 == partial_model2);
            (void)partial_equality;
            
            // Make them different
            partial_model2.materials.clear();
            bool inequality = (partial_model1 == partial_model2);
            (void)inequality;
            break;
        }
        
        case 4: {
            // Strategy 5: Test recursive equality through nested structures
            // Create a material with PBR properties
            tinygltf::Material mat_with_pbr1, mat_with_pbr2;
            mat_with_pbr1.pbrMetallicRoughness.baseColorFactor = {1.0, 0.0, 0.0, 1.0};
            mat_with_pbr2.pbrMetallicRoughness.baseColorFactor = {1.0, 0.0, 0.0, 1.0};
            
            bool pbr_equality = (mat_with_pbr1 == mat_with_pbr2);
            (void)pbr_equality;
            
            // Make them different
            mat_with_pbr2.pbrMetallicRoughness.baseColorFactor = {0.0, 1.0, 0.0, 1.0};
            bool pbr_inequality = (mat_with_pbr1 == mat_with_pbr2);
            (void)pbr_inequality;
            
            // Test camera with perspective/orthographic properties
            tinygltf::Camera cam_with_props1, cam_with_props2;
            cam_with_props1.type = "perspective";
            cam_with_props2.type = "perspective";
            
            bool cam_props_equality = (cam_with_props1 == cam_with_props2);
            (void)cam_props_equality;
            
            // Test accessor with min/max values
            tinygltf::Accessor acc_with_range1, acc_with_range2;
            acc_with_range1.minValues = {0.0, 0.0, 0.0};
            acc_with_range1.maxValues = {1.0, 1.0, 1.0};
            acc_with_range2.minValues = {0.0, 0.0, 0.0};
            acc_with_range2.maxValues = {1.0, 1.0, 1.0};
            
            bool acc_range_equality = (acc_with_range1 == acc_with_range2);
            (void)acc_range_equality;
            break;
        }
        
        case 5: {
            // Strategy 6: Test equality with loaded model structures
            // Compare individual components from the loaded model
            if (load_success) {
                // Test equality of model vectors (triggers vector::operator==)
                bool accessors_equal = (model1.accessors == model1.accessors);
                bool animations_equal = (model1.animations == model1.animations);
                bool materials_equal = (model1.materials == model1.materials);
                bool meshes_equal = (model1.meshes == model1.meshes);
                bool nodes_equal = (model1.nodes == model1.nodes);
                bool cameras_equal = (model1.cameras == model1.cameras);
                bool textures_equal = (model1.textures == model1.textures);
                bool images_equal = (model1.images == model1.images);
                bool skins_equal = (model1.skins == model1.skins);
                bool samplers_equal = (model1.samplers == model1.samplers);
                bool scenes_equal = (model1.scenes == model1.scenes);
                bool lights_equal = (model1.lights == model1.lights);
                
                (void)accessors_equal;
                (void)animations_equal;
                (void)materials_equal;
                (void)meshes_equal;
                (void)nodes_equal;
                (void)cameras_equal;
                (void)textures_equal;
                (void)images_equal;
                (void)skins_equal;
                (void)samplers_equal;
                (void)scenes_equal;
                (void)lights_equal;
                
                // Test individual structure comparisons if vectors are non-empty
                if (!model1.materials.empty()) {
                    bool first_mat_equality = (model1.materials[0] == model1.materials[0]);
                    (void)first_mat_equality;
                }
                if (!model1.cameras.empty()) {
                    bool first_cam_equality = (model1.cameras[0] == model1.cameras[0]);
                    (void)first_cam_equality;
                }
                if (!model1.animations.empty()) {
                    bool first_anim_equality = (model1.animations[0] == model1.animations[0]);
                    (void)first_anim_equality;
                }
                if (!model1.nodes.empty()) {
                    bool first_node_equality = (model1.nodes[0] == model1.nodes[0]);
                    (void)first_node_equality;
                }
                if (!model1.accessors.empty()) {
                    bool first_acc_equality = (model1.accessors[0] == model1.accessors[0]);
                    (void)first_acc_equality;
                }
                if (!model1.meshes.empty()) {
                    bool first_mesh_equality = (model1.meshes[0] == model1.meshes[0]);
                    (void)first_mesh_equality;
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_004
