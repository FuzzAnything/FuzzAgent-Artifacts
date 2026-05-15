#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_004 {
/*
 * Fuzzing harness for libvpx VP8/VP9 video codec library
 * Target: Decoder callback functionality with interface name testing
 * 
 * This harness specifically targets:
 * 1. vpx_codec_iface_name - Interface name retrieval (0% covered, 2 undiscovered branches)
 * 2. vpx_codec_register_put_slice_cb - Slice completion callback (0% covered, 12 undiscovered branches)
 * 3. vpx_codec_register_put_frame_cb - Frame completion callback (0% covered, 12 undiscovered branches)
 * 
 * Required Helper APIs:
 * - vpx_codec_dec_init_ver - Initialize decoder
 * - vpx_codec_decode - Process compressed data
 * - vpx_codec_get_frame - Retrieve decoded frames
 * - vpx_codec_destroy - Cleanup
 * - vpx_codec_error - Error checking
 * - vpx_codec_error_detail - Detailed error info
 * 
 * Differentiated from harness_002.cpp:
 * - Focuses on vpx_codec_iface_name API testing
 * - Tests callback registration in different sequences
 * - Explores more edge cases for callback functionality
 * - Tests both VP8 and VP9 decoder interfaces thoroughly
 * - Tests error paths with invalid callback registrations
 */




// Put slice callback implementation
static void test_put_slice_cb(void* user_priv, const vpx_image_t* img,
                              const vpx_image_rect_t* valid,
                              const vpx_image_rect_t* update) {
  // Callback triggered during slice decoding
  // Track that callback was invoked for verification
  (void)user_priv;
  (void)img;
  (void)valid;
  (void)update;
  
  static int slice_callback_count = 0;
  slice_callback_count++;
  
  // Test accessing image properties if image is valid
  if (img && img->fmt != VPX_IMG_FMT_NONE) {
    // Validate basic image properties
    (void)img->d_w;
    (void)img->d_h;
    (void)img->stride[0];
  }
}

// Put frame callback implementation
static void test_put_frame_cb(void* user_priv, const vpx_image_t* img) {
  // Callback triggered when frame decoding is complete
  // Track that callback was invoked for verification
  (void)user_priv;
  
  static int frame_callback_count = 0;
  frame_callback_count++;
  
  // Test accessing image properties if image is valid
  if (img && img->fmt != VPX_IMG_FMT_NONE) {
    // Validate image properties
    (void)img->d_w;
    (void)img->d_h;
    (void)img->planes[0];
    
    // Test different image formats
    switch (img->fmt) {
      case VPX_IMG_FMT_I420:
      case VPX_IMG_FMT_I422:
      case VPX_IMG_FMT_I444:
      case VPX_IMG_FMT_YV12:
      case VPX_IMG_FMT_I42016:
      case VPX_IMG_FMT_I42216:
      case VPX_IMG_FMT_I44416:
        // Valid formats
        break;
      default:
        // Unknown format
        break;
    }
  }
}

// Simple implementation of mem_get_le32 for IVF parsing
static uint32_t mem_get_le32(const uint8_t* mem) {
    return ((uint32_t)mem[0]) | ((uint32_t)mem[1] << 8) | 
           ((uint32_t)mem[2] << 16) | ((uint32_t)mem[3] << 24);
}

