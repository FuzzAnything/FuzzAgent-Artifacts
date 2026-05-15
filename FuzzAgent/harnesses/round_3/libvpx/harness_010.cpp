#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"

namespace harness_010 {
/*
 * Fuzzing harness for libvpx decoder callback registration APIs
 * Targets: vpx_codec_register_put_frame_cb and vpx_codec_register_put_slice_cb
 * 
 * This harness focuses specifically on testing the callback registration
 * mechanisms for VP8 and VP9 decoders, which currently have 0% coverage
 * despite being reached by previous fuzzers.
 * 
 * Testing strategy:
 * 1. Initialize VP8 and VP9 decoder instances
 * 2. Implement minimal callback functions for frame and slice output
 * 3. Test callback registration with various timing patterns:
 *    - Before decoding any data
 *    - After decoder initialization but before decode
 *    - Multiple registrations and re-registrations
 *    - Error cases (NULL parameters, uninitialized contexts)
 * 4. Decode minimal valid VP8/VP9 test data to trigger callbacks
 * 5. Test error handling and API boundary conditions
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 32;

// Callback user data structure
typedef struct callback_test_data {
    int frame_callback_count;
    int slice_callback_count;
    int last_callback_type;  // 0=frame, 1=slice
    bool callback_triggered;
    void* user_data_ptr;
} callback_test_data_t;

// Frame callback function (minimal implementation)
static void test_frame_callback(void* user_priv, const vpx_image_t* img) {
    if (user_priv && img) {
        callback_test_data_t* data = (callback_test_data_t*)user_priv;
        data->frame_callback_count++;
        data->last_callback_type = 0;
        data->callback_triggered = true;
        
        // Minimal validation of image data
        (void)img->d_w;
        (void)img->d_h;
        (void)img->fmt;
    }
}

// Slice callback function (minimal implementation)
static void test_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* valid,
                               const vpx_image_rect_t* update) {
    if (user_priv && img) {
        callback_test_data_t* data = (callback_test_data_t*)user_priv;
        data->slice_callback_count++;
        data->last_callback_type = 1;
        data->callback_triggered = true;
        
        // Minimal validation of parameters
        (void)img->d_w;
        (void)img->d_h;
        (void)valid;
        (void)update;
    }
}

// Helper function to generate minimal VP8 test frame from fuzzer input
static std::vector<uint8_t> generate_vp8_test_frame(FuzzedDataProvider* fdp) {
    std::vector<uint8_t> frame;
    
    // Minimal VP8 keyframe header for 64x48
    frame.push_back(0x9d);  // keyframe marker
    frame.push_back(0x01);  // width low byte (64 = 0x40)
    frame.push_back(0x2a);  // height (48 = 0x30)
    
    // Add some frame data from fuzzer input
    size_t data_size = fdp->ConsumeIntegralInRange<size_t>(1, 64);
    std::vector<uint8_t> frame_data = fdp->ConsumeBytes<uint8_t>(data_size);
    frame.insert(frame.end(), frame_data.begin(), frame_data.end());
    
    return frame;
}

// Helper function to generate minimal VP9 test frame from fuzzer input
static std::vector<uint8_t> generate_vp9_test_frame(FuzzedDataProvider* fdp) {
    std::vector<uint8_t> frame;
    
    // Minimal VP9 frame marker
    frame.push_back(0x85);  // VP9 frame marker
    frame.push_back(0x00);  // reserved
    frame.push_back(0x00);  // reserved
    frame.push_back(0x00);  // reserved
    frame.push_back(0x00);  // reserved
    
    // Add some frame data from fuzzer input
    size_t data_size = fdp->ConsumeIntegralInRange<size_t>(1, 64);
    std::vector<uint8_t> frame_data = fdp->ConsumeBytes<uint8_t>(data_size);
    frame.insert(frame.end(), frame_data.begin(), frame_data.end());
    
    return frame;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration from fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t codec_choice = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t registration_timing = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t error_testing = fdp.ConsumeBool();
    uint8_t multiple_registrations = fdp.ConsumeBool();
    
    // Initialize callback test data
    callback_test_data_t callback_data = {0};
    callback_data.user_data_ptr = &callback_data;
    
    // Choose codec interface based on fuzzer input
    const vpx_codec_iface_t *decoder_iface = nullptr;
    
    if (codec_choice == 0) {
        // VP8
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        // VP9
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    if (!decoder_iface) {
        return 0;
    }
    
    // Test different callback registration scenarios
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_result;
    
    // Scenario 1: Test error cases before initialization
    if (error_testing) {
        // Test registration on uninitialized context
        vpx_codec_ctx_t uninit_ctx;
        memset(&uninit_ctx, 0, sizeof(uninit_ctx));
        
        vpx_codec_err_t err = vpx_codec_register_put_frame_cb(
            &uninit_ctx, test_frame_callback, &callback_data);
        // Error expected - don't check result
        
        err = vpx_codec_register_put_slice_cb(
            &uninit_ctx, test_slice_callback, &callback_data);
        // Error expected - don't check result
        
        // Test registration with NULL parameters
        err = vpx_codec_register_put_frame_cb(nullptr, test_frame_callback, &callback_data);
        err = vpx_codec_register_put_frame_cb(&uninit_ctx, nullptr, &callback_data);
        
        err = vpx_codec_register_put_slice_cb(nullptr, test_slice_callback, &callback_data);
        err = vpx_codec_register_put_slice_cb(&uninit_ctx, nullptr, &callback_data);
    }
    
    // Initialize decoder
    init_result = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
    
    if (init_result != VPX_CODEC_OK) {
        // If initialization fails, try with different flags
        init_result = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 
                                        VPX_CODEC_USE_ERROR_CONCEALMENT);
        
        if (init_result != VPX_CODEC_OK) {
            return 0; // Can't proceed without decoder
        }
    }
    
    // Test callback registration based on timing pattern
    switch (registration_timing) {
        case 0:
            // Register callbacks BEFORE any decode
            vpx_codec_register_put_frame_cb(&decoder, test_frame_callback, &callback_data);
            vpx_codec_register_put_slice_cb(&decoder, test_slice_callback, &callback_data);
            break;
            
        case 1:
            // Register only frame callback
            vpx_codec_register_put_frame_cb(&decoder, test_frame_callback, &callback_data);
            break;
            
        case 2:
            // Register only slice callback
            vpx_codec_register_put_slice_cb(&decoder, test_slice_callback, &callback_data);
            break;
    }
    
    // Test multiple registrations if requested
    if (multiple_registrations) {
        // Register additional callbacks with different user data
        callback_test_data_t extra_data = {0};
        extra_data.user_data_ptr = &extra_data;
        
        vpx_codec_register_put_frame_cb(&decoder, test_frame_callback, &extra_data);
        vpx_codec_register_put_slice_cb(&decoder, test_slice_callback, &extra_data);
        
        // Re-register with original data
        vpx_codec_register_put_frame_cb(&decoder, test_frame_callback, &callback_data);
    }
    
    // Decode test frame to potentially trigger callbacks
    if (test_scenario < 4) {
        // Generate test frame based on codec choice
        std::vector<uint8_t> test_frame;
        if (codec_choice == 0) {
            test_frame = generate_vp8_test_frame(&fdp);
        } else {
            test_frame = generate_vp9_test_frame(&fdp);
        }
        
        if (!test_frame.empty()) {
            // Normal decode
            vpx_codec_decode(&decoder, test_frame.data(), 
                            static_cast<unsigned int>(test_frame.size()),
                            nullptr, 0);
            
            // Try to get frames to ensure decode completed
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t* decoded_img;
            
            // This may trigger callbacks if they're registered and codec supports them
            while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // Minimal processing
                (void)decoded_img;
            }
        }
    } else if (test_scenario < 6) {
        // Decode with zero size (error case)
        vpx_codec_decode(&decoder, nullptr, 0, nullptr, 0);
        
        // Decode with NULL data but non-zero size (error case)
        vpx_codec_decode(&decoder, nullptr, 1, nullptr, 0);
    } else {
        // Decode multiple times with generated frame
        std::vector<uint8_t> test_frame;
        if (codec_choice == 0) {
            test_frame = generate_vp8_test_frame(&fdp);
        } else {
            test_frame = generate_vp9_test_frame(&fdp);
        }
        
        if (!test_frame.empty()) {
            for (int i = 0; i < 2; i++) {
                vpx_codec_decode(&decoder, test_frame.data(), 
                                static_cast<unsigned int>(test_frame.size()),
                                nullptr, 0);
                
                vpx_codec_iter_t iter = nullptr;
                while (vpx_codec_get_frame(&decoder, &iter) != nullptr) {
                    // Drain frames
                }
            }
        }
    }
    
    // Test late callback registration (after decode)
    if (registration_timing == 0 && test_scenario % 2 == 0 && fdp.remaining_bytes() > 16) {
        callback_test_data_t late_data = {0};
        late_data.user_data_ptr = &late_data;
        
        vpx_codec_register_put_frame_cb(&decoder, test_frame_callback, &late_data);
        
        // Generate a new test frame for late callback registration test
        std::vector<uint8_t> late_test_frame;
        if (codec_choice == 0) {
            late_test_frame = generate_vp8_test_frame(&fdp);
        } else {
            late_test_frame = generate_vp9_test_frame(&fdp);
        }
        
        if (!late_test_frame.empty()) {
            // Decode again with late-registered callback
            vpx_codec_decode(&decoder, late_test_frame.data(), 
                            static_cast<unsigned int>(late_test_frame.size()),
                            nullptr, 0);
            
            vpx_codec_iter_t iter = nullptr;
            while (vpx_codec_get_frame(&decoder, &iter) != nullptr) {
                // Drain frames
            }
        }
    }
    
    // Test vpx_codec_iface_name API (also uncovered according to guidance)
    if (fdp.ConsumeBool()) {
        const char* iface_name = vpx_codec_iface_name(decoder_iface);
        (void)iface_name; // Use result to avoid unused variable warning
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    
    return 0;
}

} // namespace harness_010
