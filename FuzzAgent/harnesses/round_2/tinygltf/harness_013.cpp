#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"
#include "tiny_gltf.h"

namespace harness_013 {
// Harness 013: stb_image callback-based image loading fuzzer for tinygltf
// Targets: stbi_load_from_callbacks, stbi_load_16_from_callbacks, stbi_loadf_from_callbacks,
//          stbi_info_from_callbacks, stbi_is_hdr_from_callbacks, stbi_is_16_bit_from_callbacks
// Focus: Callback-based image loading APIs in stb_image.h module (0.55% region coverage)
// These callback APIs represent 50+ points of undiscovered complexity
// Uses FuzzedDataProvider for structured input splitting to test callback I/O mechanisms
// Differentiates from existing harnesses by targeting callback APIs directly, not memory-based loading


// Include FuzzedDataProvider for structured input splitting

// Include stb_image headers - DO NOT define STB_IMAGE_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

// Include tinygltf header - DO NOT define TINYGLTF_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

// Custom I/O callback context structure
struct CallbackContext {
    const uint8_t* data;
    size_t size;
    size_t position;
    FuzzedDataProvider* fdp; // For dynamic data generation
    int error_mode; // 0: normal, 1: read errors, 2: skip errors, 3: eof errors
};

// Declare stb_image functions we need (they're implemented in libtinygltf.a)
extern "C" {
    unsigned char* stbi_load_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);
    unsigned short* stbi_load_16_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);
    float* stbi_loadf_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);
    int stbi_info_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* comp);
    int stbi_is_hdr_from_callbacks(stbi_io_callbacks const* clbk, void* user);
    int stbi_is_16_bit_from_callbacks(stbi_io_callbacks const* clbk, void* user);
    void stbi_image_free(void* retval_from_stbi_load);
}

// Custom read callback for stbi_io_callbacks
static int custom_read(void* user, char* data, int size) {
    CallbackContext* ctx = reinterpret_cast<CallbackContext*>(user);
    
    // Simulate read errors based on error_mode
    if (ctx->error_mode == 1) {
        // Randomly fail reads
        if (ctx->fdp && ctx->fdp->ConsumeBool()) {
            return 0; // Simulate read failure
        }
    }
    
    if (ctx->position >= ctx->size) {
        return 0; // EOF
    }
    
    int bytes_to_read = static_cast<int>(std::min(ctx->size - ctx->position, static_cast<size_t>(size)));
    if (bytes_to_read > 0) {
        memcpy(data, ctx->data + ctx->position, bytes_to_read);
        ctx->position += bytes_to_read;
    }
    
    return bytes_to_read;
}

// Custom skip callback for stbi_io_callbacks
static void custom_skip(void* user, int n) {
    CallbackContext* ctx = reinterpret_cast<CallbackContext*>(user);
    
    // Simulate skip errors based on error_mode
    if (ctx->error_mode == 2) {
        // Randomly fail or behave incorrectly
        if (ctx->fdp && ctx->fdp->ConsumeBool()) {
            // Do nothing or skip wrong amount
            if (ctx->fdp->ConsumeBool()) {
                return;
            } else {
                n = -n; // Skip backwards
            }
        }
    }
    
    if (n > 0) {
        ctx->position = std::min(ctx->position + static_cast<size_t>(n), ctx->size);
    } else if (n < 0) {
        // Negative skip (unget) - move backwards
        size_t backward = static_cast<size_t>(-n);
        if (backward > ctx->position) {
            ctx->position = 0;
        } else {
            ctx->position -= backward;
        }
    }
}

