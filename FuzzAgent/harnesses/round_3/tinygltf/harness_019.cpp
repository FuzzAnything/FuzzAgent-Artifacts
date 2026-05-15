#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"

namespace harness_019 {
/*
 * Fuzzing harness for tinygltf library targeting comprehensive stb_image.h image loading operations
 * Goal: Exercise image loading pipeline with detection-first approach: validate → detect → load → cleanup
 * Primary Target APIs (with undiscovered branch complexity from coverage analysis):
 *   - stbi_load (9 undiscovered branches) - Standard 8-bit image loading
 *   - stbi_load_16 (9 undiscovered branches) - 16-bit image loading  
 *   - stbi_loadf (7 undiscovered branches) - HDR floating-point image loading
 *   - stbi_info (5 undiscovered branches) - Image metadata extraction
 *   - stbi_is_hdr (2 undiscovered branches) - HDR format detection
 *   - stbi_is_16_bit (5 undiscovered branches) - 16-bit format detection
 *   - stbi_image_free (Required Helper) - Memory cleanup
 * Secondary Targets (optional):
 *   - stbi_is_hdr_from_callbacks (0% coverage)
 *   - stbi_is_16_bit_from_file (3 undiscovered branches)
 *   - stbi_info_from_file (3 undiscovered branches)
 * Strategy: Detection-First Pipeline
 *   1. Setup Phase: Prepare fuzzer-provided image data
 *   2. Detection Phase: Call stbi_info/stbi_is_hdr/stbi_is_16_bit to validate and detect format
 *   3. Loading Phase: Based on detection results, call appropriate loader (stbi_load/stbi_load_16/stbi_loadf)
 *   4. Cleanup Phase: Proper memory management with stbi_image_free
 * Differentiation from existing harnesses:
 *   - harness_007: Direct STB API calls but no detection-first approach
 *   - harness_011: Comprehensive loading with callbacks but minimal detection logic
 *   - harness_017: Complete lifecycle with loading → writing, not focused on detection
 *   - harness_018: Image writing core functions only
 *   - harness_019: DETECTION-FIRST PIPELINE emphasizing validation before loading
 * Unique Value: Exercises error paths in detection APIs, explores format-specific loading logic,
 *               and tests the conditional loading flow based on detected format properties.
 */



// Include STB headers (implementation already built via tinygltf)

// Simple callback context for format detection testing
struct DetectionCallbackContext {
    const uint8_t* data_ptr;
    size_t data_size;
    size_t position;
    bool simulate_invalid_io;  // For testing error paths
};

// Callback functions for detection APIs testing
static int detection_read_callback(void* user, char* data, int size) {
    DetectionCallbackContext* ctx = static_cast<DetectionCallbackContext*>(user);
    if (!ctx || ctx->simulate_invalid_io || size <= 0) {
        return 0;
    }
    
    size_t remaining = ctx->data_size - ctx->position;
    size_t to_read = std::min<size_t>(remaining, size);
    
    if (to_read > 0 && ctx->data_ptr) {
        memcpy(data, ctx->data_ptr + ctx->position, to_read);
        ctx->position += to_read;
        return static_cast<int>(to_read);
    }
    
    return 0;
}

static void detection_skip_callback(void* user, int n) {
    DetectionCallbackContext* ctx = static_cast<DetectionCallbackContext*>(user);
    if (!ctx || ctx->simulate_invalid_io) {
        return;
    }
    
    if (n > 0) {
        size_t remaining = ctx->data_size - ctx->position;
        size_t to_skip = std::min<size_t>(remaining, n);
        ctx->position += to_skip;
    } else if (n < 0) {
        size_t to_back = std::min<size_t>(ctx->position, -n);
        ctx->position -= to_back;
    }
}

static int detection_eof_callback(void* user) {
    DetectionCallbackContext* ctx = static_cast<DetectionCallbackContext*>(user);
    if (!ctx || ctx->simulate_invalid_io) {
        return 1;
    }
    return (ctx->position >= ctx->data_size) ? 1 : 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough data for meaningful testing
    // We need at least some bytes for image data plus configuration parameters
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: configuration parameters first, then image data
    // Configuration parameters for testing different scenarios
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_callbacks = fdp.ConsumeBool();
    bool simulate_invalid_io = fdp.ConsumeBool();
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);  // 0 means "as-is"
    
