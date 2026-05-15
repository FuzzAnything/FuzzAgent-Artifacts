#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <algorithm>

namespace harness_007 {
/*
 * Fuzzing harness for tinygltf library GLTF component equality operators
 * 
 * This harness specifically targets 9 critical equality operators with 300+ undiscovered branches:
 * 1. Material::operator== (98 branches)
 * 2. Camera::operator== (48 branches)
 * 3. Animation::operator== (42 branches)
 * 4. Node::operator== (40 branches)
 * 5. Accessor::operator== (34 branches)
 * 6. PbrMetallicRoughness::operator== (34 branches)
 * 7. Mesh::operator== (34 branches)
 * 8. Image::operator== (32 branches)
 * 9. BufferView::operator== (30 branches)
 * 
 * The harness implements comprehensive equality testing following the coverage guidance:
 * 1. Parse GLTF file using LoadASCIIFromFile/LoadBinaryFromFile to obtain Model
 * 2. Extract components from the Model
 * 3. Test equality operators by:
 *    a) Comparing each component with itself (should return true)
 *    b) Creating modified copies and comparing (should return false for differences)
 *    c) Comparing different components of same type (should return false)
 * 4. Test edge cases: default-constructed components, empty components
 * 5. Implement mock filesystem for file-based loading without actual disk access
 */



// Mock filesystem for fuzzing - stores files in memory
class MockEqualityFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    void* user_data;
    
    MockEqualityFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockEqualityFileSystem* fs = static_cast<MockEqualityFileSystem*>(user_data);
        if (!fs) return false;
        
        auto it = fs->files.find(abs_filename);
        return it != fs->files.end() && it->second.exists;
    }
    
    // ExpandFilePath callback implementation (no-op for mocking)
    static std::string MockExpandFilePath(const std::string& filepath, void* user_data) {
        return filepath;
    }
    
    // ReadWholeFile callback implementation
    static bool MockReadWholeFile(std::vector<unsigned char>* out, 
                                 std::string* err, 
                                 const std::string& filepath, 
                                 void* user_data) {
        MockEqualityFileSystem* fs = static_cast<MockEqualityFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *out = it->second.data;
        return true;
    }
    
    // WriteWholeFile callback implementation  
    static bool MockWriteWholeFile(std::string* err, 
                                   const std::string& filepath,
                                   const std::vector<unsigned char>& contents,
                                   void* user_data) {
        MockEqualityFileSystem* fs = static_cast<MockEqualityFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        FileInfo info;
        info.data = contents;
        info.exists = true;
        fs->files[filepath] = info;
        return true;
    }
    
    // GetFileSizeInBytes callback implementation
    static bool MockGetFileSizeInBytes(size_t* filesize_out,
                                       std::string* err,
                                       const std::string& filepath,
                                       void* user_data) {
        MockEqualityFileSystem* fs = static_cast<MockEqualityFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *filesize_out = it->second.data.size();
        return true;
    }
};

