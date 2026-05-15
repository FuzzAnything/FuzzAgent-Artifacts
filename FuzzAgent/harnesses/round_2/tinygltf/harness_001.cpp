#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_001 {
// Harness 001: Extended glTF fuzzer for tinygltf
// Targets additional API functions: file-based loading, serialization, and deeper model traversal
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and tinygltf
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 3) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector byte to choose different fuzzing paths
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Create model and context
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Path 0: File-based loading simulation (ASCII)
    if ((selector & 0x07) == 0) {
        // Consume a filename from input
        std::string filename = fdp.ConsumeRandomLengthString(64);
        
        // Use remaining bytes as JSON content
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            // Simulate file loading by using LoadASCIIFromString with base_dir
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "/tmp/");
            (void)ret;
            
            // Deep traversal: access various model components
            for (const auto& accessor : model.accessors) {
                (void)accessor.name;
                (void)accessor.componentType;
                (void)accessor.type;
                (void)accessor.count;
            }
            
            for (const auto& mesh : model.meshes) {
                (void)mesh.name;
                for (const auto& primitive : mesh.primitives) {
                    (void)primitive.mode;
                    for (const auto& attr : primitive.attributes) {
                        (void)attr.first;
                        (void)attr.second;
                    }
                }
            }
        }
    }
    // Path 1: File-based loading simulation (Binary/GLB)
    else if ((selector & 0x07) == 1) {
        // Consume a filename from input
        std::string filename = fdp.ConsumeRandomLengthString(64);
        
        // Use remaining bytes as binary GLB data
        std::vector<uint8_t> binary_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!binary_data.empty()) {
            bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                binary_data.data(),
                                                static_cast<unsigned int>(binary_data.size()),
                                                /* base_dir */ "/tmp/");
            (void)ret;
            
            // Deep traversal: access buffers and images
            for (const auto& buffer : model.buffers) {
                (void)buffer.name;
                (void)buffer.data.size();
            }
            
            for (const auto& image : model.images) {
                (void)image.name;
                (void)image.width;
                (void)image.height;
                (void)image.component;
                (void)image.bits;
            }
        }
    }
    // Path 2: Serialization test (WriteGltfSceneToStream)
    else if ((selector & 0x07) == 2) {
        // First, try to parse some input
        std::string json_str = fdp.ConsumeRandomLengthString(fdp.remaining_bytes() / 2);
        
        if (!json_str.empty()) {
            bool parse_ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                                    json_str.c_str(), 
                                                    static_cast<unsigned int>(json_str.size()),
                                                    /* base_dir */ "");
            
            // Only attempt serialization if parsing succeeded (or even if it failed, test error paths)
            std::ostringstream stream;
            bool write_ret = ctx.WriteGltfSceneToStream(&model, stream, 
                                                       fdp.ConsumeBool() /* prettyPrint */, 
                                                       fdp.ConsumeBool() /* writeBinary */);
            (void)write_ret;
            
            // Access the stream content
            std::string serialized = stream.str();
            (void)serialized.size();
        }
    }
    // Path 3: Test with different Set methods
    else if ((selector & 0x07) == 3) {
        // Configure the context with fuzzed settings
        ctx.SetImageLoader(nullptr, nullptr);  // Use default loader
        ctx.SetImageWriter(nullptr, nullptr);  // Use default writer
        ctx.SetStoreOriginalJSONForExtrasAndExtensions(fdp.ConsumeBool());
        
        // Try loading with configured context
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "");
            (void)ret;
            
            // Check if extras JSON strings were stored
            (void)model.extras_json_string;
            for (const auto& node : model.nodes) {
                (void)node.extras_json_string;
            }
        }
    }
    // Path 4: Test material and texture traversal
    else if ((selector & 0x07) == 4) {
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "");
            (void)ret;
            
            // Traverse materials and textures
            for (const auto& material : model.materials) {
                (void)material.name;
                (void)material.pbrMetallicRoughness.baseColorFactor;
                (void)material.pbrMetallicRoughness.metallicFactor;
                (void)material.pbrMetallicRoughness.roughnessFactor;
                (void)material.emissiveFactor;
                (void)material.alphaMode;
                (void)material.alphaCutoff;
                (void)material.doubleSided;
            }
            
            for (const auto& texture : model.textures) {
                (void)texture.name;
                (void)texture.sampler;
                (void)texture.source;
            }
            
            for (const auto& sampler : model.samplers) {
                (void)sampler.name;
                (void)sampler.minFilter;
                (void)sampler.magFilter;
                (void)sampler.wrapS;
                (void)sampler.wrapT;
            }
        }
    }
    // Path 5: Test animation and skin traversal
    else if ((selector & 0x07) == 5) {
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "");
            (void)ret;
            
            // Traverse animations and skins
            for (const auto& animation : model.animations) {
                (void)animation.name;
                for (const auto& channel : animation.channels) {
                    (void)channel.sampler;
                    (void)channel.target_node;
                    (void)channel.target_path;
                }
                for (const auto& sampler : animation.samplers) {
                    (void)sampler.input;
                    (void)sampler.output;
                    (void)sampler.interpolation;
                }
            }
            
            for (const auto& skin : model.skins) {
                (void)skin.name;
                (void)skin.inverseBindMatrices;
                for (int joint : skin.joints) {
                    (void)joint;
                }
            }
        }
    }
    // Path 6: Test scene and camera traversal
    else if ((selector & 0x07) == 6) {
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "");
            (void)ret;
            
            // Traverse scenes and cameras
            for (const auto& scene : model.scenes) {
                (void)scene.name;
                for (int node : scene.nodes) {
                    (void)node;
                }
            }
            
            for (const auto& camera : model.cameras) {
                (void)camera.name;
                (void)camera.type;
                if (camera.type == "perspective") {
                    (void)camera.perspective.aspectRatio;
                    (void)camera.perspective.yfov;
                    (void)camera.perspective.zfar;
                    (void)camera.perspective.znear;
                } else if (camera.type == "orthographic") {
                    (void)camera.orthographic.xmag;
                    (void)camera.orthographic.ymag;
                    (void)camera.orthographic.zfar;
                    (void)camera.orthographic.znear;
                }
            }
            
            // Test default scene
            if (model.defaultScene >= 0 && model.defaultScene < static_cast<int>(model.scenes.size())) {
                (void)model.scenes[model.defaultScene].name;
            }
        }
    }
    // Path 7: Mixed loading with error handling focus
    else {
        // Consume flags for different configurations
        unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
        bool use_custom_base_dir = fdp.ConsumeBool();
        
        std::string base_dir = use_custom_base_dir ? "/custom/path/" : "";
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               base_dir,
                                               check_sections);
            (void)ret;
            
            // Check error and warning messages
            (void)err.size();
            (void)warn.size();
            
            // Test error recovery by trying to access model even if loading failed
            (void)model.asset.version;
            (void)model.asset.generator;
            (void)model.asset.copyright;
            (void)model.asset.minVersion;
        }
    }
    
    return 0;
}

} // namespace harness_001
