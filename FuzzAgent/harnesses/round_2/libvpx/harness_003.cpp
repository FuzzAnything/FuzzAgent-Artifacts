#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_frame_buffer.h"

namespace harness_003 {
/*
 * Fuzzing harness for libvpx VP9 decoder with external frame buffer callbacks
 * Targets: vpx_codec_set_frame_buffer_functions, vpx_codec_register_put_frame_cb,
 *          vpx_codec_get_stream_info, vpx_codec_dec_init_ver
 * Focus: VP9 decoder lifecycle with external frame buffer management and callbacks
 */




// Simple frame buffer pool for external frame buffer callback testing
struct FrameBufferPool {
    std::vector<vpx_codec_frame_buffer_t> buffers;
    size_t next_index;
    
    FrameBufferPool(size_t pool_size) : next_index(0) {
        buffers.resize(pool_size);
        for (auto& fb : buffers) {
            fb.data = nullptr;
            fb.size = 0;
            fb.priv = nullptr;
        }
    }
    
    ~FrameBufferPool() {
        for (auto& fb : buffers) {
            if (fb.data) {
                free(fb.data);
                fb.data = nullptr;
            }
        }
    }
    
    // Get frame buffer callback implementation
    int get_frame_buffer(size_t min_size, vpx_codec_frame_buffer_t* fb) {
        if (next_index >= buffers.size()) {
            next_index = 0; // Wrap around for simplicity
        }
        
        vpx_codec_frame_buffer_t* pool_fb = &buffers[next_index];
        
        // If existing buffer is too small, reallocate
        if (pool_fb->size < min_size) {
            if (pool_fb->data) {
                free(pool_fb->data);
            }
            pool_fb->data = static_cast<uint8_t*>(calloc(1, min_size));
            if (!pool_fb->data) {
                return -1; // Allocation failed
            }
            pool_fb->size = min_size;
        }
        
        // Zero out the buffer as required by spec
        memset(pool_fb->data, 0, pool_fb->size);
        
        // Copy to output
        fb->data = pool_fb->data;
        fb->size = pool_fb->size;
        fb->priv = pool_fb; // Store pointer back to pool entry
        
        next_index++;
        return 0; // Success
    }
    
    // Release frame buffer callback implementation
    int release_frame_buffer(vpx_codec_frame_buffer_t* fb) {
        (void)fb; // In this simple implementation, we don't actually release
                   // The buffers are managed by the pool and reused
        return 0; // Success
    }
};

// Static callback wrappers
static int get_frame_buffer_cb(void* priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferPool* pool = static_cast<FrameBufferPool*>(priv);
    if (!pool) return -1;
    return pool->get_frame_buffer(min_size, fb);
}

static int release_frame_buffer_cb(void* priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferPool* pool = static_cast<FrameBufferPool*>(priv);
    if (!pool) return -1;
    return pool->release_frame_buffer(fb);
}

// Put frame callback implementation
static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    (void)user_priv; // Unused
    (void)img;       // Unused in this simple callback
    // In a real application, this would process the decoded frame
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input: config byte + some data for decoding
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume decoder configuration from fuzzed input
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    unsigned int threads = (config_byte & 0x3f) + 1; // 1-64 threads
    
    // Determine test mode
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>();
    
    // Initialize decoder configuration
    vpx_codec_ctx_t codec;
    vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
    vpx_codec_flags_t flags = 0;
    
    // Set flags based on fuzzed input
    if (config_byte & 0x40) {
        flags |= VPX_CODEC_USE_POSTPROC;
    }
    if (config_byte & 0x80) {
        flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    }
    
    // Create frame buffer pool (size based on fuzzed input, but cap for safety)
    size_t pool_size = fdp.ConsumeIntegralInRange<size_t>(1, 16);
    std::unique_ptr<FrameBufferPool> pool(new FrameBufferPool(pool_size));
    
    // Test 1: Initialize VP9 decoder with vpx_codec_dec_init_ver
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp9_dx(), &cfg, flags, 
                                                 VPX_DECODER_ABI_VERSION);
    
    bool is_vp9 = (err == VPX_CODEC_OK);
    
    // If VP9 fails, try VP8 (but note: external frame buffers only work with VP9)
    if (!is_vp9) {
        err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp8_dx(), &cfg, flags,
                                     VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0; // Both decoders failed
        }
    }
    
    // Test 2: Set external frame buffer functions (only works with VP9)
    if (is_vp9) {
        // This may fail if decoder doesn't support external frame buffers
        vpx_codec_set_frame_buffer_functions(&codec, get_frame_buffer_cb, 
                                            release_frame_buffer_cb, pool.get());
    }
    
    // Test 3: Register put frame callback
    // This requires VPX_CODEC_CAP_PUT_FRAME capability
    vpx_codec_register_put_frame_cb(&codec, put_frame_callback, nullptr);
    
    // Get remaining data for decoding
    std::vector<uint8_t> frame_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (frame_data.size() > 0) {
        // Test 4: Decode the frame
        err = vpx_codec_decode(&codec, frame_data.data(), frame_data.size(), nullptr, 0);
        
        // Test 5: Try to get stream info after decoding
        vpx_codec_stream_info_t stream_info;
        stream_info.sz = sizeof(stream_info);
        vpx_codec_get_stream_info(&codec, &stream_info);
        
        // Test 6: Iterate through decoded frames
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* img = nullptr;
        while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
            // Frame successfully decoded
            // Can access img->d_w, img->d_h, img->planes, etc.
            (void)img; // Avoid unused variable warning
        }
    }
    
    // Test 7: Try peek_stream_info with the same data
    if (frame_data.size() > 0) {
        vpx_codec_stream_info_t peek_info;
        peek_info.sz = sizeof(peek_info);
        vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), frame_data.data(), 
                                   frame_data.size(), &peek_info);
    }
    
    // Test different ABI versions (error paths)
    if (test_mode & 0x01) {
        // Try with wrong ABI version to test error path
        vpx_codec_ctx_t codec2;
        vpx_codec_dec_init_ver(&codec2, vpx_codec_vp9_dx(), &cfg, flags,
                              VPX_DECODER_ABI_VERSION - 1); // Wrong version
        vpx_codec_destroy(&codec2);
    }
    
    // Cleanup
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_003
