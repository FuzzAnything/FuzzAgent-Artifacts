#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_016 {
/*
 * Fuzzing harness for tiny_gltf.h targeting Model comparison operations and Value accessor functions.
 * Specifically targets the largest remaining API coverage gap:
 * - Model::operator== (161 undiscovered branches)
 * - Material::operator== (41 undiscovered branches)
 * - Node::operator== (16 undiscovered branches)
 * - Animation::operator== (15 undiscovered branches)
 * - PbrMetallicRoughness::operator== (14 undiscovered branches)
 * - Mesh::operator== (11 undiscovered branches)
 * - Value::Get<int> (0% coverage)
 * - Value::Get<double> (0% coverage)
 * - Value::Get<bool> (0% coverage)
 *
 * Required Helper APIs:
 * - LoadASCIIFromFile
 * - LoadBinaryFromFile
 * - LoadBinaryFromMemory
 *
 * This harness implements a comprehensive comparison testing workflow:
 * 1) Create TinyGLTF instance
 * 2) Load two GLTF models using LoadASCIIFromFile/LoadBinaryFromFile
 * 3) Optionally modify Model objects for different comparison scenarios
 * 4) Call Model comparison operator (operator==) to exercise all nested comparisons
 * 5) Test individual component comparisons (Material, Node, Mesh, Animation, etc.)
 * 6) Test Value accessor functions on Model components (Get<int>, Get<double>, Get<bool>)
 * 7) Clean up Model objects
 *
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp tests basic loading without comparison operations
 * - harness_001.cpp tests file loading with minimal operations
 * - harness_015.cpp tests loading with callback configuration
 * - This harness specifically focuses on comparison operators and Value accessors
 *   to cover the 161+ undiscovered branches in Model comparison alone
 *
 * Uses FuzzedDataProvider for structured input splitting to generate:
 * - Loading method selection (ASCII from file, Binary from file/memory)
 * - Model modification scenarios (identical, different, partially modified)
 * - Comparison type selection (full model, individual components)
 * - Value accessor testing scenarios
 * - Input for model data (GLTF/GLB content)
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// ============================================================================
// Utility Functions
// ============================================================================

// Create a temporary file with given content
static std::string create_temp_file(const std::vector<uint8_t>& content, const std::string& extension) {
    char tmpname[] = "/tmp/tinygltf_XXXXXX";
    int fd = mkstemp(tmpname);
    if (fd < 0) {
        return "";
    }
    
    // Write content
    if (!content.empty()) {
        write(fd, content.data(), content.size());
    }
    
    close(fd);
    
    // Add extension if requested
    if (!extension.empty()) {
        std::string new_name = std::string(tmpname) + extension;
        rename(tmpname, new_name.c_str());
        return new_name;
    }
    
    return std::string(tmpname);
}

// Clean up temporary file
static void cleanup_temp_file(const std::string& filename) {
    if (!filename.empty()) {
        unlink(filename.c_str());
    }
}

// ============================================================================
// Model Modification Functions
// ============================================================================

// Modify a Model object to create different comparison scenarios
static void modify_model_for_comparison(tinygltf::Model& model, uint8_t modification_type) {
    switch (modification_type % 5) {
        case 0:
            // Case 0: No modification (identical models)
            break;
            
        case 1:
            // Case 1: Modify materials
            if (!model.materials.empty()) {
                // Change name of first material
                model.materials[0].name = "ModifiedMaterial";
                // Modify PBR values
                model.materials[0].pbrMetallicRoughness.baseColorFactor[0] = 0.5;
                model.materials[0].pbrMetallicRoughness.metallicFactor = 0.7;
            } else {
                // Add a material if none exist
                tinygltf::Material material;
                material.name = "AddedMaterial";
                model.materials.push_back(material);
            }
            break;
            
        case 2:
            // Case 2: Modify nodes
            if (!model.nodes.empty()) {
                model.nodes[0].name = "ModifiedNode";
                if (model.nodes[0].translation.size() >= 3) {
                    model.nodes[0].translation[0] = 10.0;
                    model.nodes[0].translation[1] = 20.0;
                    model.nodes[0].translation[2] = 30.0;
                }
            } else {
                // Add a node if none exist
                tinygltf::Node node;
                node.name = "AddedNode";
                model.nodes.push_back(node);
            }
            break;
            
        case 3:
            // Case 3: Modify animations
            if (!model.animations.empty()) {
                model.animations[0].name = "ModifiedAnimation";
            } else {
                // Add an animation if none exist
                tinygltf::Animation animation;
                animation.name = "AddedAnimation";
                model.animations.push_back(animation);
            }
            break;
            
        case 4:
            // Case 4: Modify meshes
            if (!model.meshes.empty()) {
                model.meshes[0].name = "ModifiedMesh";
            } else {
                // Add a mesh if none exist
                tinygltf::Mesh mesh;
                mesh.name = "AddedMesh";
                model.meshes.push_back(mesh);
            }
            break;
    }
}

// ============================================================================
// Value Accessor Testing Functions
// ============================================================================

// Test Value::Get<int>() on model components
static void test_value_get_int(const tinygltf::Model& model) {
    // Test on extras if present
    if (model.extras.Type() != tinygltf::NULL_TYPE && model.extras.IsObject()) {
        // Try to get integer values from extras
        auto keys = model.extras.Keys();
        for (const auto& key : keys) {
            if (model.extras.Has(key) && model.extras.Get(key).IsInt()) {
                try {
                    int value = model.extras.Get(key).Get<int>();
                    // Use value to avoid unused variable warning
                    (void)value;
                } catch (...) {
                    // Ignore exceptions - some Values might not be convertible
                }
            }
        }
    }
    
    // Test on materials extras
    for (const auto& material : model.materials) {
        if (material.extras.Type() != tinygltf::NULL_TYPE && material.extras.IsObject()) {
            auto keys = material.extras.Keys();
            for (const auto& key : keys) {
                if (material.extras.Has(key) && material.extras.Get(key).IsInt()) {
                    try {
                        int value = material.extras.Get(key).Get<int>();
                        (void)value;
                    } catch (...) {
                        // Ignore exceptions
                    }
                }
            }
        }
    }
}

// Test Value::Get<double>() on model components
static void test_value_get_double(const tinygltf::Model& model) {
    // Test on extras if present
    if (model.extras.Type() != tinygltf::NULL_TYPE && model.extras.IsObject()) {
        auto keys = model.extras.Keys();
        for (const auto& key : keys) {
            if (model.extras.Has(key) && model.extras.Get(key).IsNumber()) {
                try {
                    double value = model.extras.Get(key).Get<double>();
                    (void)value;
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
    }
    
    // Test on materials extras (which are Value objects)
    for (const auto& material : model.materials) {
        if (material.extras.Type() != tinygltf::NULL_TYPE && material.extras.IsObject()) {
            auto keys = material.extras.Keys();
            for (const auto& key : keys) {
                if (material.extras.Has(key) && material.extras.Get(key).IsNumber()) {
                    try {
                        double value = material.extras.Get(key).Get<double>();
                        (void)value;
                    } catch (...) {
                        // Ignore exceptions
                    }
                }
            }
        }
    }
}

// Test Value::Get<bool>() on model components
static void test_value_get_bool(const tinygltf::Model& model) {
    // Test on extras if present
    if (model.extras.Type() != tinygltf::NULL_TYPE && model.extras.IsObject()) {
        auto keys = model.extras.Keys();
        for (const auto& key : keys) {
            if (model.extras.Has(key) && model.extras.Get(key).IsBool()) {
                try {
                    bool value = model.extras.Get(key).Get<bool>();
                    (void)value;
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
    }
    // Test on materials extras (which are Value objects)
    for (const auto& material : model.materials) {
        if (material.extras.Type() != tinygltf::NULL_TYPE && material.extras.IsObject()) {
            auto keys = material.extras.Keys();
            for (const auto& key : keys) {
                if (material.extras.Has(key) && material.extras.Get(key).IsBool()) {
                    try {
                        bool value = material.extras.Get(key).Get<bool>();
                        (void)value;
                    } catch (...) {
                        // Ignore exceptions
                    }
                }
            }
        }
    }
}

// ============================================================================
// Main Fuzzing Function
// ============================================================================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    const size_t MIN_SIZE = sizeof(uint8_t) * 4 + 64; // Enough for choices and some model data
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ========================================================================
    // Phase 1: Consume Input for Configuration
    // ========================================================================
    
    // Consume configuration choices
    uint8_t loading_method = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=ASCII file, 1=Binary file, 2=Binary memory
    uint8_t modification_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    uint8_t comparison_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_value_accessors = fdp.ConsumeBool();
    
    // Consume model data for temporary files
    size_t model_data_size = fdp.ConsumeIntegralInRange<size_t>(64, std::min(size / 2, static_cast<size_t>(1024)));
    std::vector<uint8_t> model_data = fdp.ConsumeBytes<uint8_t>(model_data_size);
    
    // Consume remaining data for second model (if different)
    std::vector<uint8_t> second_model_data;
    if (comparison_type == 1 || comparison_type == 3) {
        // For different models or modified models, use remaining data
        second_model_data = fdp.ConsumeRemainingBytes<uint8_t>();
    } else {
        // For identical models, use same data
        second_model_data = model_data;
    }
    
    // ========================================================================
    // Phase 2: Create Temporary Files (if needed for file loading)
    // ========================================================================
    
    std::string temp_file1, temp_file2;
    std::string extension;
    
    switch (loading_method) {
        case 0: // ASCII file (.gltf)
            extension = ".gltf";
            break;
        case 1: // Binary file (.glb)
        case 2: // Binary memory
            extension = ".glb";
            break;
    }
    
    if (loading_method == 0 || loading_method == 1) {
        // File loading methods need temporary files
        temp_file1 = create_temp_file(model_data, extension);
        if (temp_file1.empty()) {
            return 0; // Failed to create temp file
        }
        
        if (!second_model_data.empty() && (comparison_type == 1 || comparison_type == 3)) {
            temp_file2 = create_temp_file(second_model_data, extension);
            if (temp_file2.empty()) {
                cleanup_temp_file(temp_file1);
                return 0;
            }
        } else {
            temp_file2 = temp_file1; // Same file for identical models
        }
    }
    
    // ========================================================================
    // Phase 3: Initialize TinyGLTF and Load Models
    // ========================================================================
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF loader;
    std::string err1, err2, warn1, warn2;
    bool load_success1 = false, load_success2 = false;
    
    switch (loading_method) {
        case 0: // LoadASCIIFromFile
            if (!temp_file1.empty()) {
                load_success1 = loader.LoadASCIIFromFile(&model1, &err1, &warn1, temp_file1);
            }
            if (!temp_file2.empty()) {
                load_success2 = loader.LoadASCIIFromFile(&model2, &err2, &warn2, temp_file2);
            }
            break;
            
        case 1: // LoadBinaryFromFile
            if (!temp_file1.empty()) {
                load_success1 = loader.LoadBinaryFromFile(&model1, &err1, &warn1, temp_file1);
            }
            if (!temp_file2.empty()) {
                load_success2 = loader.LoadBinaryFromFile(&model2, &err2, &warn2, temp_file2);
            }
            break;
            
        case 2: // LoadBinaryFromMemory
            load_success1 = loader.LoadBinaryFromMemory(&model1, &err1, &warn1,
                                                       model_data.data(),
                                                       static_cast<unsigned int>(model_data.size()),
                                                       "");
            if (!second_model_data.empty()) {
                load_success2 = loader.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                           second_model_data.data(),
                                                           static_cast<unsigned int>(second_model_data.size()),
                                                           "");
            } else {
                model2 = model1; // Copy for identical models
                load_success2 = load_success1;
            }
            break;
    }
    
    // ========================================================================
    // Phase 4: Modify Models for Comparison Testing
    // ========================================================================
    
    // Apply modifications based on comparison type
    switch (comparison_type) {
        case 0: // Identical models
            // No modifications needed
            break;
            
        case 1: // Different models (loaded from different data)
            // Already different due to different input data
            break;
            
        case 2: // Modified copy (same base, one modified)
            if (load_success1) {
                model2 = model1; // Copy
                modify_model_for_comparison(model2, modification_type);
            }
            break;
            
        case 3: // Both modified differently
            if (load_success1 && load_success2) {
                modify_model_for_comparison(model1, modification_type);
                modify_model_for_comparison(model2, (modification_type + 1) % 5);
            }
            break;
    }
    
    // ========================================================================
    // Phase 5: Execute Comparison Operations
    // ========================================================================
    
    // Test Model::operator== (primary target - 161 undiscovered branches)
    if (load_success1 && load_success2) {
        bool models_equal = (model1 == model2);
        (void)models_equal; // Use result to avoid unused variable warning
    }
    
    // Test individual component comparison operators
    if (load_success1 && load_success2) {
        // Material::operator== (41 undiscovered branches)
        if (!model1.materials.empty() && !model2.materials.empty()) {
            bool materials_equal = (model1.materials[0] == model2.materials[0]);
            (void)materials_equal;
        }
        
        // Node::operator== (16 undiscovered branches)
        if (!model1.nodes.empty() && !model2.nodes.empty()) {
            bool nodes_equal = (model1.nodes[0] == model2.nodes[0]);
            (void)nodes_equal;
        }
        
        // Animation::operator== (15 undiscovered branches)
        if (!model1.animations.empty() && !model2.animations.empty()) {
            bool animations_equal = (model1.animations[0] == model2.animations[0]);
            (void)animations_equal;
        }
        
        // PbrMetallicRoughness::operator== (14 undiscovered branches)
        if (!model1.materials.empty() && !model2.materials.empty()) {
            bool pbr_equal = (model1.materials[0].pbrMetallicRoughness == 
                            model2.materials[0].pbrMetallicRoughness);
            (void)pbr_equal;
        }
        
        // Mesh::operator== (11 undiscovered branches)
        if (!model1.meshes.empty() && !model2.meshes.empty()) {
            bool meshes_equal = (model1.meshes[0] == model2.meshes[0]);
            (void)meshes_equal;
        }
    }
    
    // ========================================================================
    // Phase 6: Test Value Accessor Functions (0% coverage targets)
    // ========================================================================
    
    if (test_value_accessors && load_success1) {
        test_value_get_int(model1);
        test_value_get_double(model1);
        test_value_get_bool(model1);
    }
    
    if (test_value_accessors && load_success2) {
        test_value_get_int(model2);
        test_value_get_double(model2);
        test_value_get_bool(model2);
    }
    
    // ========================================================================
    // Phase 7: Cleanup
    // ========================================================================
    
    // Clean up temporary files if created
    if (!temp_file1.empty()) {
        cleanup_temp_file(temp_file1);
    }
    if (!temp_file2.empty() && temp_file2 != temp_file1) {
        cleanup_temp_file(temp_file2);
    }
    
    // Model and loader objects are automatically destroyed when they go out of scope
    
    return 0;
}

} // namespace harness_016
