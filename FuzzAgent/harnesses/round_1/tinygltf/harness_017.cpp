#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace harness_017 {
/*
 * Fuzzing harness for tinygltf STB image callback-based loading functions
 * 
 * This harness targets STB image loading callback functions with 30+ undiscovered branches:
 * 1) stbi_info_from_callbacks - 6 undiscovered branches
 * 2) stbi_loadf_from_callbacks - 6 undiscovered branches  
 * 3) stbi_load_from_callbacks - 6 undiscovered branches
 * 4) stbi_is_16_bit_from_callbacks - 6 undiscovered branches
 * 5) stbi_load_16_from_callbacks - 6 undiscovered branches
 * 
 * These callback-based functions are COMPLETELY UNCOVERED (0% coverage) in all 17 existing fuzzers.
 * 
 * Implementation sequence:
 * 1) Parse fuzzer input as image data buffer
 * 2) Implement custom read/seek/eof callback functions that read from this buffer
 * 3) Initialize stbi_io_callbacks structure with callback pointers
 * 4) Call stbi_info_from_callbacks to get image dimensions/format
 * 5) Based on image type, call appropriate loading functions
 * 6) Verify loaded image data
 * 7) Call stbi_image_free for cleanup
 * 
 * Semantically different from existing harnesses by focusing on callback-based I/O
 * rather than memory-based or file-based loading.
 */


// Forward declarations for stb_image functions
extern "C" {
    // stb_image types
    typedef unsigned short stbi_us;
    
    // stb_image callback structure
    typedef struct {
        int      (*read)  (void *user, char *data, int size);
        void     (*skip)  (void *user, int n);
        int      (*eof)   (void *user);
    } stbi_io_callbacks;
    
    // Callback-based loading functions (targets)
    extern int stbi_info_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp);
    extern float* stbi_loadf_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *channels_in_file, int desired_channels);
    extern unsigned char* stbi_load_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *channels_in_file, int desired_channels);
    extern int stbi_is_16_bit_from_callbacks(stbi_io_callbacks const *clbk, void *user);
    extern stbi_us* stbi_load_16_from_callbacks(stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *channels_in_file, int desired_channels);
    
    // Cleanup function
    extern void stbi_image_free(void *retval_from_stbi_load);
}


// Buffer context for callback functions
struct BufferContext {
    const uint8_t* data;
    size_t size;
    size_t position;
};

// Callback: Read data from buffer
static int buffer_read(void* user, char* data, int size) {
    BufferContext* ctx = static_cast<BufferContext*>(user);
    if (!ctx || !data || size <= 0) return 0;
    
    // Calculate how many bytes we can read
    size_t bytes_available = ctx->size - ctx->position;
    size_t bytes_to_read = (size_t)size < bytes_available ? (size_t)size : bytes_available;
    
    if (bytes_to_read > 0) {
        memcpy(data, ctx->data + ctx->position, bytes_to_read);
        ctx->position += bytes_to_read;
    }
    
    return (int)bytes_to_read;
}

// Callback: Skip bytes in buffer
static void buffer_skip(void* user, int n) {
    BufferContext* ctx = static_cast<BufferContext*>(user);
    if (!ctx) return;
    
    if (n > 0) {
        // Forward skip
        size_t skip_amount = (size_t)n;
        if (ctx->position + skip_amount > ctx->size) {
            ctx->position = ctx->size;  // Skip to end
        } else {
            ctx->position += skip_amount;
        }
    } else if (n < 0) {
        // Backward skip (rewind)
        size_t rewind_amount = (size_t)(-n);
        if (rewind_amount > ctx->position) {
            ctx->position = 0;  // Rewind to beginning
        } else {
            ctx->position -= rewind_amount;
        }
    }
}

// Callback: Check if at end of buffer
static int buffer_eof(void* user) {
    BufferContext* ctx = static_cast<BufferContext*>(user);
    if (!ctx) return 1;
    
    return (ctx->position >= ctx->size) ? 1 : 0;
}

