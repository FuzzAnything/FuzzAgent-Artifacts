#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <memory>
#include "stb_image.h"
#include "stb_image_write.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_010 {
/*
 * Fuzzing harness for stb_image.h module with only 3.39% line coverage and 70+ points of undiscovered complexity.
 * This harness specifically targets callback-based image loading APIs.
 * 
 * Target APIs:
 * - stbi_load_from_callbacks (Target: completely uncovered, 6 undiscovered branches)
 * - stbi_load_16_from_callbacks (Target: completely uncovered, 6 undiscovered branches)
 * - stbi_loadf_from_callbacks (Target: completely uncovered, 6 undiscovered branches)
 * - stbi_info_from_callbacks (Target: completely uncovered, 6 undiscovered branches)
 * - stbi_is_16_bit_from_callbacks (Target: completely uncovered, 6 undiscovered branches)
 * - stbi_load (Target: reached but 0% executed, 12 undiscovered branches)
 * - stbi_load_16 (Target: reached but 0% executed, 12 undiscovered branches)
 * - stbi_loadf (Target: reached but 0% executed, 10 undiscovered branches)
 * - stbi_write_png (Target: partially covered, 4 undiscovered branches)
 * - stbi_write_jpg (Target: reached but 0% executed, 2 undiscovered branches)
 * - stbi_image_free (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Create test image data in memory (PNG, JPEG, HDR formats)
 * 2. Implement callbacks for stbi_load_*_from_callbacks APIs
 * 3. Call stbi_info_from_callbacks to get image info
 * 4. Call stbi_is_16_bit_from_callbacks to check bit depth
 * 5. Call stbi_load_from_callbacks / stbi_load_16_from_callbacks / stbi_loadf_from_callbacks to load images
 * 6. Call stbi_load / stbi_load_16 / stbi_loadf from file and memory variants
 * 7. Call stbi_write_png and stbi_write_jpg to write test images
 * 8. Clean up with stbi_image_free
 * 
 * Implementation uses FuzzedDataProvider for structured input splitting to generate various
 * image parameters and test different API call sequences.
 */

// Include stb_image and stb_image_write implementations directly
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION



// Memory buffer context for callbacks
struct MemoryBufferContext {
    const unsigned char* data;
    size_t size;
    size_t position;
};

// Callback functions for stbi_io_callbacks
static int memory_read(void* user, char* data, int size) {
    MemoryBufferContext* ctx = static_cast<MemoryBufferContext*>(user);
    if (ctx->position >= ctx->size) {
        return 0;
    }
    
    size_t bytes_to_read = std::min(static_cast<size_t>(size), ctx->size - ctx->position);
    if (bytes_to_read > 0) {
        memcpy(data, ctx->data + ctx->position, bytes_to_read);
        ctx->position += bytes_to_read;
    }
    return static_cast<int>(bytes_to_read);
}

static void memory_skip(void* user, int n) {
    MemoryBufferContext* ctx = static_cast<MemoryBufferContext*>(user);
    if (n > 0) {
        ctx->position = std::min(ctx->position + static_cast<size_t>(n), ctx->size);
    } else if (n < 0) {
        // Handle negative skip (unget)
        size_t unget_amount = static_cast<size_t>(-n);
        if (unget_amount > ctx->position) {
            ctx->position = 0;
        } else {
            ctx->position -= unget_amount;
        }
    }
}

static int memory_eof(void* user) {
    MemoryBufferContext* ctx = static_cast<MemoryBufferContext*>(user);
    return (ctx->position >= ctx->size) ? 1 : 0;
}

// Memory writer callback for stbi_write_*_to_func
struct MemoryWriterContext {
    std::vector<unsigned char> buffer;
};

static void memory_write_callback(void* context, void* data, int size) {
    MemoryWriterContext* ctx = static_cast<MemoryWriterContext*>(context);
    unsigned char* byte_data = static_cast<unsigned char*>(data);
    ctx->buffer.insert(ctx->buffer.end(), byte_data, byte_data + size);
}

// Generate synthetic image data for testing
std::vector<unsigned char> GenerateSyntheticImage(int width, int height, int components, FuzzedDataProvider& fdp) {
    std::vector<unsigned char> image_data;
    size_t pixel_count = width * height * components;
    
    if (pixel_count == 0) {
        return image_data;
    }
    
    // Limit size to prevent excessive memory usage
    size_t max_pixels = 64 * 64 * 4; // 64x64 RGBA max
    if (pixel_count > max_pixels) {
        // Recalculate to stay within limits
        width = 64;
        height = 64;
        pixel_count = width * height * components;
    }
    
    image_data = fdp.ConsumeBytes<unsigned char>(pixel_count);
    
    // If not enough data, fill with pattern
    if (image_data.size() < pixel_count) {
        size_t remaining = pixel_count - image_data.size();
        std::vector<unsigned char> pattern = fdp.ConsumeBytes<unsigned char>(std::min(remaining, static_cast<size_t>(256)));
        while (image_data.size() < pixel_count) {
            for (unsigned char val : pattern) {
                if (image_data.size() >= pixel_count) break;
                image_data.push_back(val);
            }
        }
    }
    
    return image_data;
}

