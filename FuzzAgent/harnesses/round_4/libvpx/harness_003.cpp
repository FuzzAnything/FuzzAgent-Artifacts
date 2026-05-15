#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"

namespace harness_003 {
/*
 * Fuzzing harness for libvpx error handling, decoder callback, and image management APIs
 * Targets: vpx_codec_error (24 undiscovered branches), vpx_codec_err_to_string (22), 
 *          vpx_codec_error_detail (6), vpx_codec_set_frame_buffer_functions (14),
 *          vpx_codec_register_put_frame_cb (12), vpx_codec_register_put_slice_cb (12),
 *          vpx_img_wrap (10), vpx_codec_get_stream_info (12)
 * Helper APIs: vpx_codec_dec_init_ver, vpx_codec_decode, vpx_codec_destroy
 * Semantic diversity: This harness focuses on error handling paths and decoder callbacks
 *                     (vs. harness_000: basic decoding, harness_001: encoding, 
 *                      harness_002: multi-encoder initialization)
 */



// Callback functions for frame buffer allocation
// We track allocations in a vector to ensure all are freed
struct FrameBufferTracker {
    std::vector<void*> allocations;
    
    void add_allocation(void* ptr) {
        allocations.push_back(ptr);
    }
    
    bool remove_allocation(void* ptr) {
        for (auto it = allocations.begin(); it != allocations.end(); ++it) {
            if (*it == ptr) {
                allocations.erase(it);
                return true;
            }
        }
        return false;
    }
    
    void free_all_remaining() {
        for (void* ptr : allocations) {
            free(ptr);
        }
        allocations.clear();
    }
    
    ~FrameBufferTracker() {
        free_all_remaining();
    }
};

extern "C" {
    // Frame buffer get callback
    int get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                           vpx_codec_frame_buffer_t *fb) {
        // Allocate memory for frame buffer
        uint8_t *buf = (uint8_t*)malloc(min_size);
        if (!buf) return -1;
        
        // Zero out the buffer as required by API documentation
        memset(buf, 0, min_size);
        
        fb->data = buf;
        fb->size = min_size;
        fb->priv = NULL; // Not used in this implementation
        
        // Track allocation in tracker if provided
        if (cb_priv) {
            FrameBufferTracker* tracker = static_cast<FrameBufferTracker*>(cb_priv);
            tracker->add_allocation(buf);
        }
        
        return 0;
    }
    
