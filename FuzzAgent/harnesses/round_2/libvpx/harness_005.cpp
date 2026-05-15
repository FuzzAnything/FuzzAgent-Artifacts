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

namespace harness_005 {
/*
 * Fuzzing harness for libvpx decoder slice callback registration APIs
 * Targets: vpx_codec_register_put_slice_cb (Primary: 12 undiscovered branches, 0% coverage)
 * Required helpers: vpx_codec_dec_init_ver, vpx_codec_decode, vpx_codec_get_frame, vpx_codec_destroy
 * Optional uncovered APIs: vpx_img_wrap, vpx_codec_error_detail, vpx_codec_iface_name, vpx_codec_get_caps
 * Focus: Testing decoder slice callback registration and lifecycle for both VP8 and VP9 codecs
 * Semantic differentiation: First harness targeting slice callbacks (distinct from frame callbacks in harness_003)
 */




// Slice callback implementation
static void put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* valid,
                               const vpx_image_rect_t* update) {
    (void)user_priv;  // Unused in this simple callback
    (void)img;        // Unused in this simple callback
    (void)valid;      // Unused in this simple callback
    (void)update;     // Unused in this simple callback
    
    // In a real application, this callback would process partially decoded slices
    // The callback receives:
    // - img: The partially decoded image
    // - valid: The valid rectangle within the image
    // - update: The updated rectangle within the image
}

// Image wrapper callback for testing vpx_img_wrap
static vpx_image_t* create_test_image(FuzzedDataProvider& fdp) {
    // Consume image parameters from fuzzed input
    vpx_img_fmt_t fmt = static_cast<vpx_img_fmt_t>(
        fdp.ConsumeIntegralInRange<int>(VPX_IMG_FMT_NONE, VPX_IMG_FMT_HIGHBITDEPTH));
    
    // Limit dimensions for safety
    unsigned int d_w = fdp.ConsumeIntegralInRange<unsigned int>(1, 1024);
    unsigned int d_h = fdp.ConsumeIntegralInRange<unsigned int>(1, 1024);
    
    // Create buffer for image data
    size_t buffer_size = d_w * d_h * 3; // Approximate size for YUV formats
    std::vector<unsigned char> buffer(buffer_size);
    
    // Fill buffer with fuzzed data
    std::vector<uint8_t> fuzzed_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    if (fuzzed_buffer.size() < buffer_size) {
        return nullptr; // Not enough data
    }
    memcpy(buffer.data(), fuzzed_buffer.data(), buffer_size);
    
    // Create image using vpx_img_wrap
    vpx_image_t* img = vpx_img_wrap(nullptr, fmt, d_w, d_h, 1, buffer.data());
    
    return img;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size needed: configuration + some data for decoding
    const size_t MIN_SIZE = 32;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration from fuzzed input
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Determine which codec to test (VP8 or VP9)
    bool test_vp8 = (config_byte & 0x01) != 0;
    bool test_vp9 = (config_byte & 0x02) != 0;
    
    // Determine test mode from configuration
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>();
    
    // Initialize decoder configuration
    vpx_codec_ctx_t codec;
    unsigned int threads = ((config_byte >> 2) & 0x3f) + 1; // 1-64 threads
    vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
    
    // Set decoder flags based on fuzzed input
    vpx_codec_flags_t flags = 0;
    if (config_byte & 0x40) {
        flags |= VPX_CODEC_USE_POSTPROC;
    }
    if (config_byte & 0x80) {
        flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    }
    
    vpx_codec_iface_t* codec_iface = nullptr;
    vpx_codec_err_t err = VPX_CODEC_ERROR;
    
    // Test 1: Try to initialize decoder with specified codec
    if (test_vp8) {
        codec_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&codec, codec_iface, &cfg, flags, 
                                     VPX_DECODER_ABI_VERSION);
    }
    
    // If VP8 failed or we're testing VP9, try VP9
    if ((err != VPX_CODEC_OK && test_vp9) || (!test_vp8 && test_vp9)) {
        codec_iface = vpx_codec_vp9_dx();
        err = vpx_codec_dec_init_ver(&codec, codec_iface, &cfg, flags,
                                     VPX_DECODER_ABI_VERSION);
    }
    
    // If both failed, return early
    if (err != VPX_CODEC_OK) {
        // Test error handling APIs even on failure
        if (test_mode & 0x01) {
            // Test vpx_codec_error_detail
            const char* error_detail = vpx_codec_error_detail(&codec);
            (void)error_detail; // Mark as used
        }
        return 0;
    }
    
    // Test 2: Test codec interface name API
    if (test_mode & 0x02) {
        const char* iface_name = vpx_codec_iface_name(codec_iface);
        (void)iface_name; // Mark as used
    }
    
    // Test 3: Test codec capabilities API
    if (test_mode & 0x04) {
        int caps = vpx_codec_get_caps(codec_iface);
        (void)caps; // Mark as used
    }
    
    // Test 4: Register slice callback
    // Note: This requires VPX_CODEC_CAP_PUT_SLICE capability
    vpx_codec_err_t slice_err = vpx_codec_register_put_slice_cb(&codec, 
                                                               put_slice_callback, 
                                                               nullptr);
    
    // Test error handling for slice registration
    if (slice_err != VPX_CODEC_OK) {
        // Expected for codecs without slice callback capability
        if (test_mode & 0x08) {
            const char* error_detail = vpx_codec_error_detail(&codec);
            (void)error_detail; // Mark as used
        }
    }
    
    // Test 5: Create test image using vpx_img_wrap
    vpx_image_t* test_img = nullptr;
    if (test_mode & 0x10 && fdp.remaining_bytes() > 1024) {
        test_img = create_test_image(fdp);
    }
    
    // Test 6: Decode remaining data as compressed video frame
    std::vector<uint8_t> frame_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (frame_data.size() > 0) {
        err = vpx_codec_decode(&codec, frame_data.data(), frame_data.size(), 
                              nullptr, 0);
        
        // Test error handling after decode
        if (err != VPX_CODEC_OK && (test_mode & 0x20)) {
            const char* error_detail = vpx_codec_error_detail(&codec);
            (void)error_detail; // Mark as used
        }
        
        // Test 7: Try to get decoded frames (iteratively)
        if (err == VPX_CODEC_OK || err == VPX_CODEC_UNSUP_BITSTREAM) {
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t* img = nullptr;
            
            // Iterate through all available decoded frames
            while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
                // Frame successfully decoded
                // Can access img->d_w, img->d_h, img->planes, etc.
                (void)img; // Avoid unused variable warning
                
                // If we have a test image, compare properties (simplified)
                if (test_img) {
                    // Simple property comparison (not actual image comparison)
                    (void)test_img->fmt;
                    (void)test_img->d_w;
                    (void)test_img->d_h;
                }
            }
        }
    }
    
    // Test 8: Clean up test image if created
    if (test_img) {
        vpx_img_free(test_img);
    }
    
    // Test 9: Destroy decoder context
    err = vpx_codec_destroy(&codec);
    
    // Final error handling test
    if (err != VPX_CODEC_OK && (test_mode & 0x40)) {
        const char* error_detail = vpx_codec_error_detail(&codec);
        (void)error_detail; // Mark as used
    }
    
    return 0;
}

} // namespace harness_005
