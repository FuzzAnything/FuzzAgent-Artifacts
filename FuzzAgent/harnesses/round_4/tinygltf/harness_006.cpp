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
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_006 {
/*
 * Fuzzing harness for tinygltf library - harness_006.cpp
 * 
 * This harness specifically targets WriteGltfSceneToFile with embedded images,
 * testing the default image writing path and stb_image_write.h integration:
 * - WriteGltfSceneToFile: Comprehensive testing with embedded images parameter
 * - Default image writing path through stb_image_write.h integration
 * - Model creation with various image configurations (PNG, JPG, BMP, GIF)
 * - Testing image embedding (embedImages=true) vs external files (embedImages=false)
 * - Complex models with textures, materials, and multiple image variations
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Memory-based operations only (LoadASCIIFromString, LoadBinaryFromMemory)
 * - harness_001: Basic file I/O operations (load/write without validation)
 * - harness_002: Round-trip operations with validation of existing files
 * - harness_003: Error paths, boundary conditions, and failure scenarios
 * - harness_004: Focus on creating models from scratch and comprehensive serialization testing
 * - harness_005: Specialized targeting of image writing APIs, texture export, and binary image data serialization
 * - harness_006: Specifically targets WriteGltfSceneToFile with embedded images, default image writing path, and stb_image_write.h integration
 * 
 * Targets 169+ undiscovered branches in WriteGltfSceneToFile and 0% coverage in stb_image_write.h module.
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing of different
 * image configurations, embedding scenarios, and serialization parameter combinations.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation with stb_image_write
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper function to create a temporary file for writing
static std::string create_output_temp_file(const std::string& extension) {
    char temp_filename[64];
    if (extension == ".glb") {
        strcpy(temp_filename, "/tmp/tinygltf_write_XXXXXX.glb");
    } else {
        strcpy(temp_filename, "/tmp/tinygltf_write_XXXXXX.gltf");
    }
    
    int fd = mkstemps(temp_filename, extension == ".glb" ? 4 : 5);
    if (fd == -1) {
        return "";
    }
    close(fd);
    
    return std::string(temp_filename);
}

// Helper function to consume a valid UTF-8 string from fuzzer input
// Generates ASCII-only strings (0x20-0x7E) which are always valid UTF-8
static std::string consume_valid_utf8_string(FuzzedDataProvider& fdp, size_t max_length) {
    if (fdp.remaining_bytes() == 0 || max_length == 0) {
        return "";
    }
    
    // Determine actual length to consume
    size_t length = fdp.ConsumeIntegralInRange<size_t>(0, std::min(max_length, fdp.remaining_bytes()));
    if (length == 0) {
        return "";
    }
    
    // Consume raw bytes
    std::vector<uint8_t> bytes = fdp.ConsumeBytes<uint8_t>(length);
    
    // Convert to valid UTF-8 (ASCII subset: printable characters 32-126)
    std::string result;
    result.reserve(bytes.size());
    
    for (uint8_t byte : bytes) {
        // Map byte to ASCII printable range (32-126)
        // This ensures valid UTF-8 while preserving some variability
        char c = 32 + (byte % 95);  // 95 printable ASCII chars (32-126)
        result.push_back(c);
    }
    
    return result;
}

// Helper to create a test image with various configurations
static tinygltf::Image create_test_image(FuzzedDataProvider& fdp, int image_idx, size_t max_image_size = 4096) {
    tinygltf::Image image;
    
    // Generate image name from fuzzer input (valid UTF-8)
    if (fdp.remaining_bytes() > 5) {
        image.name = consume_valid_utf8_string(fdp, 32);
    }
    // Determine image type from fuzzer input
    uint8_t image_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (image_type) {
        case 0:
            image.mimeType = "image/png";
            break;
        case 1:
            image.mimeType = "image/jpeg";
            break;
        case 2:
            image.mimeType = "image/bmp";
            break;
        case 3:
            image.mimeType = "image/gif";
            break;
    }
    
    // Generate image dimensions (capped to reasonable values)
    image.width = fdp.ConsumeIntegralInRange<int>(1, 256);
    image.height = fdp.ConsumeIntegralInRange<int>(1, 256);
    image.component = fdp.ConsumeIntegralInRange<int>(1, 4);  // 1=gray, 2=gray+alpha, 3=RGB, 4=RGBA
    image.bits = 8;  // Only 8-bit supported by default writer
    image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    
    // Determine if image should be encoded "as_is" or raw pixel data
    bool as_is = fdp.ConsumeBool();
    image.as_is = as_is;
    
    // Generate image data
    size_t pixel_count = static_cast<size_t>(image.width) * image.height * image.component;
    size_t max_data_size = std::min(pixel_count, max_image_size);
    
    if (max_data_size > 0 && fdp.remaining_bytes() >= max_data_size) {
        if (as_is) {
            // For "as_is" images, generate encoded data (simulate PNG/JPG encoded bytes)
            size_t encoded_size = fdp.ConsumeIntegralInRange<size_t>(10, std::min(max_data_size, fdp.remaining_bytes()));
            if (encoded_size > 0) {
                image.image = fdp.ConsumeBytes<unsigned char>(encoded_size);
            }
        } else {
            // For raw pixel data, generate pixel_count bytes
            if (pixel_count <= fdp.remaining_bytes()) {
                image.image = fdp.ConsumeBytes<unsigned char>(pixel_count);
            } else {
                // Not enough data for full image, generate partial
                size_t partial_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(pixel_count, fdp.remaining_bytes()));
                if (partial_size > 0) {
                    image.image = fdp.ConsumeBytes<unsigned char>(partial_size);
                }
            }
        }
    }
    
    // Generate URI (optional - some images may have URIs, some may not)
    if (fdp.remaining_bytes() > 10 && fdp.ConsumeBool()) {
        std::string uri_type = fdp.ConsumeBool() ? "data:" : "file:";
        if (uri_type == "data:") {
            image.uri = "data:image/png;base64," + std::to_string(image_idx);
        } else {
            image.uri = "images/image_" + std::to_string(image_idx) + ".png";
        }
    }
    
    // Randomly set bufferView (some images may reference buffer data)
    if (fdp.remaining_bytes() > 5 && fdp.ConsumeBool()) {
        image.bufferView = fdp.ConsumeIntegralInRange<int>(-1, 10);
    } else {
        image.bufferView = -1;
    }
    
    return image;
}

// Helper to create a test buffer
static tinygltf::Buffer create_test_buffer(FuzzedDataProvider& fdp, int buffer_idx) {
    tinygltf::Buffer buffer;
    
    // Generate buffer name from fuzzer input (valid UTF-8)
    if (fdp.remaining_bytes() > 5) {
        buffer.name = consume_valid_utf8_string(fdp, 32);
    }
    
    // Generate buffer data
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(4096, fdp.remaining_bytes()));
    if (buffer_size > 0) {
        buffer.data = fdp.ConsumeBytes<unsigned char>(buffer_size);
    }
    
    // Generate URI (optional)
    if (fdp.remaining_bytes() > 10 && fdp.ConsumeBool()) {
        buffer.uri = "buffer_" + std::to_string(buffer_idx) + ".bin";
    }
    
    return buffer;
}

// Helper to create a test texture referencing an image
static tinygltf::Texture create_test_texture(FuzzedDataProvider& fdp, int texture_idx, int image_idx) {
    tinygltf::Texture texture;
    
    // Generate texture name from fuzzer input (valid UTF-8)
    if (fdp.remaining_bytes() > 5) {
        texture.name = consume_valid_utf8_string(fdp, 32);
    }
    
    // Reference an image
    texture.source = image_idx;
    
    // Set sampler (optional)
    if (fdp.remaining_bytes() > 5 && fdp.ConsumeBool()) {
        texture.sampler = fdp.ConsumeIntegralInRange<int>(-1, 10);
    }
    
    return texture;
}

// Helper to create a test material referencing a texture
static tinygltf::Material create_test_material(FuzzedDataProvider& fdp, int material_idx, int texture_idx) {
    tinygltf::Material material;
    
    // Generate material name from fuzzer input (valid UTF-8)
    if (fdp.remaining_bytes() > 5) {
        material.name = consume_valid_utf8_string(fdp, 32);
    }
    
    // Create PBR metallic roughness parameters
    material.pbrMetallicRoughness.baseColorFactor = {
        fdp.ConsumeFloatingPoint<float>(),
        fdp.ConsumeFloatingPoint<float>(),
        fdp.ConsumeFloatingPoint<float>(),
        fdp.ConsumeFloatingPoint<float>()
    };
    
    material.pbrMetallicRoughness.metallicFactor = fdp.ConsumeFloatingPoint<float>();
    material.pbrMetallicRoughness.roughnessFactor = fdp.ConsumeFloatingPoint<float>();
    
    // Reference a texture for base color
    if (texture_idx >= 0 && fdp.ConsumeBool()) {
        material.pbrMetallicRoughness.baseColorTexture.index = texture_idx;
        material.pbrMetallicRoughness.baseColorTexture.texCoord = 0;
    }
    
    // Set other material properties
    material.alphaMode = fdp.ConsumeBool() ? "BLEND" : "OPAQUE";
    material.doubleSided = fdp.ConsumeBool();
    material.emissiveFactor = {
        fdp.ConsumeFloatingPoint<float>(),
        fdp.ConsumeFloatingPoint<float>(),
        fdp.ConsumeFloatingPoint<float>()
    };
    
    return material;
}

// Helper to populate a model with images, textures, and materials
static void populate_model_with_images_and_textures(tinygltf::Model& model, FuzzedDataProvider& fdp) {
    // Determine number of images to create (0-5)
    size_t num_images = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    // Create images
    for (size_t i = 0; i < num_images && fdp.remaining_bytes() > 100; i++) {
        model.images.push_back(create_test_image(fdp, i));
    }
    
    // Create buffers (some images may reference buffer views)
    size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    for (size_t i = 0; i < num_buffers && fdp.remaining_bytes() > 50; i++) {
        model.buffers.push_back(create_test_buffer(fdp, i));
    }
    
    // Create textures referencing images
    size_t num_textures = std::min(num_images, fdp.ConsumeIntegralInRange<size_t>(0, 5));
    for (size_t i = 0; i < num_textures && fdp.remaining_bytes() > 50; i++) {
        int image_idx = (i < model.images.size()) ? i : -1;
        model.textures.push_back(create_test_texture(fdp, i, image_idx));
    }
    
    // Create materials referencing textures
    size_t num_materials = std::min(num_textures, fdp.ConsumeIntegralInRange<size_t>(0, 5));
    for (size_t i = 0; i < num_materials && fdp.remaining_bytes() > 50; i++) {
        int texture_idx = (i < model.textures.size()) ? i : -1;
        model.materials.push_back(create_test_material(fdp, i, texture_idx));
    }
    
    // Set model metadata
    if (fdp.remaining_bytes() > 10) {
        model.asset.version = "2.0";
        model.asset.generator = "Fuzzer harness_006";
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough data for meaningful testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF context
    tinygltf::TinyGLTF ctx;
    
    // Parse test configuration from fuzzer input
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Bit layout for configuration:
    // Bit 0: embedImages (0=false, 1=true)
    // Bit 1: embedBuffers (0=false, 1=true)
    // Bit 2: prettyPrint (0=false, 1=true)
    // Bit 3: writeBinary (0=glTF, 1=GLB)
    // Bits 4-5: test scenario (00=basic, 01=complex, 10=mixed, 11=roundtrip)
    // Bit 6: use default writer (always 1 for this harness)
    // Bit 7: validate after write
    
    bool embed_images = (config_byte & 0x01) != 0;
    bool embed_buffers = (config_byte & 0x02) != 0;
    bool pretty_print = (config_byte & 0x04) != 0;
    bool write_binary = (config_byte & 0x08) != 0;
    uint8_t test_scenario = (config_byte >> 4) & 0x03;
    bool validate_after_write = (config_byte & 0x80) != 0;
    
    // Ensure we use default image writer (not custom callbacks)
    // This harness specifically tests the default stb_image_write.h integration
    ctx.SetImageWriter(nullptr, nullptr);
    
    // Create model with images and textures based on test scenario
    tinygltf::Model model;
    
    switch (test_scenario) {
        case 0:  // Basic: simple model with 1-2 images
            populate_model_with_images_and_textures(model, fdp);
            // Limit to 2 images max for basic test
            if (model.images.size() > 2) {
                model.images.resize(2);
            }
            break;
            
        case 1:  // Complex: full model with multiple images, textures, materials
            populate_model_with_images_and_textures(model, fdp);
            break;
            
        case 2:  // Mixed: model with some images having URIs, some not
            populate_model_with_images_and_textures(model, fdp);
            // Randomly clear URIs from some images
            for (size_t i = 0; i < model.images.size(); i++) {
                if (fdp.ConsumeBool()) {
                    model.images[i].uri.clear();
                }
            }
            break;
            
        case 3:  // Edge cases: empty images, invalid dimensions, etc.
            populate_model_with_images_and_textures(model, fdp);
            // Modify some images to test edge cases
            for (size_t i = 0; i < model.images.size() && i < 3; i++) {
                if (fdp.ConsumeBool()) {
                    // Empty image data
                    model.images[i].image.clear();
                }
                if (fdp.ConsumeBool()) {
                    // Invalid dimensions
                    model.images[i].width = 0;
                    model.images[i].height = 0;
                }
                if (fdp.ConsumeBool()) {
                    // Unsupported pixel format
                    model.images[i].bits = 16;
                }
            }
            break;
    }
    
    // Skip if no images were created (not enough fuzzer data)
    if (model.images.empty()) {
        return 0;
    }
    
    // Create output filename
    std::string output_filename = create_output_temp_file(write_binary ? ".glb" : ".gltf");
    if (output_filename.empty()) {
        return 0;
    }
    
    // Clean up the file after testing
    struct TempFileCleanup {
        std::string filename;
        ~TempFileCleanup() { if (!filename.empty()) unlink(filename.c_str()); }
    } cleanup{output_filename};
    
    // Main test: WriteGltfSceneToFile with embedded images
    // This is the primary API we're targeting
    bool write_result = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                embed_images,
                                                embed_buffers,
                                                pretty_print,
                                                write_binary);
    
    // The result is intentionally ignored to explore both success and failure paths
    (void)write_result;
    
    // Optional: Try to read back the written file if validation is requested
    if (validate_after_write && write_result && fdp.remaining_bytes() > 50) {
        tinygltf::Model readback_model;
        std::string err, warn;
        
        bool readback_result = false;
        if (write_binary) {
            readback_result = ctx.LoadBinaryFromFile(&readback_model, &err, &warn,
                                                    output_filename,
                                                    tinygltf::REQUIRE_VERSION);
        } else {
            readback_result = ctx.LoadASCIIFromFile(&readback_model, &err, &warn,
                                                   output_filename,
                                                   tinygltf::REQUIRE_VERSION);
        }
        (void)readback_result;
    }
    
    // Additional test: Write to stream for comparison
    if (fdp.remaining_bytes() > 50) {
        std::ostringstream oss;
        bool stream_result = ctx.WriteGltfSceneToStream(&model, oss,
                                                       pretty_print,
                                                       write_binary);
        (void)stream_result;
    }
    
    // Test with different embedding combinations to explore more code paths
    if (fdp.remaining_bytes() > 100) {
        // Test opposite embedding settings
        std::string alt_filename = create_output_temp_file(write_binary ? ".glb" : ".gltf");
        if (!alt_filename.empty()) {
            struct TempFileCleanup2 {
                std::string filename;
                ~TempFileCleanup2() { if (!filename.empty()) unlink(filename.c_str()); }
            } cleanup2{alt_filename};
            
            bool alt_write_result = ctx.WriteGltfSceneToFile(&model, alt_filename,
                                                            !embed_images,  // Opposite of first test
                                                            !embed_buffers, // Opposite of first test
                                                            !pretty_print,  // Opposite of first test
                                                            write_binary);  // Same binary setting
            
            (void)alt_write_result;
        }
    }
    
    return 0;
}

} // namespace harness_006