    // Frame buffer release callback  
    int release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
        if (fb->data) {
            // Remove from tracker if tracker exists
            if (cb_priv) {
                FrameBufferTracker* tracker = static_cast<FrameBufferTracker*>(cb_priv);
                tracker->remove_allocation(fb->data);
            }
            // Free the memory
            free(fb->data);
            fb->data = NULL;
        }
        return 0;
    }
    
    // Put frame callback
    void put_frame_cb(void *user_priv, const vpx_image_t *img) {
        // Callback triggered when frame is available
        // Can be used to process decoded frames
        (void)user_priv;
        (void)img;
    }
    
    // Put slice callback
    void put_slice_cb(void *user_priv, const vpx_image_t *img,
                     const vpx_image_rect_t *valid,
                     const vpx_image_rect_t *update) {
        // Callback triggered when slice is available
        (void)user_priv;
        (void)img;
        (void)valid;
        (void)update;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed: configuration parameters + some encoded data
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Split the input into different testing paths
    int test_path = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // Common configuration parameters
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
    vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
    
    // Choose decoder type: 0 for VP8, 1 for VP9
    int decoder_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Image format selection
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
    vpx_img_fmt_t image_format;
    switch (format_choice) {
        case 0: image_format = VPX_IMG_FMT_YV12; break;
        case 1: image_format = VPX_IMG_FMT_I420; break;
        case 2: image_format = VPX_IMG_FMT_I422; break;
        case 3: image_format = VPX_IMG_FMT_I444; break;
        case 4: image_format = VPX_IMG_FMT_I42016; break;
        default: image_format = VPX_IMG_FMT_I420; break;
    }
    
    // Test different paths based on fuzzer input
    switch (test_path) {
        case 0: {
            // Path 1: Error Handling Path
            // Intentionally trigger initialization failures
            
            vpx_codec_ctx_t codec;
            vpx_codec_err_t err;
            
            // Try to initialize with invalid parameters
            // Use NULL iface or invalid ABI version to trigger errors
            int invalid_ver = fdp.ConsumeIntegralInRange<int>(-10, 10);
            
            if (fdp.ConsumeBool()) {
                // Test with NULL iface
                err = vpx_codec_dec_init_ver(&codec, NULL, &cfg, 0, VPX_DECODER_ABI_VERSION);
            } else {
                // Test with invalid ABI version
                if (decoder_type == 0) {
                    err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp8_dx(), &cfg, 0, invalid_ver);
                } else {
                    err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp9_dx(), &cfg, 0, invalid_ver);
                }
            }
            
            // Test error retrieval functions
            const char *error_str = vpx_codec_error(&codec);
            const char *err_to_str = vpx_codec_err_to_string(err);
            const char *error_detail = vpx_codec_error_detail(&codec);
            
            // Use the error strings (even if NULL) to explore those code paths
            (void)error_str;
            (void)err_to_str;
            (void)error_detail;
            
            // Clean up if initialization partially succeeded
            if (err == VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
            }
            break;
        }
        
        case 1: {
            // Path 2: Decoder Callback Path with vpx_img_wrap
            vpx_codec_ctx_t codec;
            vpx_codec_err_t err;
            
            // Create image buffer for vpx_img_wrap
            // Calculate required buffer size for the image format
            size_t stride_align = 32; // Common alignment
            size_t buffer_size = width * height * 3; // Conservative estimate
            
            // Allocate buffer for image data
            std::vector<uint8_t> img_buffer(buffer_size);
            
            // Create image using vpx_img_wrap
            vpx_image_t img;
            vpx_image_t *wrapped_img = vpx_img_wrap(&img, image_format, width, height, 
                                                   stride_align, img_buffer.data());
            
            if (wrapped_img == NULL) {
                // Try with different parameters
                wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, 
                                          fdp.ConsumeIntegralInRange<unsigned int>(1, 128),
                                          fdp.ConsumeIntegralInRange<unsigned int>(1, 128),
                                          32, img_buffer.data());
            }
            
            // Initialize decoder
            if (decoder_type == 0) {
                err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp8_dx(), &cfg, 0, 
                                           VPX_DECODER_ABI_VERSION);
            } else {
                err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp9_dx(), &cfg, 0, 
                                           VPX_DECODER_ABI_VERSION);
            }
            
            if (err != VPX_CODEC_OK) {
                // Try without config
                if (decoder_type == 0) {
                    err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp8_dx(), NULL, 0, 
                                               VPX_DECODER_ABI_VERSION);
                } else {
                    err = vpx_codec_dec_init_ver(&codec, vpx_codec_vp9_dx(), NULL, 0, 
                                               VPX_DECODER_ABI_VERSION);
                }
                if (err != VPX_CODEC_OK) {
                    return 0;
                }
            }
            
            // Set frame buffer functions if supported
            FrameBufferTracker tracker;
            vpx_codec_set_frame_buffer_functions(&codec, 
                                                get_frame_buffer_cb,
                                                release_frame_buffer_cb,
                                                &tracker);
            
            // Register callbacks
            vpx_codec_register_put_frame_cb(&codec, put_frame_cb, NULL);
            vpx_codec_register_put_slice_cb(&codec, put_slice_cb, NULL);
            
            // Get stream info
            vpx_codec_stream_info_t stream_info;
            stream_info.sz = sizeof(stream_info);
            vpx_codec_get_stream_info(&codec, &stream_info);
            
            // Try to decode some data if available
            size_t remaining_bytes = fdp.remaining_bytes();
            if (remaining_bytes > 0) {
                size_t decode_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                    std::min(remaining_bytes, size_t(65536)));
                std::vector<uint8_t> decode_data = fdp.ConsumeBytes<uint8_t>(decode_size);
                
                vpx_codec_decode(&codec, decode_data.data(), 
                               static_cast<unsigned int>(decode_data.size()), 
                               NULL, 0);
            }
            
            // Cleanup
            vpx_codec_destroy(&codec);
            break;
        }
        
        case 2: {
            // Path 3: Mixed testing with vpx_util module coverage
            
            // Test vpx_img_wrap with various parameters
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 100; i++) {
                unsigned int test_w = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
                unsigned int test_h = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
                unsigned int align = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
                
                // Allocate buffer
                size_t buf_size = test_w * test_h * 3;
                std::vector<uint8_t> test_buffer(buf_size);
                
                vpx_image_t test_img;
                vpx_img_wrap(&test_img, image_format, test_w, test_h, align, 
                           test_buffer.data());
            }
            
            // Test error functions with various scenarios
            vpx_codec_ctx_t codec1, codec2;
            
            // Initialize one decoder successfully
            vpx_codec_err_t err1;
            if (decoder_type == 0) {
                err1 = vpx_codec_dec_init_ver(&codec1, vpx_codec_vp8_dx(), &cfg, 0, 
                                            VPX_DECODER_ABI_VERSION);
            } else {
                err1 = vpx_codec_dec_init_ver(&codec1, vpx_codec_vp9_dx(), &cfg, 0, 
                                            VPX_DECODER_ABI_VERSION);
            }
            
            if (err1 == VPX_CODEC_OK) {
                // Get error info (should be NULL or "No error")
                const char *error1 = vpx_codec_error(&codec1);
                const char *detail1 = vpx_codec_error_detail(&codec1);
                (void)error1;
                (void)detail1;
                
                // Try to get stream info
                vpx_codec_stream_info_t si;
                si.sz = sizeof(si);
                vpx_codec_get_stream_info(&codec1, &si);
                
                vpx_codec_destroy(&codec1);
            }
            
            // Try to initialize second decoder with potentially invalid flags
            vpx_codec_flags_t invalid_flags = fdp.ConsumeIntegral<vpx_codec_flags_t>();
            vpx_codec_err_t err2;
            if (decoder_type == 0) {
                err2 = vpx_codec_dec_init_ver(&codec2, vpx_codec_vp8_dx(), &cfg, 
                                            invalid_flags, VPX_DECODER_ABI_VERSION);
            } else {
                err2 = vpx_codec_dec_init_ver(&codec2, vpx_codec_vp9_dx(), &cfg, 
                                            invalid_flags, VPX_DECODER_ABI_VERSION);
            }
            
            // Test error functions on potentially failed initialization
            if (err2 != VPX_CODEC_OK) {
                const char *error2 = vpx_codec_error(&codec2);
                const char *err_str2 = vpx_codec_err_to_string(err2);
                const char *detail2 = vpx_codec_error_detail(&codec2);
                (void)error2;
                (void)err_str2;
                (void)detail2;
            }
            
            // Clean up if partially initialized
            if (err2 == VPX_CODEC_OK) {
                vpx_codec_destroy(&codec2);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