    // Consume remaining bytes as image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (image_data.empty()) {
        return 0;
    }
    
    // Setup callbacks context if needed
    DetectionCallbackContext callback_ctx;
    stbi_io_callbacks callbacks;
    
    if (use_callbacks) {
        callback_ctx.data_ptr = image_data.data();
        callback_ctx.data_size = image_data.size();
        callback_ctx.position = 0;
        callback_ctx.simulate_invalid_io = simulate_invalid_io;
        
        callbacks.read = detection_read_callback;
        callbacks.skip = detection_skip_callback;
        callbacks.eof = detection_eof_callback;
    }
    
    // PHASE 1: DETECTION AND VALIDATION
    int width = 0, height = 0, components = 0;
    int is_hdr = 0;
    int is_16bit = 0;
    int info_result = 0;
    
    // Test different detection APIs based on test scenario
    switch (test_scenario % 4) {
        case 0:
            // Use memory-based detection APIs
            info_result = stbi_info_from_memory(image_data.data(), image_data.size(), 
                                               &width, &height, &components);
            is_hdr = stbi_is_hdr_from_memory(image_data.data(), image_data.size());
            is_16bit = stbi_is_16_bit_from_memory(image_data.data(), image_data.size());
            break;
            
        case 1:
            // Use callback-based detection APIs
            if (use_callbacks) {
                info_result = stbi_info_from_callbacks(&callbacks, &callback_ctx, 
                                                      &width, &height, &components);
                is_hdr = stbi_is_hdr_from_callbacks(&callbacks, &callback_ctx);
                is_16bit = stbi_is_16_bit_from_callbacks(&callbacks, &callback_ctx);
            } else {
                // Fall back to memory-based
                info_result = stbi_info_from_memory(image_data.data(), image_data.size(), 
                                                   &width, &height, &components);
                is_hdr = stbi_is_hdr_from_memory(image_data.data(), image_data.size());
                is_16bit = stbi_is_16_bit_from_memory(image_data.data(), image_data.size());
            }
            break;
            
        case 2:
            // Test with file-based detection (simulated via memory)
            // Note: In practice, we'd need actual files, but we test the memory equivalents
            info_result = stbi_info_from_memory(image_data.data(), image_data.size(), 
                                               &width, &height, &components);
            is_hdr = stbi_is_hdr_from_memory(image_data.data(), image_data.size());
            is_16bit = stbi_is_16_bit_from_memory(image_data.data(), image_data.size());
            break;
            
        case 3:
            // Mixed detection approach - test individual APIs separately
            info_result = stbi_info_from_memory(image_data.data(), image_data.size(), 
                                               &width, &height, &components);
            // Reset callback position for separate calls
            if (use_callbacks) {
                callback_ctx.position = 0;
                is_hdr = stbi_is_hdr_from_callbacks(&callbacks, &callback_ctx);
                callback_ctx.position = 0;
                is_16bit = stbi_is_16_bit_from_callbacks(&callbacks, &callback_ctx);
            } else {
                is_hdr = stbi_is_hdr_from_memory(image_data.data(), image_data.size());
                is_16bit = stbi_is_16_bit_from_memory(image_data.data(), image_data.size());
            }
            break;
    }
    
    // PHASE 2: LOADING BASED ON DETECTION RESULTS
    // Reset positions for loading phase
    if (use_callbacks) {
        callback_ctx.position = 0;
    }
    
    // Determine which loader to use based on detection results
    // We'll try multiple loaders regardless of detection to exercise all code paths
    void* loaded_data_8bit = nullptr;
    void* loaded_data_16bit = nullptr; 
    void* loaded_data_float = nullptr;
    
    int loaded_width = 0, loaded_height = 0, loaded_channels = 0;
    
    // Always try 8-bit loading (standard)
    if (!use_callbacks) {
        loaded_data_8bit = stbi_load_from_memory(image_data.data(), image_data.size(),
                                                &loaded_width, &loaded_height, &loaded_channels,
                                                desired_channels > 0 ? desired_channels : 0);
    } else {
        callback_ctx.position = 0;
        loaded_data_8bit = stbi_load_from_callbacks(&callbacks, &callback_ctx,
                                                   &loaded_width, &loaded_height, &loaded_channels,
                                                   desired_channels > 0 ? desired_channels : 0);
    }
    
    // Try 16-bit loading if detection suggests it might be 16-bit OR we want to exercise the API
    if (is_16bit || (test_scenario % 3 == 0)) {
        if (!use_callbacks) {
            // Reset callback position if we were using callbacks before
            if (use_callbacks) callback_ctx.position = 0;
            loaded_data_16bit = reinterpret_cast<void*>(
                stbi_load_16_from_memory(image_data.data(), image_data.size(),
                                        &loaded_width, &loaded_height, &loaded_channels,
                                        desired_channels > 0 ? desired_channels : 0));
        } else {
            callback_ctx.position = 0;
            loaded_data_16bit = reinterpret_cast<void*>(
                stbi_load_16_from_callbacks(&callbacks, &callback_ctx,
                                           &loaded_width, &loaded_height, &loaded_channels,
                                           desired_channels > 0 ? desired_channels : 0));
        }
    }
    
    // Try float/HDR loading if detection suggests HDR OR we want to exercise the API
    if (is_hdr || (test_scenario % 5 == 0)) {
        if (!use_callbacks) {
            if (use_callbacks) callback_ctx.position = 0;
            loaded_data_float = reinterpret_cast<void*>(
                stbi_loadf_from_memory(image_data.data(), image_data.size(),
                                      &loaded_width, &loaded_height, &loaded_channels,
                                      desired_channels > 0 ? desired_channels : 0));
        } else {
            callback_ctx.position = 0;
            loaded_data_float = reinterpret_cast<void*>(
                stbi_loadf_from_callbacks(&callbacks, &callback_ctx,
                                         &loaded_width, &loaded_height, &loaded_channels,
                                         desired_channels > 0 ? desired_channels : 0));
        }
    }
    
    // PHASE 3: VALIDATE AND PROCESS LOADED DATA (if any)
    // Simple validation of loaded data
    if (loaded_data_8bit) {
        // Could do basic validation here, but just having it loaded exercises the API
        // For fuzzing purposes, the mere call with fuzzer data is sufficient
    }
    
    if (loaded_data_16bit) {
        // 16-bit data validation placeholder
    }
    
    if (loaded_data_float) {
        // Float data validation placeholder
    }
    
    // PHASE 4: CLEANUP
    // Properly free all allocated image data
    if (loaded_data_8bit) {
        stbi_image_free(loaded_data_8bit);
    }
    
    if (loaded_data_16bit) {
        stbi_image_free(loaded_data_16bit);
    }
    
    if (loaded_data_float) {
        stbi_image_free(loaded_data_float);
    }
    
    return 0;
}

} // namespace harness_019