// Custom eof callback for stbi_io_callbacks
static int custom_eof(void* user) {
    CallbackContext* ctx = reinterpret_cast<CallbackContext*>(user);
    
    // Simulate eof errors based on error_mode
    if (ctx->error_mode == 3) {
        // Randomly report wrong EOF status
        if (ctx->fdp && ctx->fdp->ConsumeBool()) {
            return ctx->position < ctx->size ? 1 : 0; // Invert
        }
    }
    
    return ctx->position >= ctx->size ? 1 : 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for callback testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector (0-6 for different callback API tests)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    // Consume error mode for callback simulation
    int error_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Split input: part for image data, part for fuzzer control
    size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_data_size);
    
    if (image_data.empty()) return 0;
    
    // Create callback context
    CallbackContext ctx;
    ctx.data = image_data.data();
    ctx.size = image_data.size();
    ctx.position = 0;
    ctx.fdp = &fdp;
    ctx.error_mode = error_mode;
    
    // Setup callbacks structure
    stbi_io_callbacks callbacks;
    callbacks.read = custom_read;
    callbacks.skip = custom_skip;
    callbacks.eof = custom_eof;
    
    // Variables for image info
    int width = 0, height = 0, channels = 0;
    
    // Operation 0: stbi_load_from_callbacks (8-bit)
    if (operation == 0) {
        int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4); // 0 = as-is
        
        unsigned char* image = stbi_load_from_callbacks(&callbacks, &ctx, 
                                                       &width, &height, &channels, 
                                                       desired_channels);
        if (image) {
            // Exercise the loaded data briefly
            if (width > 0 && height > 0 && channels > 0) {
                size_t pixel_count = static_cast<size_t>(width) * height * channels;
                if (pixel_count > 0) {
                    // Access some pixels
                    (void)image[0];
                    (void)image[std::min(pixel_count - 1, static_cast<size_t>(100))];
                }
            }
            stbi_image_free(image);
        }
    }
    // Operation 1: stbi_load_16_from_callbacks (16-bit)
    else if (operation == 1) {
        int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
        
        unsigned short* image = stbi_load_16_from_callbacks(&callbacks, &ctx,
                                                           &width, &height, &channels,
                                                           desired_channels);
        if (image) {
            if (width > 0 && height > 0 && channels > 0) {
                size_t pixel_count = static_cast<size_t>(width) * height * channels;
                if (pixel_count > 0) {
                    (void)image[0];
                    (void)image[std::min(pixel_count - 1, static_cast<size_t>(100))];
                }
            }
            stbi_image_free(image);
        }
    }
    // Operation 2: stbi_loadf_from_callbacks (float HDR)
    else if (operation == 2) {
#ifndef STBI_NO_LINEAR
        int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
        
        float* image = stbi_loadf_from_callbacks(&callbacks, &ctx,
                                                &width, &height, &channels,
                                                desired_channels);
        if (image) {
            if (width > 0 && height > 0 && channels > 0) {
                size_t pixel_count = static_cast<size_t>(width) * height * channels;
                if (pixel_count > 0) {
                    (void)image[0];
                    (void)image[std::min(pixel_count - 1, static_cast<size_t>(100))];
                }
            }
            stbi_image_free(image);
        }
#endif
    }
    // Operation 3: stbi_info_from_callbacks (image info only)
    else if (operation == 3) {
        int result = stbi_info_from_callbacks(&callbacks, &ctx, &width, &height, &channels);
        (void)result; // Use result to avoid unused variable warning
    }
    // Operation 4: stbi_is_hdr_from_callbacks (HDR detection)
    else if (operation == 4) {
        int is_hdr = stbi_is_hdr_from_callbacks(&callbacks, &ctx);
        (void)is_hdr;
    }
    // Operation 5: stbi_is_16_bit_from_callbacks (16-bit detection)
    else if (operation == 5) {
        int is_16bit = stbi_is_16_bit_from_callbacks(&callbacks, &ctx);
        (void)is_16bit;
    }
    // Operation 6: Combination test - multiple operations on same data
    else if (operation == 6) {
        // Reset position for multiple operations
        ctx.position = 0;
        int info_result = stbi_info_from_callbacks(&callbacks, &ctx, &width, &height, &channels);
        
        // Reset position
        ctx.position = 0;
        int is_hdr = stbi_is_hdr_from_callbacks(&callbacks, &ctx);
        
        // Reset position
        ctx.position = 0;
        int is_16bit = stbi_is_16_bit_from_callbacks(&callbacks, &ctx);
        
        // Reset position and try loading based on detected properties
        ctx.position = 0;
        if (is_hdr) {
#ifndef STBI_NO_LINEAR
            float* hdr_image = stbi_loadf_from_callbacks(&callbacks, &ctx, &width, &height, &channels, 0);
            if (hdr_image) {
                stbi_image_free(hdr_image);
            }
#endif
        } else if (is_16bit) {
            ctx.position = 0;
            unsigned short* image16 = stbi_load_16_from_callbacks(&callbacks, &ctx, &width, &height, &channels, 0);
            if (image16) {
                stbi_image_free(image16);
            }
        } else {
            ctx.position = 0;
            unsigned char* image8 = stbi_load_from_callbacks(&callbacks, &ctx, &width, &height, &channels, 0);
            if (image8) {
                stbi_image_free(image8);
            }
        }
        
        (void)info_result;
        (void)is_hdr;
        (void)is_16bit;
    }
    
    return 0;
}

} // namespace harness_013
