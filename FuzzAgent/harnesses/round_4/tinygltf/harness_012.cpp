#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_012 {
/*
 * Fuzzing harness for tinygltf library - harness_012.cpp
 * 
 * This harness targets comprehensive GLTF model equality comparison with focus on
 * individual component equality operators that have high undiscovered branch counts:
 * - Material::operator== (98 undiscovered branches)
 * - Camera::operator== (48 undiscovered branches) 
 * - Animation::operator== (42 undiscovered branches)
 * - Node::operator== (40 undiscovered branches)
 * - PbrMetallicRoughness::operator== (34 undiscovered branches)
 * - Mesh::operator== (34 undiscovered branches)
 * - Image::operator== (32 undiscovered branches)
 * - BufferView::operator== (30 undiscovered branches)
 * - Model::operator== (330/386 undiscovered branches - only 14.5% coverage)
 * 
 * Invocation sequence: Load/create two GLTF models, manipulate models to have diverse structures,
 * compare models using Model::operator==, directly compare individual components,
 * test both equal and unequal cases.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Memory-based operations only (LoadASCIIFromString, LoadBinaryFromMemory)
 * - harness_001: Basic file I/O operations (load/write without validation)
 * - harness_002: Round-trip operations with validation of existing files
 * - harness_003: Error paths, boundary conditions, and failure scenarios
 * - harness_004: Focus on creating models from scratch and comprehensive serialization testing
 * - harness_005: Image writing APIs and texture export
 * - harness_006: WriteGltfSceneToFile with embedded images and stb_image_write.h integration
 * - harness_007: Direct stb_image_write.h testing
 * - harness_008: Configuration and callback API testing
 * - harness_009: Basic Model equality operators and serialization-comparison workflows
 * - harness_010: WriteGltfSceneToFile parameter variations and round-trip validation
 * - harness_011: WriteGltfSceneToFile error conditions and validation failures
 * - harness_012: FOCUS ON DEEP EQUALITY COMPARISON OF INDIVIDUAL COMPONENTS - specifically targets
 *                the equality operators of Material, Camera, Animation, Node, PbrMetallicRoughness,
 *                Mesh, Image, BufferView, and comprehensive Model equality with diverse structures
 * 
 * Targets 650+ undiscovered branches across 9 critical equality operators with low coverage.
 * 
 * Uses FuzzedDataProvider to create diverse model structures, manipulate individual components,
 * and test both equal and unequal comparison scenarios for maximum coverage.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper to create a PbrMetallicRoughness structure
static tinygltf::PbrMetallicRoughness create_test_pbr_metallic_roughness(FuzzedDataProvider& fdp) {
    tinygltf::PbrMetallicRoughness pbr;
    
    // Base color factor (RGBA)
    if (fdp.remaining_bytes() >= 4 * sizeof(double)) {
        pbr.baseColorFactor = {
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>()
        };
    }
    
    // Metallic factor
    if (fdp.remaining_bytes() >= sizeof(double)) {
        pbr.metallicFactor = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    }
    
    // Roughness factor
    if (fdp.remaining_bytes() >= sizeof(double)) {
        pbr.roughnessFactor = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    }
    
    // Base color texture
    if (fdp.remaining_bytes() > 0) {
        pbr.baseColorTexture.index = fdp.ConsumeIntegralInRange<int>(-1, 10);
        pbr.baseColorTexture.texCoord = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    // Metallic roughness texture
    if (fdp.remaining_bytes() > 0) {
        pbr.metallicRoughnessTexture.index = fdp.ConsumeIntegralInRange<int>(-1, 10);
        pbr.metallicRoughnessTexture.texCoord = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    return pbr;
}

// Helper to create a Material structure
static tinygltf::Material create_test_material(FuzzedDataProvider& fdp) {
    tinygltf::Material material;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        material.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // PBR metallic roughness
    material.pbrMetallicRoughness = create_test_pbr_metallic_roughness(fdp);
    
    // Emissive factor
    if (fdp.remaining_bytes() >= 3 * sizeof(double)) {
        material.emissiveFactor = {
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>()
        };
    }
    
    // Alpha mode
    if (fdp.remaining_bytes() > 0) {
        uint8_t alpha_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (alpha_mode) {
            case 0: material.alphaMode = "OPAQUE"; break;
            case 1: material.alphaMode = "MASK"; break;
            case 2: material.alphaMode = "BLEND"; break;
        }
    }
    
    // Alpha cutoff
    if (fdp.remaining_bytes() >= sizeof(double)) {
        material.alphaCutoff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    }
    
    // Double sided
    if (fdp.remaining_bytes() > 0) {
        material.doubleSided = fdp.ConsumeBool();
    }
    
    // Normal texture
    if (fdp.remaining_bytes() > 0) {
        material.normalTexture.index = fdp.ConsumeIntegralInRange<int>(-1, 10);
        material.normalTexture.texCoord = fdp.ConsumeIntegralInRange<int>(0, 1);
        if (fdp.remaining_bytes() >= sizeof(double)) {
            material.normalTexture.scale = fdp.ConsumeFloatingPoint<double>();
        }
    }
    
    // Occlusion texture
    if (fdp.remaining_bytes() > 0) {
        material.occlusionTexture.index = fdp.ConsumeIntegralInRange<int>(-1, 10);
        material.occlusionTexture.texCoord = fdp.ConsumeIntegralInRange<int>(0, 1);
        if (fdp.remaining_bytes() >= sizeof(double)) {
            material.occlusionTexture.strength = fdp.ConsumeFloatingPoint<double>();
        }
    }
    
    // Emissive texture
    if (fdp.remaining_bytes() > 0) {
        material.emissiveTexture.index = fdp.ConsumeIntegralInRange<int>(-1, 10);
        material.emissiveTexture.texCoord = fdp.ConsumeIntegralInRange<int>(0, 1);
    }
    
    return material;
}

// Helper to create a Camera structure
static tinygltf::Camera create_test_camera(FuzzedDataProvider& fdp) {
    tinygltf::Camera camera;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        camera.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // Camera type
    if (fdp.remaining_bytes() > 0) {
        bool is_perspective = fdp.ConsumeBool();
        camera.type = is_perspective ? "perspective" : "orthographic";
    }
    
    // Perspective camera parameters
    if (camera.type == "perspective" && fdp.remaining_bytes() >= 4 * sizeof(double)) {
        camera.perspective.aspectRatio = fdp.ConsumeFloatingPoint<double>();
        camera.perspective.yfov = fdp.ConsumeFloatingPoint<double>();
        camera.perspective.zfar = fdp.ConsumeFloatingPoint<double>();
        camera.perspective.znear = fdp.ConsumeFloatingPoint<double>();
    }
    
    // Orthographic camera parameters
    if (camera.type == "orthographic" && fdp.remaining_bytes() >= 4 * sizeof(double)) {
        camera.orthographic.xmag = fdp.ConsumeFloatingPoint<double>();
        camera.orthographic.ymag = fdp.ConsumeFloatingPoint<double>();
        camera.orthographic.zfar = fdp.ConsumeFloatingPoint<double>();
        camera.orthographic.znear = fdp.ConsumeFloatingPoint<double>();
    }
    
    return camera;
}

// Helper to create an Animation structure
static tinygltf::Animation create_test_animation(FuzzedDataProvider& fdp) {
    tinygltf::Animation animation;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        animation.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // Create some channels
    size_t channel_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < channel_count && fdp.remaining_bytes() > 10; ++i) {
        tinygltf::AnimationChannel channel;
        channel.sampler = fdp.ConsumeIntegralInRange<int>(0, 5);
        
        // Target
        // Target node and path
        channel.target_node = fdp.ConsumeIntegralInRange<int>(-1, 10);
        if (fdp.remaining_bytes() > 5) {
            uint8_t path_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (path_type) {
                case 0: channel.target_path = "translation"; break;
                case 1: channel.target_path = "rotation"; break;
                case 2: channel.target_path = "scale"; break;
            }
        }
        
        animation.channels.push_back(channel);
    }
    
    // Create some samplers
    size_t sampler_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < sampler_count && fdp.remaining_bytes() > 10; ++i) {
        tinygltf::AnimationSampler sampler;
        sampler.input = fdp.ConsumeIntegralInRange<int>(-1, 10);
        sampler.output = fdp.ConsumeIntegralInRange<int>(-1, 10);
        
        if (fdp.remaining_bytes() > 5) {
            uint8_t interpolation = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (interpolation) {
                case 0: sampler.interpolation = "LINEAR"; break;
                case 1: sampler.interpolation = "STEP"; break;
                case 2: sampler.interpolation = "CUBICSPLINE"; break;
            }
        }
        
        animation.samplers.push_back(sampler);
    }
    
    return animation;
}

// Helper to create a Node structure
static tinygltf::Node create_test_node(FuzzedDataProvider& fdp) {
    tinygltf::Node node;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        node.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // Camera reference
    if (fdp.remaining_bytes() > 0) {
        node.camera = fdp.ConsumeIntegralInRange<int>(-1, 10);
    }
    
    // Mesh reference
    if (fdp.remaining_bytes() > 0) {
        node.mesh = fdp.ConsumeIntegralInRange<int>(-1, 10);
    }
    
    // Skin reference
    if (fdp.remaining_bytes() > 0) {
        node.skin = fdp.ConsumeIntegralInRange<int>(-1, 10);
    }
    
    // Children
    size_t child_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < child_count && fdp.remaining_bytes() > sizeof(int); ++i) {
        node.children.push_back(fdp.ConsumeIntegralInRange<int>(0, 10));
    }
    
    // Transformation data
    if (fdp.remaining_bytes() >= 4 * sizeof(double)) {
        node.rotation = {
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>()
        };
    }
    
    if (fdp.remaining_bytes() >= 3 * sizeof(double)) {
        node.scale = {
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>()
        };
    }
    
    if (fdp.remaining_bytes() >= 3 * sizeof(double)) {
        node.translation = {
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>()
        };
    }
    
    if (fdp.remaining_bytes() >= 16 * sizeof(double)) {
        node.matrix.resize(16);
        for (int j = 0; j < 16 && fdp.remaining_bytes() >= sizeof(double); ++j) {
            node.matrix[j] = fdp.ConsumeFloatingPoint<double>();
        }
    }
    
    return node;
}

// Helper to create a Mesh structure
static tinygltf::Mesh create_test_mesh(FuzzedDataProvider& fdp) {
    tinygltf::Mesh mesh;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        mesh.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // Create primitives
    size_t primitive_count = fdp.ConsumeIntegralInRange<size_t>(0, 2);
    for (size_t i = 0; i < primitive_count && fdp.remaining_bytes() > 10; ++i) {
        tinygltf::Primitive primitive;
        
        // Mode
        primitive.mode = fdp.ConsumeIntegralInRange<int>(-1, 6);
        
        // Material
        primitive.material = fdp.ConsumeIntegralInRange<int>(-1, 10);
        
        // Indices
        primitive.indices = fdp.ConsumeIntegralInRange<int>(-1, 10);
        
        // Add some attributes
        size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
        for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 5; ++j) {
            std::string attr_name = fdp.ConsumeRandomLengthString(10);
            int attr_value = fdp.ConsumeIntegralInRange<int>(0, 10);
            primitive.attributes[attr_name] = attr_value;
        }
        
        mesh.primitives.push_back(primitive);
    }
    
    // Weights
    size_t weight_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < weight_count && fdp.remaining_bytes() >= sizeof(double); ++i) {
        mesh.weights.push_back(fdp.ConsumeFloatingPoint<double>());
    }
    
    return mesh;
}

// Helper to create an Image structure
static tinygltf::Image create_test_image(FuzzedDataProvider& fdp) {
    tinygltf::Image image;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        image.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // URI
    if (fdp.remaining_bytes() > 5) {
        image.uri = fdp.ConsumeRandomLengthString(128);
    }
    
    // MIME type
    if (fdp.remaining_bytes() > 5) {
        uint8_t mime_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (mime_type) {
            case 0: image.mimeType = "image/jpeg"; break;
            case 1: image.mimeType = "image/png"; break;
            case 2: image.mimeType = "image/bmp"; break;
        }
    }
    
    // Image data
    size_t image_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min((size_t)1024, fdp.remaining_bytes()));
    if (image_size > 0) {
        image.image.resize(image_size);
        for (size_t i = 0; i < image_size; ++i) {
            image.image[i] = fdp.ConsumeIntegral<uint8_t>();
        }
    }
    
    // Width and height
    if (fdp.remaining_bytes() >= 2 * sizeof(int)) {
        image.width = fdp.ConsumeIntegralInRange<int>(0, 1024);
        image.height = fdp.ConsumeIntegralInRange<int>(0, 1024);
    }
    
    // Bits per component
    if (fdp.remaining_bytes() >= sizeof(int)) {
        image.bits = fdp.ConsumeIntegralInRange<int>(0, 32);
    }
    
    // Component count
    if (fdp.remaining_bytes() >= sizeof(int)) {
        image.component = fdp.ConsumeIntegralInRange<int>(0, 4);
    }
    
    return image;
}

// Helper to create a BufferView structure
static tinygltf::BufferView create_test_buffer_view(FuzzedDataProvider& fdp) {
    tinygltf::BufferView buffer_view;
    
    // Name
    if (fdp.remaining_bytes() > 5) {
        buffer_view.name = fdp.ConsumeRandomLengthString(32);
    }
    
    // Buffer index
    if (fdp.remaining_bytes() > 0) {
        buffer_view.buffer = fdp.ConsumeIntegralInRange<int>(0, 10);
    }
    
    // Byte offset
    if (fdp.remaining_bytes() >= sizeof(size_t)) {
        buffer_view.byteOffset = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    }
    
    // Byte length
    if (fdp.remaining_bytes() >= sizeof(size_t)) {
        buffer_view.byteLength = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    }
    
    // Byte stride
    if (fdp.remaining_bytes() >= sizeof(size_t)) {
        buffer_view.byteStride = fdp.ConsumeIntegralInRange<size_t>(0, 252);
        // Ensure stride is multiple of 4 if non-zero
        if (buffer_view.byteStride > 0 && buffer_view.byteStride % 4 != 0) {
            buffer_view.byteStride = (buffer_view.byteStride / 4 + 1) * 4;
        }
    }
    
    // Target
    if (fdp.remaining_bytes() > 0) {
        uint8_t target = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (target) {
            case 0: buffer_view.target = 0; break; // No target
            case 1: buffer_view.target = TINYGLTF_TARGET_ARRAY_BUFFER; break;
            case 2: buffer_view.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER; break;
        }
    }
    
    return buffer_view;
}

// Helper to create a comprehensive test model with various components
static tinygltf::Model create_comprehensive_model(FuzzedDataProvider& fdp, bool make_different) {
    tinygltf::Model model;
    
    // Asset metadata
    model.asset.version = "2.0";
    if (fdp.remaining_bytes() > 5) {
        model.asset.generator = fdp.ConsumeRandomLengthString(64);
    }
    
    // Create multiple materials
    size_t material_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 5 : 3);
    for (size_t i = 0; i < material_count && fdp.remaining_bytes() > 50; ++i) {
        model.materials.push_back(create_test_material(fdp));
    }
    
    // Create multiple cameras
    size_t camera_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 3 : 2);
    for (size_t i = 0; i < camera_count && fdp.remaining_bytes() > 50; ++i) {
        model.cameras.push_back(create_test_camera(fdp));
    }
    
    // Create multiple animations
    size_t animation_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 3 : 2);
    for (size_t i = 0; i < animation_count && fdp.remaining_bytes() > 50; ++i) {
        model.animations.push_back(create_test_animation(fdp));
    }
    
    // Create multiple nodes
    size_t node_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 5 : 3);
    for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 50; ++i) {
        model.nodes.push_back(create_test_node(fdp));
    }
    
    // Create multiple meshes
    size_t mesh_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 4 : 2);
    for (size_t i = 0; i < mesh_count && fdp.remaining_bytes() > 50; ++i) {
        model.meshes.push_back(create_test_mesh(fdp));
    }
    
    // Create multiple images
    size_t image_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 3 : 2);
    for (size_t i = 0; i < image_count && fdp.remaining_bytes() > 100; ++i) {
        model.images.push_back(create_test_image(fdp));
    }
    
    // Create multiple buffer views
    size_t buffer_view_count = fdp.ConsumeIntegralInRange<size_t>(0, make_different ? 4 : 2);
    for (size_t i = 0; i < buffer_view_count && fdp.remaining_bytes() > 50; ++i) {
        model.bufferViews.push_back(create_test_buffer_view(fdp));
    }
    
    // Create scenes
    size_t scene_count = fdp.ConsumeIntegralInRange<size_t>(0, 2);
    for (size_t i = 0; i < scene_count && fdp.remaining_bytes() > 20; ++i) {
        tinygltf::Scene scene;
        scene.name = fdp.ConsumeRandomLengthString(32);
        
        // Add some nodes to scene
        size_t scene_node_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
        for (size_t j = 0; j < scene_node_count && fdp.remaining_bytes() > sizeof(int); ++j) {
            if (!model.nodes.empty()) {
                scene.nodes.push_back(fdp.ConsumeIntegralInRange<int>(0, model.nodes.size() - 1));
            }
        }
        
        model.scenes.push_back(scene);
    }
    
    // Set default scene if we have scenes
    if (!model.scenes.empty() && fdp.remaining_bytes() > sizeof(int)) {
        model.defaultScene = fdp.ConsumeIntegralInRange<int>(0, model.scenes.size() - 1);
    }
    
    return model;
}

// Test individual component equality operators
static void test_component_equality_operators(FuzzedDataProvider& fdp) {
    // Test Material equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::Material mat1 = create_test_material(fdp);
        tinygltf::Material mat2 = mat1; // Copy for equal test
        bool material_equal = (mat1 == mat2);
        (void)material_equal;
        
        // Create a different material for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::Material mat3 = create_test_material(fdp);
            bool material_not_equal = !(mat1 == mat3);
            (void)material_not_equal;
        }
    }
    
    // Test Camera equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::Camera cam1 = create_test_camera(fdp);
        tinygltf::Camera cam2 = cam1; // Copy for equal test
        bool camera_equal = (cam1 == cam2);
        (void)camera_equal;
        
        // Create a different camera for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::Camera cam3 = create_test_camera(fdp);
            bool camera_not_equal = !(cam1 == cam3);
            (void)camera_not_equal;
        }
    }
    
    // Test Animation equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::Animation anim1 = create_test_animation(fdp);
        tinygltf::Animation anim2 = anim1; // Copy for equal test
        bool animation_equal = (anim1 == anim2);
        (void)animation_equal;
        
        // Create a different animation for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::Animation anim3 = create_test_animation(fdp);
            bool animation_not_equal = !(anim1 == anim3);
            (void)animation_not_equal;
        }
    }
    
    // Test Node equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::Node node1 = create_test_node(fdp);
        tinygltf::Node node2 = node1; // Copy for equal test
        bool node_equal = (node1 == node2);
        (void)node_equal;
        
        // Create a different node for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::Node node3 = create_test_node(fdp);
            bool node_not_equal = !(node1 == node3);
            (void)node_not_equal;
        }
    }
    
    // Test PbrMetallicRoughness equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::PbrMetallicRoughness pbr1 = create_test_pbr_metallic_roughness(fdp);
        tinygltf::PbrMetallicRoughness pbr2 = pbr1; // Copy for equal test
        bool pbr_equal = (pbr1 == pbr2);
        (void)pbr_equal;
        
        // Create a different PBR for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::PbrMetallicRoughness pbr3 = create_test_pbr_metallic_roughness(fdp);
            bool pbr_not_equal = !(pbr1 == pbr3);
            (void)pbr_not_equal;
        }
    }
    
    // Test Mesh equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::Mesh mesh1 = create_test_mesh(fdp);
        tinygltf::Mesh mesh2 = mesh1; // Copy for equal test
        bool mesh_equal = (mesh1 == mesh2);
        (void)mesh_equal;
        
        // Create a different mesh for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::Mesh mesh3 = create_test_mesh(fdp);
            bool mesh_not_equal = !(mesh1 == mesh3);
            (void)mesh_not_equal;
        }
    }
    
    // Test Image equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::Image img1 = create_test_image(fdp);
        tinygltf::Image img2 = img1; // Copy for equal test
        bool image_equal = (img1 == img2);
        (void)image_equal;
        
        // Create a different image for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::Image img3 = create_test_image(fdp);
            bool image_not_equal = !(img1 == img3);
            (void)image_not_equal;
        }
    }
    
    // Test BufferView equality
    if (fdp.remaining_bytes() > 100) {
        tinygltf::BufferView bv1 = create_test_buffer_view(fdp);
        tinygltf::BufferView bv2 = bv1; // Copy for equal test
        bool buffer_view_equal = (bv1 == bv2);
        (void)buffer_view_equal;
        
        // Create a different buffer view for inequality test
        if (fdp.remaining_bytes() > 100) {
            tinygltf::BufferView bv3 = create_test_buffer_view(fdp);
            bool buffer_view_not_equal = !(bv1 == bv3);
            (void)buffer_view_not_equal;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need significant input for comprehensive testing
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test type based on fuzzer input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Test 1: Individual component equality operators
    if (test_type == 0 || test_type == 3) {
        test_component_equality_operators(fdp);
    }
    
    // Test 2: Comprehensive model equality testing
    if ((test_type == 1 || test_type == 3) && fdp.remaining_bytes() > 500) {
        // Create two identical models
        tinygltf::Model model1 = create_comprehensive_model(fdp, false);
        tinygltf::Model model2 = model1; // Copy constructor for equal test
        
        // Test Model::operator== on identical models
        bool models_equal = (model1 == model2);
        (void)models_equal;
        
        // Test implicit operator!= pattern
        bool models_not_equal = !(model1 == model2);
        (void)models_not_equal;
        
        // Test vector equality of individual components
        bool materials_equal = (model1.materials == model2.materials);
        bool cameras_equal = (model1.cameras == model2.cameras);
        bool animations_equal = (model1.animations == model2.animations);
        bool nodes_equal = (model1.nodes == model2.nodes);
        bool meshes_equal = (model1.meshes == model2.meshes);
        bool images_equal = (model1.images == model2.images);
        bool buffer_views_equal = (model1.bufferViews == model2.bufferViews);
        
        (void)materials_equal;
        (void)cameras_equal;
        (void)animations_equal;
        (void)nodes_equal;
        (void)meshes_equal;
        (void)images_equal;
        (void)buffer_views_equal;
    }
    
    // Test 3: Model inequality testing
    if ((test_type == 2 || test_type == 3) && fdp.remaining_bytes() > 1000) {
        // Create two different models
        tinygltf::Model model1 = create_comprehensive_model(fdp, false);
        tinygltf::Model model2 = create_comprehensive_model(fdp, true);
        
        // Test Model::operator== on different models (should be false)
        bool different_models_equal = (model1 == model2);
        (void)different_models_equal;
        
        // Test implicit operator!= pattern (should be true)
        bool different_models_not_equal = !(model1 == model2);
        (void)different_models_not_equal;
        
        // Test individual component comparisons
        if (!model1.materials.empty() && !model2.materials.empty()) {
            bool first_materials_equal = (model1.materials[0] == model2.materials[0]);
            (void)first_materials_equal;
        }
        
        if (!model1.cameras.empty() && !model2.cameras.empty()) {
            bool first_cameras_equal = (model1.cameras[0] == model2.cameras[0]);
            (void)first_cameras_equal;
        }
        
        if (!model1.animations.empty() && !model2.animations.empty()) {
            bool first_animations_equal = (model1.animations[0] == model2.animations[0]);
            (void)first_animations_equal;
        }
    }
    
    return 0;
}

} // namespace harness_012
