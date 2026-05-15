#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"

namespace harness_005 {
/*
 * Fuzzing harness for libaom AV1 decoder memory management APIs
 * Targets uncovered decoder memory management functions identified by CoverageAnalyzerAgent:
 * - aom_codec_dec_init_ver (22 branches, 12 undiscovered)
 * - aom_codec_set_frame_buffer_functions (14 branches, 14 undiscovered)
 * - aom_codec_control (14 branches, 6 undiscovered)
 * Focus on decoder memory management with external frame buffer callbacks
 * Differentiates from existing harnesses by targeting decoder memory management with custom buffer allocation
 */



// Custom frame buffer structure to track allocated buffers
struct CustomFrameBuffer {
    uint8_t* data;
    size_t size;
    bool in_use;
};

// Frame buffer pool for managing custom buffers
class FrameBufferPool {
public:
    FrameBufferPool() = default;
    
    ~FrameBufferPool() {
        for (auto& fb : buffers_) {
            if (fb.data) {
                free(fb.data);
                fb.data = nullptr;
            }
        }
    }
    
    // Get callback function
    static int GetFrameBuffer(void* priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
        FrameBufferPool* pool = static_cast<FrameBufferPool*>(priv);
        if (!pool || !fb) {
            return -1;
        }
        
        // Find an available buffer or allocate a new one
        CustomFrameBuffer* custom_fb = nullptr;
        for (auto& buffer : pool->buffers_) {
            if (!buffer.in_use && buffer.size >= min_size) {
                buffer.in_use = true;
                custom_fb = &buffer;
                break;
            }
        }
        
        // If no suitable buffer found, allocate a new one
        if (!custom_fb) {
            CustomFrameBuffer new_buffer;
            new_buffer.data = static_cast<uint8_t*>(calloc(1, min_size));
            if (!new_buffer.data) {
                return -1;
            }
            new_buffer.size = min_size;
            new_buffer.in_use = true;
            pool->buffers_.push_back(new_buffer);
            custom_fb = &pool->buffers_.back();
        }
        
        // Fill the aom_codec_frame_buffer_t structure
        fb->data = custom_fb->data;
        fb->size = custom_fb->size;
        fb->priv = custom_fb;  // Store pointer for release callback
        
        return 0;
    }
    
    // Release callback function
    static int ReleaseFrameBuffer(void* priv, aom_codec_frame_buffer_t* fb) {
        if (!priv || !fb || !fb->priv) {
            return -1;
        }
        
        CustomFrameBuffer* custom_fb = static_cast<CustomFrameBuffer*>(fb->priv);
        custom_fb->in_use = false;
        
        // Don't free the memory, just mark as available for reuse
        
        return 0;
    }
    
private:
    std::vector<CustomFrameBuffer> buffers_;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    const size_t MIN_INPUT_SIZE = 32;  // Basic params + minimal data
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 64);
    bool allow_lowbitdepth = fdp.ConsumeBool();
    bool use_external_buffers = fdp.ConsumeBool();
    uint8_t num_control_ops = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    
    // Get decoder interface
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }
    
    // Initialize decoder configuration
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {static_cast<unsigned int>(threads), 0, 0, 
                               static_cast<unsigned int>(!allow_lowbitdepth)};
    
    // Step 1: Initialize decoder with aom_codec_dec_init_ver
    aom_codec_flags_t flags = 0;
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 
                                                        flags, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Initialization failed, nothing to test
        return 0;
    }
    
    // Step 2: Set up external frame buffer callbacks if requested
    FrameBufferPool buffer_pool;
    if (use_external_buffers) {
        aom_codec_err_t buffer_result = aom_codec_set_frame_buffer_functions(
            &decoder,
            &FrameBufferPool::GetFrameBuffer,
            &FrameBufferPool::ReleaseFrameBuffer,
            &buffer_pool);
        
        // Continue even if this fails - some decoders may not support external buffers
    }
    
    // Step 3: Perform control operations using aom_codec_control
    for (uint8_t i = 0; i < num_control_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t control_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (control_type) {
            case 0: {
                // AV1_SET_TILE_MODE
                uint8_t tile_mode = fdp.ConsumeBool();
                AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1_SET_TILE_MODE, tile_mode ? 1 : 0);
                break;
            }
            case 1: {
                // AV1D_SET_IS_ANNEXB
                uint8_t is_annexb = fdp.ConsumeBool();
                AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_IS_ANNEXB, is_annexb ? 1 : 0);
                break;
            }
            case 2: {
                // AV1D_SET_OUTPUT_ALL_LAYERS
                uint8_t output_all_layers = fdp.ConsumeBool();
                AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_OUTPUT_ALL_LAYERS, 
                                            output_all_layers ? 1 : 0);
                break;
            }
            case 3: {
                // AV1D_SET_OPERATING_POINT
                uint8_t operating_point = fdp.ConsumeIntegral<uint8_t>() % 32;
                AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_OPERATING_POINT, operating_point);
                break;
            }
            case 4: {
                // AV1D_SET_SKIP_FILM_GRAIN
                uint8_t skip_film_grain = fdp.ConsumeBool();
                AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_SKIP_FILM_GRAIN, 
                                            skip_film_grain ? 1 : 0);
                break;
            }
            case 5: {
                // AV1D_EXT_TILE_DEBUG
                uint8_t ext_tile_debug = fdp.ConsumeBool();
                AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_EXT_TILE_DEBUG, 
                                            ext_tile_debug ? 1 : 0);
                break;
            }
            case 6: {
                // Try to get stream info using control
                aom_codec_stream_info_t stream_info = {0};
                stream_info.is_annexb = fdp.ConsumeBool() ? 1 : 0;
                aom_codec_get_stream_info(&decoder, &stream_info);
                break;
            }
            case 7: {
                // Test with invalid control ID (should fail gracefully)
                int invalid_control = 9999;
                uint8_t dummy_value = fdp.ConsumeIntegral<uint8_t>();
                aom_codec_control(&decoder, invalid_control, dummy_value);
                break;
            }
        }
    }
    
    // Step 4: Decode remaining data (if any)
    std::vector<uint8_t> decode_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (!decode_data.empty()) {
        // Try to peek stream info first
        aom_codec_stream_info_t stream_info = {0};
        stream_info.is_annexb = 0;  // Default to not Annex-B
        
        aom_codec_peek_stream_info(decoder_iface, decode_data.data(), 
                                  decode_data.size(), &stream_info);
        
        // Attempt to decode
        aom_codec_decode(&decoder, decode_data.data(), decode_data.size(), nullptr);
        
        // Try to get decoded frames if available
        aom_codec_iter_t iter = nullptr;
        aom_image_t* img = nullptr;
        int frame_count = 0;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr && frame_count < 5) {
            frame_count++;
            // Don't process frames, just ensure the API is exercised
        }
    }
    
    // Step 5: Test error reporting functions
    if (operation_mode % 2 == 0) {
        // Exercise error reporting APIs
        const char* error_detail = aom_codec_error_detail(&decoder);
        const char* error_string = aom_codec_err_to_string(decoder.err);
        (void)error_detail;  // Mark as used
        (void)error_string;  // Mark as used
    }
    
    // Step 6: Clean up
    aom_codec_destroy(&decoder);
    
    return 0;
}

} // namespace harness_005