#define IVF_FRAME_HDR_SZ (4 + 8)  /* 4 byte size + 8 byte timestamp */
#define IVF_FILE_HDR_SZ 32

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Need minimum data for initialization and configuration
  if (size < 128) {
    return 0;
  }
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume configuration parameters from fuzzer input
  uint8_t codec_type = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
  uint8_t callback_test_mode = fdp.ConsumeIntegral<uint8_t>() % 4; // 0-3 test modes
  bool test_interface_name = fdp.ConsumeBool();
  bool test_error_paths = fdp.ConsumeBool();
  bool register_both_callbacks = fdp.ConsumeBool();
  bool test_invalid_registration = fdp.ConsumeBool();
  
  // Thread configuration
  unsigned int threads = (fdp.ConsumeIntegral<uint8_t>() % 4) + 1;
  
  // Choose codec interface based on fuzzed input
  vpx_codec_iface_t* decoder_iface = nullptr;
  if (codec_type == 0) {
    decoder_iface = vpx_codec_vp8_dx();
  } else {
    decoder_iface = vpx_codec_vp9_dx();
  }
  
  if (!decoder_iface) {
    return 0;
  }
  
  // Step 1: Test vpx_codec_iface_name API
  if (test_interface_name) {
    const char* iface_name = vpx_codec_iface_name(decoder_iface);
    
    // Verify interface name is not null and has some content
    if (iface_name && iface_name[0] != '\0') {
      // Check for expected interface names
      if (codec_type == 0) {
        // VP8 decoder interface
        // Interface name should contain "VP8" or "vp8"
      } else {
        // VP9 decoder interface  
        // Interface name should contain "VP9" or "vp9"
      }
    }
  }
  
  // Step 2: Get decoder capabilities
  vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
  
  // Initialize decoder configuration
  vpx_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = threads;
  dec_cfg.w = 0;  // Unknown width
  dec_cfg.h = 0;  // Unknown height
  
  // Initialize decoder context
  vpx_codec_ctx_t decoder;
  vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
      &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
  
  // Step 3: Test error handling APIs on initialization result
  if (init_result != VPX_CODEC_OK) {
    // Test error handling APIs on failed initialization
    const char* error_str = vpx_codec_error(&decoder);
    const char* error_detail = vpx_codec_error_detail(&decoder);
    
    (void)error_str;
    (void)error_detail;
    
    // Early return if initialization failed
    return 0;
  }
  
  // Step 4: Test callback registration based on test mode
  void* callback_priv = nullptr;
  vpx_codec_err_t slice_result = VPX_CODEC_OK;
  vpx_codec_err_t frame_result = VPX_CODEC_OK;
  
  // Different callback registration sequences based on test mode
  switch (callback_test_mode) {
    case 0:
      // Register slice callback only (if supported)
      if ((caps & VPX_CODEC_CAP_PUT_SLICE)) {
        slice_result = vpx_codec_register_put_slice_cb(
            &decoder, test_put_slice_cb, callback_priv);
      }
      break;
      
    case 1:
      // Register frame callback only (if supported)
      if ((caps & VPX_CODEC_CAP_PUT_FRAME)) {
        frame_result = vpx_codec_register_put_frame_cb(
            &decoder, test_put_frame_cb, callback_priv);
      }
      break;
      
    case 2:
      // Register both callbacks (if supported)
      if (register_both_callbacks) {
        if ((caps & VPX_CODEC_CAP_PUT_SLICE)) {
          slice_result = vpx_codec_register_put_slice_cb(
              &decoder, test_put_slice_cb, callback_priv);
        }
        if ((caps & VPX_CODEC_CAP_PUT_FRAME)) {
          frame_result = vpx_codec_register_put_frame_cb(
              &decoder, test_put_frame_cb, callback_priv);
        }
      }
      break;
      
    case 3:
      // Test invalid registration sequences
      if (test_invalid_registration) {
        // Try registering callbacks multiple times (should fail)
        if ((caps & VPX_CODEC_CAP_PUT_SLICE)) {
          slice_result = vpx_codec_register_put_slice_cb(
              &decoder, test_put_slice_cb, callback_priv);
          
          // Try registering again (should return error)
          vpx_codec_err_t duplicate_result = vpx_codec_register_put_slice_cb(
              &decoder, test_put_slice_cb, callback_priv);
          (void)duplicate_result;
        }
      }
      break;
  }
  
  // Step 5: Test error handling for callback registration results
  if (slice_result != VPX_CODEC_OK) {
    const char* slice_error = vpx_codec_error(&decoder);
    const char* slice_error_detail = vpx_codec_error_detail(&decoder);
    (void)slice_error;
    (void)slice_error_detail;
  }
  
  if (frame_result != VPX_CODEC_OK) {
    const char* frame_error = vpx_codec_error(&decoder);
    const char* frame_error_detail = vpx_codec_error_detail(&decoder);
    (void)frame_error;
    (void)frame_error_detail;
  }
  
  // Step 6: Decode with fuzzed input data
  for (int decode_pass = 0; decode_pass < 3; ++decode_pass) {
    // Consume decode data from fuzzer input
    size_t decode_data_size = std::min<size_t>(
        fdp.ConsumeIntegralInRange<size_t>(0, 4096), fdp.remaining_bytes());
    
    if (decode_data_size == 0 && decode_pass > 0) {
      break; // No more data
    }
    
    std::vector<uint8_t> decode_data = fdp.ConsumeBytes<uint8_t>(decode_data_size);
    
    // Choose user_priv based on test scenario
    void* user_priv = nullptr;
    if (callback_test_mode == 1) {
      user_priv = callback_priv; // Use callback private data
    }
    
    // Set deadline (0 = unlimited)
    long deadline = 0;
    
    // Perform decode
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, decode_data.data(), decode_data.size(), user_priv, deadline);
    
    // Step 7: Test error handling APIs after decode
    if (decode_result != VPX_CODEC_OK) {
      const char* decode_error = vpx_codec_error(&decoder);
      const char* decode_error_detail = vpx_codec_error_detail(&decoder);
      
      (void)decode_error;
      (void)decode_error_detail;
    } else {
      // Try to get decoded frames
      vpx_codec_iter_t iter = nullptr;
      vpx_image_t* img;
      while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Frame available for processing
        (void)img;
        
        // Test frame properties
        if (img->fmt != VPX_IMG_FMT_NONE) {
          // Valid frame format - test various properties
          (void)img->d_w;
          (void)img->d_h;
          (void)img->bit_depth;
          (void)img->x_chroma_shift;
          (void)img->y_chroma_shift;
          
          // Test plane access
          for (int plane = 0; plane < 3; ++plane) {
            if (img->planes[plane]) {
              (void)img->stride[plane];
            }
          }
        }
      }
    }
    
    // Step 8: Test error paths if enabled
    if (test_error_paths && decode_pass == 0) {
      // Test with NULL data pointer (should trigger error)
      vpx_codec_err_t null_decode_result = vpx_codec_decode(
          &decoder, nullptr, 0, nullptr, 0);
      
      if (null_decode_result != VPX_CODEC_OK) {
        const char* null_error = vpx_codec_error(&decoder);
        const char* null_error_detail = vpx_codec_error_detail(&decoder);
        (void)null_error;
        (void)null_error_detail;
      }
      
      // Test with zero size but valid pointer
      if (decode_data.size() > 0) {
        vpx_codec_err_t zero_size_result = vpx_codec_decode(
            &decoder, decode_data.data(), 0, nullptr, 0);
        
        if (zero_size_result != VPX_CODEC_OK) {
          const char* zero_size_error = vpx_codec_error(&decoder);
          (void)zero_size_error;
        }
      }
      
      // Test with invalid deadline
      vpx_codec_err_t invalid_deadline_result = vpx_codec_decode(
          &decoder, decode_data.data(), decode_data.size(), nullptr, -1);
      
      if (invalid_deadline_result != VPX_CODEC_OK) {
        const char* deadline_error = vpx_codec_error(&decoder);
        (void)deadline_error;
      }
    }
  }
  
  // Step 9: Test interface name on initialized decoder
  if (test_interface_name) {
    // Get interface name from initialized context
    if (decoder.iface) {
      const char* ctx_iface_name = vpx_codec_iface_name(decoder.iface);
      (void)ctx_iface_name;
    }
    
    // Also test with the original interface pointer
    const char* iface_name_again = vpx_codec_iface_name(decoder_iface);
    (void)iface_name_again;
  }
  
  // Step 10: Cleanup
  vpx_codec_err_t destroy_result = vpx_codec_destroy(&decoder);
  
  // Test error handling on destroy (should succeed)
  if (destroy_result != VPX_CODEC_OK) {
    const char* destroy_error = vpx_codec_err_to_string(destroy_result);
    (void)destroy_error;
  }
  
  return 0;
}

} // namespace harness_004
