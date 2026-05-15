#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_frame_buffer.h"

namespace harness_003 {
/*
 * Fuzzing harness for libvpx decoder callback and external buffer management APIs
 * Targets: vpx_codec_set_frame_buffer_functions, vpx_codec_register_put_frame_cb, 
 *          vpx_codec_register_put_slice_cb, and related decoder APIs
 * Focuses on VP9 decoder callback mechanisms and external buffer management
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 128;

// Maximum buffer size for external frame buffers
const size_t MAX_BUFFER_SIZE = 1024 * 1024;  // 1MB

// Callback user data structure
typedef struct callback_user_data {
    int frame_count;
    int slice_count;
    bool error_occurred;
} callback_user_data_t;

// Frame callback function
static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    if (user_priv && img) {
        callback_user_data_t* data = (callback_user_data_t*)user_priv;
        data->frame_count++;
        // We could do something with the image here, but for fuzzing we just count
    }
}

// Slice callback function  
static void put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* valid,
                               const vpx_image_rect_t* update) {
    if (user_priv && img) {
        callback_user_data_t* data = (callback_user_data_t*)user_priv;
        data->slice_count++;
        // Count slices, but don't process them extensively for performance
    }
}

// External frame buffer get callback
static int get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t* fb) {
    if (!fb) return -1;
    
    // Allocate buffer with requested minimum size
    size_t alloc_size = std::max(min_size, (size_t)4096);
    alloc_size = std::min(alloc_size, MAX_BUFFER_SIZE);
    
    uint8_t* buffer = (uint8_t*)malloc(alloc_size);
    if (!buffer) return -1;
    
    // Zero out the buffer as required by API
    memset(buffer, 0, alloc_size);
    
    fb->data = buffer;
    fb->size = alloc_size;
    fb->priv = cb_priv;  // Store private data for release callback
    
    return 0;
}

// External frame buffer release callback
static int release_frame_buffer_callback(void* cb_priv,
                                         vpx_codec_frame_buffer_t* fb) {
    if (!fb || !fb->data) return -1;
    
    // Free the buffer
    free(fb->data);
    fb->data = nullptr;
    fb->size = 0;
    
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t codec_choice = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t width_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t height_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t thread_count = (fdp.ConsumeIntegral<uint8_t>() % 8) + 1;
    uint8_t callback_config = fdp.ConsumeIntegral<uint8_t>() % 4;  // Which callbacks to use
    uint8_t use_external_buffers = fdp.ConsumeBool();
    uint8_t use_postproc = fdp.ConsumeBool();
    uint8_t use_error_concealment = fdp.ConsumeBool();
    
    // Map width/height indices to actual dimensions
    unsigned int widths[] = {64, 128, 256, 512};
    unsigned int heights[] = {48, 96, 192, 384};
    unsigned int width = widths[width_idx % 4];
    unsigned int height = heights[height_idx % 4];
    
    // Choose codec interface based on fuzzer input
    const vpx_codec_iface_t* decoder_iface = nullptr;
    
    if (codec_choice == 0) {
        // VP8 decoder
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        // VP9 decoder (supports external frame buffers)
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    if (!decoder_iface) {
        return 0;
    }
    
    // Check if codec supports external frame buffers (VP9 only)
    vpx_codec_caps_t codec_caps = vpx_codec_get_caps(decoder_iface);
    bool supports_external_buffers = (codec_caps & VPX_CODEC_CAP_EXTERNAL_FRAME_BUFFER) != 0;
    bool supports_put_frame = (codec_caps & VPX_CODEC_CAP_PUT_FRAME) != 0;
    bool supports_put_slice = (codec_caps & VPX_CODEC_CAP_PUT_SLICE) != 0;
    
    // Adjust configuration based on codec capabilities
    if (use_external_buffers && !supports_external_buffers) {
        use_external_buffers = false;
    }
    if ((callback_config & 0x01) && !supports_put_frame) {
        callback_config &= ~0x01;  // Clear frame callback bit
    }
    if ((callback_config & 0x02) && !supports_put_slice) {
        callback_config &= ~0x02;  // Clear slice callback bit
    }
    
    // Initialize decoder configuration
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t cfg = {thread_count, 0, 0};
    vpx_codec_flags_t flags = 0;
    
    if (use_postproc) {
        flags |= VPX_CODEC_USE_POSTPROC;
    }
    if (use_error_concealment) {
        flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    }
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, flags);
    if (err != VPX_CODEC_OK) {
        // Try without flags if initialization fails
        flags = 0;
        err = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, flags);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Setup callback user data
    callback_user_data_t user_data = {0, 0, false};
    callback_user_data_t* cb_user_data = &user_data;
    
    // Register callbacks based on configuration
    if (callback_config & 0x01) {
        // Register frame callback
        err = vpx_codec_register_put_frame_cb(&decoder, put_frame_callback, cb_user_data);
        // Don't fail if callback registration fails - some codecs may not support it
    }
    
    if (callback_config & 0x02) {
        // Register slice callback
        err = vpx_codec_register_put_slice_cb(&decoder, put_slice_callback, cb_user_data);
        // Don't fail if callback registration fails
    }
    
    // Setup external frame buffers if requested and supported
    if (use_external_buffers) {
        // Setup callback user data for frame buffers
        void* fb_user_data = nullptr;  // Could be used to track allocations
        
        err = vpx_codec_set_frame_buffer_functions(&decoder,
                                                   get_frame_buffer_callback,
                                                   release_frame_buffer_callback,
                                                   fb_user_data);
        // If external buffer setup fails, we can still continue with internal buffers
    }
    
    // Consume remaining data as compressed video frames for decoding
    // We'll simulate decoding by feeding random data to the decoder
    // This will exercise the callback paths and buffer management
    
    size_t frames_to_decode = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t frame_idx = 0; frame_idx < frames_to_decode && fdp.remaining_bytes() > 0; frame_idx++) {
        // Determine maximum frame size based on remaining bytes after consuming the frame size value
        size_t max_frame_size = std::min((size_t)4096, fdp.remaining_bytes());
        if (max_frame_size == 0) break;
        
        // Consume frame size (this also consumes bytes from input)
        size_t requested_frame_size = fdp.ConsumeIntegralInRange<size_t>(1, max_frame_size);
        
        // Now consume the actual frame data - ConsumeBytes may return fewer bytes than requested
        std::vector<uint8_t> frame_data = fdp.ConsumeBytes<uint8_t>(requested_frame_size);
        
        // Use actual size returned by ConsumeBytes
        size_t actual_frame_size = frame_data.size();
        if (actual_frame_size == 0) break;
        
        // Attempt to decode the frame (will likely fail since it's random data,
        // but will exercise API paths and callbacks)
        err = vpx_codec_decode(&decoder, frame_data.data(), actual_frame_size, nullptr, 0);
        
        // Try to get decoded frames (will trigger callbacks if registered)
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* img = nullptr;
        
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Frame callback should have been triggered already if registered
            // We could process the image here, but for fuzzing we just want to
            // exercise the code paths
        }
        
        // Also try peek stream info on the frame data
        vpx_codec_stream_info_t stream_info;
        stream_info.sz = sizeof(stream_info);
        vpx_codec_peek_stream_info(decoder_iface, frame_data.data(), actual_frame_size, &stream_info);
        
        // Try get stream info from decoder context
        vpx_codec_get_stream_info(&decoder, &stream_info);
    }
    
    // Test decoder control APIs that might be relevant
    if (codec_choice == 1) {  // VP9 specific controls
        int enable = fdp.ConsumeBool() ? 1 : 0;
        vpx_codec_control(&decoder, VP9D_SET_LOOP_FILTER_OPT, enable);
        
        enable = fdp.ConsumeBool() ? 1 : 0;
        vpx_codec_control(&decoder, VP9D_SET_ROW_MT, enable);
        
        enable = fdp.ConsumeBool() ? 1 : 0;
        vpx_codec_control(&decoder, VP9_INVERT_TILE_DECODE_ORDER, enable);
    } else if (codec_choice == 0) {  // VP8 specific controls
        if (use_postproc) {
            vp8_postproc_cfg_t pp = {0, 0, 0};
            pp.post_proc_flag = fdp.ConsumeIntegral<uint8_t>() % 7;
            pp.deblocking_level = fdp.ConsumeIntegral<uint8_t>() % 16;
            pp.noise_level = fdp.ConsumeIntegral<uint8_t>() % 16;
            vpx_codec_control(&decoder, VP8_SET_POSTPROC, &pp);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    
    return 0;
}

} // namespace harness_003