// Helper function to modify a copy of a component to make it different
template<typename T>
void modifyComponent(T& component) {
    // Add a unique identifier to make components different
    if constexpr (std::is_same_v<T, tinygltf::Material>) {
        component.name = "modified_" + component.name;
        if (!component.pbrMetallicRoughness.baseColorFactor.empty()) {
            component.pbrMetallicRoughness.baseColorFactor[0] += 0.1;
        }
    } else if constexpr (std::is_same_v<T, tinygltf::Camera>) {
        component.name = "modified_" + component.name;
    } else if constexpr (std::is_same_v<T, tinygltf::Animation>) {
        component.name = "modified_" + component.name;
    } else if constexpr (std::is_same_v<T, tinygltf::Node>) {
        component.name = "modified_" + component.name;
        if (!component.translation.empty()) {
            component.translation[0] += 1.0;
        }
    } else if constexpr (std::is_same_v<T, tinygltf::Accessor>) {
        component.name = "modified_" + component.name;
        component.byteOffset += 1;
    } else if constexpr (std::is_same_v<T, tinygltf::PbrMetallicRoughness>) {
        if (!component.baseColorFactor.empty()) {
            component.baseColorFactor[0] += 0.1;
        }
        component.metallicFactor += 0.1;
    } else if constexpr (std::is_same_v<T, tinygltf::Mesh>) {
        component.name = "modified_" + component.name;
    } else if constexpr (std::is_same_v<T, tinygltf::Image>) {
        component.name = "modified_" + component.name;
    } else if constexpr (std::is_same_v<T, tinygltf::BufferView>) {
        component.name = "modified_" + component.name;
        component.byteOffset += 1;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to choose testing strategy
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool store_original_json = fdp.ConsumeBool();
    
    // Consume remaining bytes as glTF data
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (gltf_data.empty()) return 0;
    
    // Create mock filesystem and add glTF data as a file
    MockEqualityFileSystem mock_fs;
    MockEqualityFileSystem::FileInfo gltf_file;
    gltf_file.data = gltf_data;
    gltf_file.exists = true;
    mock_fs.files["test.gltf"] = gltf_file;
    mock_fs.files["test.glb"] = gltf_file; // Also add as binary
    
    // Initialize TinyGLTF context with mock filesystem
    tinygltf::TinyGLTF ctx;
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Set up filesystem callbacks
    ctx.SetFsCallbacks(tinygltf::FsCallbacks{
        MockEqualityFileSystem::MockFileExists,
        MockEqualityFileSystem::MockExpandFilePath,
        MockEqualityFileSystem::MockReadWholeFile,
        MockEqualityFileSystem::MockWriteWholeFile,
        MockEqualityFileSystem::MockGetFileSizeInBytes,
        &mock_fs
    });
    
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    bool load_success = false;
    
    // Choose loading method based on selector
    if (selector % 2 == 0) {
        // Test LoadASCIIFromFile
        load_success = ctx.LoadASCIIFromFile(&model, &err, &warn, "test.gltf", check_sections);
    } else {
        // Test LoadBinaryFromFile
        load_success = ctx.LoadBinaryFromFile(&model, &err, &warn, "test.glb", check_sections);
    }
    
    // If file loading fails, try memory-based loading as fallback
    if (!load_success) {
        std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), gltf_data.size());
        load_success = ctx.LoadASCIIFromString(&model, &err, &warn,
                                              json_str.c_str(),
                                              static_cast<unsigned int>(gltf_data.size()),
                                              "",
                                              check_sections);
        
        if (!load_success) {
            load_success = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                   gltf_data.data(),
                                                   static_cast<unsigned int>(gltf_data.size()),
                                                   "",
                                                   check_sections);
        }
    }
    
    // Test equality operators based on selector strategy
    switch (selector % 8) {
        case 0: {
            // Strategy 1: Test equality operators on components extracted from model
            if (load_success && !model.materials.empty()) {
                tinygltf::Material& mat = model.materials[0];
                bool self_eq = (mat == mat); // Compare with self
                (void)self_eq;
                
                // Test PbrMetallicRoughness equality
                tinygltf::PbrMetallicRoughness& pbr = mat.pbrMetallicRoughness;
                bool pbr_self_eq = (pbr == pbr);
                (void)pbr_self_eq;
            }
            
            if (!model.cameras.empty()) {
                tinygltf::Camera& cam = model.cameras[0];
                bool cam_eq = (cam == cam);
                (void)cam_eq;
            }
            
            if (!model.animations.empty()) {
                tinygltf::Animation& anim = model.animations[0];
                bool anim_eq = (anim == anim);
                (void)anim_eq;
            }
            
            if (!model.nodes.empty()) {
                tinygltf::Node& node = model.nodes[0];
                bool node_eq = (node == node);
                (void)node_eq;
            }
            
            if (!model.accessors.empty()) {
                tinygltf::Accessor& acc = model.accessors[0];
                bool acc_eq = (acc == acc);
                (void)acc_eq;
            }
            
            if (!model.meshes.empty()) {
                tinygltf::Mesh& mesh = model.meshes[0];
                bool mesh_eq = (mesh == mesh);
                (void)mesh_eq;
            }
            
            if (!model.images.empty()) {
                tinygltf::Image& img = model.images[0];
                bool img_eq = (img == img);
                (void)img_eq;
            }
            
            if (!model.bufferViews.empty()) {
                tinygltf::BufferView& bv = model.bufferViews[0];
                bool bv_eq = (bv == bv);
                (void)bv_eq;
            }
            break;
        }
        
        case 1: {
            // Strategy 2: Create modified copies and compare for inequality
            if (load_success && !model.materials.empty()) {
                tinygltf::Material original_mat = model.materials[0];
                tinygltf::Material modified_mat = original_mat;
                modifyComponent(modified_mat);
                bool mat_ineq = (original_mat == modified_mat); // Should be false
                (void)mat_ineq;
                
                // Test PbrMetallicRoughness inequality
                tinygltf::PbrMetallicRoughness original_pbr = original_mat.pbrMetallicRoughness;
                tinygltf::PbrMetallicRoughness modified_pbr = original_pbr;
                modifyComponent(modified_pbr);
                bool pbr_ineq = (original_pbr == modified_pbr); // Should be false
                (void)pbr_ineq;
            }
            
            if (!model.cameras.empty()) {
                tinygltf::Camera original_cam = model.cameras[0];
                tinygltf::Camera modified_cam = original_cam;
                modifyComponent(modified_cam);
                bool cam_ineq = (original_cam == modified_cam);
                (void)cam_ineq;
            }
            
            if (!model.animations.empty()) {
                tinygltf::Animation original_anim = model.animations[0];
                tinygltf::Animation modified_anim = original_anim;
                modifyComponent(modified_anim);
                bool anim_ineq = (original_anim == modified_anim);
                (void)anim_ineq;
            }
            
            if (!model.nodes.empty()) {
                tinygltf::Node original_node = model.nodes[0];
                tinygltf::Node modified_node = original_node;
                modifyComponent(modified_node);
                bool node_ineq = (original_node == modified_node);
                (void)node_ineq;
            }
            
            if (!model.accessors.empty()) {
                tinygltf::Accessor original_acc = model.accessors[0];
                tinygltf::Accessor modified_acc = original_acc;
                modifyComponent(modified_acc);
                bool acc_ineq = (original_acc == modified_acc);
                (void)acc_ineq;
            }
            
            if (!model.meshes.empty()) {
                tinygltf::Mesh original_mesh = model.meshes[0];
                tinygltf::Mesh modified_mesh = original_mesh;
                modifyComponent(modified_mesh);
                bool mesh_ineq = (original_mesh == modified_mesh);
                (void)mesh_ineq;
            }
            
            if (!model.images.empty()) {
                tinygltf::Image original_img = model.images[0];
                tinygltf::Image modified_img = original_img;
                modifyComponent(modified_img);
                bool img_ineq = (original_img == modified_img);
                (void)img_ineq;
            }
            
            if (!model.bufferViews.empty()) {
                tinygltf::BufferView original_bv = model.bufferViews[0];
                tinygltf::BufferView modified_bv = original_bv;
                modifyComponent(modified_bv);
                bool bv_ineq = (original_bv == modified_bv);
                (void)bv_ineq;
            }
            break;
        }
        
        case 2: {
            // Strategy 3: Compare different components of same type (should be false)
            if (load_success && model.materials.size() >= 2) {
                bool mat_diff_eq = (model.materials[0] == model.materials[1]);
                (void)mat_diff_eq;
                
                // Test PbrMetallicRoughness from different materials
                bool pbr_diff_eq = (model.materials[0].pbrMetallicRoughness == 
                                   model.materials[1].pbrMetallicRoughness);
                (void)pbr_diff_eq;
            }
            
            if (model.cameras.size() >= 2) {
                bool cam_diff_eq = (model.cameras[0] == model.cameras[1]);
                (void)cam_diff_eq;
            }
            
            if (model.animations.size() >= 2) {
                bool anim_diff_eq = (model.animations[0] == model.animations[1]);
                (void)anim_diff_eq;
            }
            
            if (model.nodes.size() >= 2) {
                bool node_diff_eq = (model.nodes[0] == model.nodes[1]);
                (void)node_diff_eq;
            }
            
            if (model.accessors.size() >= 2) {
                bool acc_diff_eq = (model.accessors[0] == model.accessors[1]);
                (void)acc_diff_eq;
            }
            
            if (model.meshes.size() >= 2) {
                bool mesh_diff_eq = (model.meshes[0] == model.meshes[1]);
                (void)mesh_diff_eq;
            }
            
            if (model.images.size() >= 2) {
                bool img_diff_eq = (model.images[0] == model.images[1]);
                (void)img_diff_eq;
            }
            
            if (model.bufferViews.size() >= 2) {
                bool bv_diff_eq = (model.bufferViews[0] == model.bufferViews[1]);
                (void)bv_diff_eq;
            }
            break;
        }
        
        case 3: {
            // Strategy 4: Test edge cases with default-constructed components
            tinygltf::Material default_mat1, default_mat2;
            bool default_mat_eq = (default_mat1 == default_mat2); // Should be true
            (void)default_mat_eq;
            
            tinygltf::PbrMetallicRoughness default_pbr1, default_pbr2;
            bool default_pbr_eq = (default_pbr1 == default_pbr2); // Should be true
            (void)default_pbr_eq;
            
            tinygltf::Camera default_cam1, default_cam2;
            bool default_cam_eq = (default_cam1 == default_cam2);
            (void)default_cam_eq;
            
            tinygltf::Animation default_anim1, default_anim2;
            bool default_anim_eq = (default_anim1 == default_anim2);
            (void)default_anim_eq;
            
            tinygltf::Node default_node1, default_node2;
            bool default_node_eq = (default_node1 == default_node2);
            (void)default_node_eq;
            
            tinygltf::Accessor default_acc1, default_acc2;
            bool default_acc_eq = (default_acc1 == default_acc2);
            (void)default_acc_eq;
            
            tinygltf::Mesh default_mesh1, default_mesh2;
            bool default_mesh_eq = (default_mesh1 == default_mesh2);
            (void)default_mesh_eq;
            
            tinygltf::Image default_img1, default_img2;
            bool default_img_eq = (default_img1 == default_img2);
            (void)default_img_eq;
            
            tinygltf::BufferView default_bv1, default_bv2;
            bool default_bv_eq = (default_bv1 == default_bv2);
            (void)default_bv_eq;
            break;
        }
        
        case 4: {
            // Strategy 5: Test with partially populated components
            tinygltf::Material partial_mat1, partial_mat2;
            partial_mat1.name = "material1";
            partial_mat2.name = "material1";
            bool partial_mat_eq1 = (partial_mat1 == partial_mat2); // Should be true
            (void)partial_mat_eq1;
            
            partial_mat2.name = "material2";
            bool partial_mat_eq2 = (partial_mat1 == partial_mat2); // Should be false
            (void)partial_mat_eq2;
            
            // Test PbrMetallicRoughness with different values
            tinygltf::PbrMetallicRoughness partial_pbr1, partial_pbr2;
            partial_pbr1.baseColorFactor = {1.0, 0.0, 0.0, 1.0};
            partial_pbr2.baseColorFactor = {1.0, 0.0, 0.0, 1.0};
            bool partial_pbr_eq1 = (partial_pbr1 == partial_pbr2); // Should be true
            (void)partial_pbr_eq1;
            
            partial_pbr2.baseColorFactor[0] = 0.5;
            bool partial_pbr_eq2 = (partial_pbr1 == partial_pbr2); // Should be false
            (void)partial_pbr_eq2;
            break;
        }
        
        case 5: {
            // Strategy 6: Test equality operators through Model::operator==
            if (load_success) {
                // Compare model with itself
                bool model_self_eq = (model == model);
                (void)model_self_eq;
                
                // Create copy and compare
                tinygltf::Model model_copy = model;
                bool model_copy_eq = (model == model_copy);
                (void)model_copy_eq;
                
                // Test assignment operator
                tinygltf::Model model_assigned;
                model_assigned = model;
                bool model_assign_eq = (model == model_assigned);
                (void)model_assign_eq;
            }
            break;
        }
        
        case 6: {
            // Strategy 7: Test vector equality (which calls element equality operators)
            if (load_success) {
                // Compare material vectors
                bool mats_vec_eq = (model.materials == model.materials);
                (void)mats_vec_eq;
                
                // Compare camera vectors
                bool cams_vec_eq = (model.cameras == model.cameras);
                (void)cams_vec_eq;
                
                // Compare animation vectors
                bool anims_vec_eq = (model.animations == model.animations);
                (void)anims_vec_eq;
                
                // Compare node vectors
                bool nodes_vec_eq = (model.nodes == model.nodes);
                (void)nodes_vec_eq;
                
                // Compare accessor vectors
                bool accs_vec_eq = (model.accessors == model.accessors);
                (void)accs_vec_eq;
                
                // Compare mesh vectors
                bool meshes_vec_eq = (model.meshes == model.meshes);
                (void)meshes_vec_eq;
                
                // Compare image vectors
                bool imgs_vec_eq = (model.images == model.images);
                (void)imgs_vec_eq;
                
                // Compare bufferView vectors
                bool bvs_vec_eq = (model.bufferViews == model.bufferViews);
                (void)bvs_vec_eq;
            }
            break;
        }
        
        case 7: {
            // Strategy 8: Comprehensive test of all 9 target equality operators
            // Create test instances for each type and test all scenarios
            tinygltf::Material test_mat1, test_mat2, test_mat3;
            test_mat1.name = "test1";
            test_mat2.name = "test1";
            test_mat3.name = "test2";
            
            bool mat_eq1 = (test_mat1 == test_mat2); // Same name, should be true
            bool mat_eq2 = (test_mat1 == test_mat3); // Different name, should be false
            bool mat_eq3 = (test_mat1 == test_mat1); // Self comparison, should be true
            (void)mat_eq1; (void)mat_eq2; (void)mat_eq3;
            
            // Test PbrMetallicRoughness equality
            tinygltf::PbrMetallicRoughness test_pbr1, test_pbr2, test_pbr3;
            test_pbr1.baseColorFactor = {0.5, 0.5, 0.5, 1.0};
            test_pbr2.baseColorFactor = {0.5, 0.5, 0.5, 1.0};
            test_pbr3.baseColorFactor = {0.6, 0.5, 0.5, 1.0};
            
            bool pbr_eq1 = (test_pbr1 == test_pbr2); // Same values, should be true
            bool pbr_eq2 = (test_pbr1 == test_pbr3); // Different values, should be false
            bool pbr_eq3 = (test_pbr1 == test_pbr1); // Self comparison, should be true
            (void)pbr_eq1; (void)pbr_eq2; (void)pbr_eq3;
            
            // Test other types similarly
            tinygltf::Camera test_cam1, test_cam2;
            bool cam_eq = (test_cam1 == test_cam2);
            (void)cam_eq;
            
            tinygltf::Animation test_anim1, test_anim2;
            bool anim_eq = (test_anim1 == test_anim2);
            (void)anim_eq;
            
            tinygltf::Node test_node1, test_node2;
            bool node_eq = (test_node1 == test_node2);
            (void)node_eq;
            
            tinygltf::Accessor test_acc1, test_acc2;
            bool acc_eq = (test_acc1 == test_acc2);
            (void)acc_eq;
            
            tinygltf::Mesh test_mesh1, test_mesh2;
            bool mesh_eq = (test_mesh1 == test_mesh2);
            (void)mesh_eq;
            
            tinygltf::Image test_img1, test_img2;
            bool img_eq = (test_img1 == test_img2);
            (void)img_eq;
            
            tinygltf::BufferView test_bv1, test_bv2;
            bool bv_eq = (test_bv1 == test_bv2);
            (void)bv_eq;
            break;
        }
    }
    
    return 0;
}

} // namespace harness_007