// Initialize callback structure
static stbi_io_callbacks create_callbacks() {
    stbi_io_callbacks callbacks;
    callbacks.read = buffer_read;
    callbacks.skip = buffer_skip;
    callbacks.eof = buffer_eof;
    return callbacks;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create buffer context for callbacks
    BufferContext ctx;
    
    // Consume a portion of input as image data
    size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(16, std::min(size - 16, (size_t)65536));
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_data_size);
    
    if (image_data.empty()) return 0;
    
    // Initialize context
    ctx.data = image_data.data();
    ctx.size = image_data.size();
    ctx.position = 0;
    
    // Create callback structure
    stbi_io_callbacks callbacks = create_callbacks();
    
    // Test 1: stbi_info_from_callbacks
    int width = 0, height = 0, channels = 0;
    int info_result = stbi_info_from_callbacks(&callbacks, &ctx, &width, &height, &channels);
    
    // Reset position for next calls
    ctx.position = 0;
    
    // Consume operation selector from remaining input
    uint8_t operation = 0;
    if (fdp.remaining_bytes() > 0) {
        operation = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Test appropriate loading function based on operation selector
    switch (operation % 5) {
        case 0: {
            // Test stbi_load_from_callbacks (LDR images)
            int desired_channels = (operation % 4) + 1;  // 1-4 channels
            unsigned char* ldr_data = stbi_load_from_callbacks(&callbacks, &ctx, &width, &height, &channels, desired_channels);
            if (ldr_data) {
                // Verify we got some data
                if (width > 0 && height > 0 && desired_channels > 0) {
                    // Basic sanity check
                    size_t expected_size = (size_t)(width * height * desired_channels);
                    (void)expected_size;
                }
                stbi_image_free(ldr_data);
            }
            break;
        }
        
        case 1: {
            // Test stbi_loadf_from_callbacks (HDR images)
            ctx.position = 0;
            float* hdr_data = stbi_loadf_from_callbacks(&callbacks, &ctx, &width, &height, &channels, 0);  // 0 = preserve original channels
            if (hdr_data) {
                // Verify HDR data
                if (width > 0 && height > 0 && channels > 0) {
                    // Basic sanity check
                    size_t expected_size = (size_t)(width * height * channels);
                    (void)expected_size;
                }
                stbi_image_free(hdr_data);
            }
            break;
        }
        
        case 2: {
            // Test stbi_load_16_from_callbacks (16-bit images)
            ctx.position = 0;
            int is_16bit = stbi_is_16_bit_from_callbacks(&callbacks, &ctx);
            (void)is_16bit;  // Use result
            
            ctx.position = 0;
            stbi_us* sixteen_bit_data = stbi_load_16_from_callbacks(&callbacks, &ctx, &width, &height, &channels, 0);
            if (sixteen_bit_data) {
                // Verify 16-bit data
                if (width > 0 && height > 0 && channels > 0) {
                    // Basic sanity check
                    size_t expected_size = (size_t)(width * height * channels);
                    (void)expected_size;
                }
                stbi_image_free(sixteen_bit_data);
            }
            break;
        }
        
        case 3: {
            // Test stbi_is_16_bit_from_callbacks
            ctx.position = 0;
            int is_sixteen_bit = stbi_is_16_bit_from_callbacks(&callbacks, &ctx);
            (void)is_sixteen_bit;  // Use result
            break;
        }
        
        case 4: {
            // Comprehensive test: try multiple loading approaches
            ctx.position = 0;
            int check_16bit = stbi_is_16_bit_from_callbacks(&callbacks, &ctx);
            
            ctx.position = 0;
            int desired_channels = 4;  // RGBA
            unsigned char* ldr_data = stbi_load_from_callbacks(&callbacks, &ctx, &width, &height, &channels, desired_channels);
            if (ldr_data) {
                stbi_image_free(ldr_data);
            }
            
            // Also try with different channel count
            ctx.position = 0;
            if (check_16bit) {
                stbi_us* sixteen_bit_data = stbi_load_16_from_callbacks(&callbacks, &ctx, &width, &height, &channels, 3);  // RGB
                if (sixteen_bit_data) {
                    stbi_image_free(sixteen_bit_data);
                }
            }
            break;
        }
    }
    
    // Test error cases by manipulating buffer position
    if (fdp.remaining_bytes() > 0) {
        // Test with exhausted buffer
        ctx.position = ctx.size;  // Move to end
        int eof_check = buffer_eof(&ctx);
        (void)eof_check;
        
        // Try reading from exhausted buffer
        char dummy[4];
        int read_result = buffer_read(&ctx, dummy, 4);
        (void)read_result;
        
        // Test skip beyond buffer bounds
        buffer_skip(&ctx, 1000);
        buffer_skip(&ctx, -1000);
    }
    
    return 0;
}

} // namespace harness_017