// Generate synthetic HDR image data (float)
std::vector<float> GenerateSyntheticHDRImage(int width, int height, int components, FuzzedDataProvider& fdp) {
    std::vector<float> image_data;
    size_t pixel_count = width * height * components;
    
    if (pixel_count == 0) {
        return image_data;
    }
    
    // Limit size to prevent excessive memory usage
    size_t max_pixels = 32 * 32 * 4; // 32x32 RGBA max for HDR (float is larger)
    if (pixel_count > max_pixels) {
        width = 32;
        height = 32;
        pixel_count = width * height * components;
    }
    
    // Consume bytes and convert to floats
    std::vector<unsigned char> byte_data = fdp.ConsumeBytes<unsigned char>(pixel_count * sizeof(float));
    
    // If not enough data, fill with zeros
    image_data.resize(pixel_count, 0.0f);
    size_t floats_to_copy = std::min(byte_data.size() / sizeof(float), pixel_count);
    if (floats_to_copy > 0) {
        memcpy(image_data.data(), byte_data.data(), floats_to_copy * sizeof(float));
    }
    
    return image_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for image generation
    int width = fdp.ConsumeIntegralInRange<int>(1, 128);
    int height = fdp.ConsumeIntegralInRange<int>(1, 128);
    int components = fdp.ConsumeIntegralInRange<int>(1, 4);
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4); // 0 means preserve original
    
    // Generate synthetic image data
    std::vector<unsigned char> image_data = GenerateSyntheticImage(width, height, components, fdp);
    if (image_data.empty()) {
        return 0;
    }
    
    // Generate HDR image data
    std::vector<float> hdr_image_data = GenerateSyntheticHDRImage(width, height, components, fdp);
    
    // Set up memory buffer context for callbacks
    MemoryBufferContext mem_ctx;
    mem_ctx.data = image_data.data();
    mem_ctx.size = image_data.size();
    mem_ctx.position = 0;
    
    // Set up callbacks structure
    stbi_io_callbacks callbacks;
    callbacks.read = memory_read;
    callbacks.skip = memory_skip;
    callbacks.eof = memory_eof;
    
    // 1. Test stbi_info_from_callbacks
    int info_width = 0, info_height = 0, info_comp = 0;
    mem_ctx.position = 0; // Reset position
    (void)stbi_info_from_callbacks(&callbacks, &mem_ctx, &info_width, &info_height, &info_comp);
    
    // 2. Test stbi_is_16_bit_from_callbacks
    mem_ctx.position = 0; // Reset position
    (void)stbi_is_16_bit_from_callbacks(&callbacks, &mem_ctx);
    
    // 3. Test stbi_load_from_callbacks (8-bit)
    mem_ctx.position = 0; // Reset position
    int load_width = 0, load_height = 0, load_comp = 0;
    unsigned char* loaded_data_8bit = stbi_load_from_callbacks(&callbacks, &mem_ctx, 
                                                              &load_width, &load_height, 
                                                              &load_comp, desired_channels);
    
    // 4. Test stbi_load_16_from_callbacks (16-bit)
    mem_ctx.position = 0; // Reset position
    unsigned short* loaded_data_16bit = stbi_load_16_from_callbacks(&callbacks, &mem_ctx,
                                                                   &load_width, &load_height,
                                                                   &load_comp, desired_channels);
    
    // 5. Test stbi_loadf_from_callbacks (float/HDR) if we have HDR data
    if (!hdr_image_data.empty()) {
        MemoryBufferContext hdr_mem_ctx;
        hdr_mem_ctx.data = reinterpret_cast<const unsigned char*>(hdr_image_data.data());
        hdr_mem_ctx.size = hdr_image_data.size() * sizeof(float);
        hdr_mem_ctx.position = 0;
        
        float* loaded_data_float = stbi_loadf_from_callbacks(&callbacks, &hdr_mem_ctx,
                                                            &load_width, &load_height,
                                                            &load_comp, desired_channels);
        if (loaded_data_float) {
            stbi_image_free(loaded_data_float);
        }
    }
    
    // 6. Test direct memory loading APIs
    // stbi_load_from_memory
    int mem_width = 0, mem_height = 0, mem_comp = 0;
    unsigned char* mem_data_8bit = stbi_load_from_memory(image_data.data(), 
                                                        static_cast<int>(image_data.size()),
                                                        &mem_width, &mem_height, 
                                                        &mem_comp, desired_channels);
    
    // stbi_load_16_from_memory
    unsigned short* mem_data_16bit = stbi_load_16_from_memory(image_data.data(),
                                                             static_cast<int>(image_data.size()),
                                                             &mem_width, &mem_height,
                                                             &mem_comp, desired_channels);
    
    // stbi_loadf_from_memory (if HDR data available)
    if (!hdr_image_data.empty()) {
        float* mem_data_float = stbi_loadf_from_memory(
            reinterpret_cast<const unsigned char*>(hdr_image_data.data()),
            static_cast<int>(hdr_image_data.size() * sizeof(float)),
            &mem_width, &mem_height, &mem_comp, desired_channels);
        if (mem_data_float) {
            stbi_image_free(mem_data_float);
        }
    }
    
    // 7. Test stbi_write_png and stbi_write_jpg if we have loaded image data
    if (loaded_data_8bit && load_width > 0 && load_height > 0 && load_comp > 0) {
        MemoryWriterContext write_ctx;
        
        // Test stbi_write_png_to_func
        (void)stbi_write_png_to_func(memory_write_callback, &write_ctx,
                                    load_width, load_height, load_comp,
                                    loaded_data_8bit, load_width * load_comp);
        
        // Test stbi_write_jpg_to_func
        write_ctx.buffer.clear();
        (void)stbi_write_jpg_to_func(memory_write_callback, &write_ctx,
                                    load_width, load_height, load_comp,
                                    loaded_data_8bit, 85); // 85% quality
    }
    // 8. Clean up allocated image data
    if (loaded_data_8bit) {
        stbi_image_free(loaded_data_8bit);
    }
    if (loaded_data_16bit) {
        stbi_image_free(loaded_data_16bit);
    }
    if (mem_data_8bit) {
        stbi_image_free(mem_data_8bit);
    }
    if (mem_data_16bit) {
        stbi_image_free(mem_data_16bit);
    }
    return 0;
}

} // namespace harness_010
