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

namespace harness_006 {
/*
 * Fuzzing harness for libvpx VP8/VP9 video codec library
 * Target: Decoder callback operations with comprehensive control API testing
 * 
 * This harness specifically targets:
 * 1. vpx_codec_register_put_frame_cb (0% covered, 12 undiscovered branches)
 * 2. vpx_codec_register_put_slice_cb (0% covered, 12 undiscovered branches)  
 * 3. vpx_codec_control_ (0% covered, 8 undiscovered branches)
 * 
 * Required Helper APIs:
 * - vpx_codec_dec_init_ver - Initialize decoder
 * - vpx_codec_decode - Process compressed data
 * - vpx_codec_get_frame - Retrieve decoded frames
 * - vpx_codec_destroy - Cleanup
 * - vpx_codec_vp8_dx or vpx_codec_vp9_dx - Codec interface
 * 
 * Differentiated from existing harnesses:
 * - harness_002.cpp: Advanced decoder callback and error handling APIs
 * - harness_004.cpp: Decoder callback functionality with interface name testing
 * - This harness focuses on vpx_codec_control_ API with comprehensive testing
 *   of various control IDs and their parameter types
 * 
 * Testing strategy:
 * 1. Initialize decoder with VP8 or VP9 interface
 * 2. Register frame and slice callbacks
 * 3. Test vpx_codec_control_ with various control IDs and data types
 * 4. Decode input data to trigger callbacks
 * 5. Retrieve frames and test additional controls
 * 6. Cleanup resources
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
}

// Put frame callback implementation  
static void test_put_frame_cb(void* user_priv, const vpx_image_t* img) {
  // Callback triggered when frame decoding is complete
  // Track that callback was invoked for verification
  (void)user_priv;
  (void)img;
  
  static int frame_callback_count = 0;
  frame_callback_count++;
}

// Simple IVF frame header parsing
static const uint8_t* parse_ivf_frame(const uint8_t* data, size_t size, 
                                      size_t* frame_size) {
  if (size < 12) return nullptr;
  
  // IVF frame header: 4 bytes frame size + 8 bytes timestamp
  *frame_size = (size_t)data[0] | ((size_t)data[1] << 8) | 
                ((size_t)data[2] << 16) | ((size_t)data[3] << 24);
  
  // Validate frame size
  if (*frame_size == 0 || *frame_size > size - 12) {
    return nullptr;
  }
  
  return data + 12; // Skip IVF frame header
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size for meaningful testing
  if (size < 64) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Choose decoder interface (VP8 or VP9)
  int decoder_choice = fdp.ConsumeIntegralInRange(0, 1);
  vpx_codec_iface_t* decoder_iface = nullptr;
  
  if (decoder_choice == 0) {
    decoder_iface = vpx_codec_vp8_dx();
  } else {
    decoder_iface = vpx_codec_vp9_dx();
  }
  
  if (!decoder_iface) return 0;
  
  // Initialize decoder configuration
  vpx_codec_ctx_t decoder;
  vpx_codec_dec_cfg_t cfg = {0};
  cfg.threads = fdp.ConsumeIntegralInRange(1, 4);
  cfg.w = fdp.ConsumeIntegralInRange(16, 1920);
  cfg.h = fdp.ConsumeIntegralInRange(16, 1080);
  
  // Initialize decoder with version
  vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &cfg, 0, 
                                               VPX_DECODER_ABI_VERSION);
  if (res != VPX_CODEC_OK) {
    return 0;
  }
  
  // Register frame callback
  res = vpx_codec_register_put_frame_cb(&decoder, test_put_frame_cb, nullptr);
  // Note: Some decoders may not support callbacks, so we continue even if this fails
  
  // Register slice callback  
  res = vpx_codec_register_put_slice_cb(&decoder, test_put_slice_cb, nullptr);
  // Note: Some decoders may not support callbacks, so we continue even if this fails
  
  // Test vpx_codec_control_ with various control IDs
  // Use fuzzed data to select which controls to test
  
  // First, test controls that take integer pointers
  if (fdp.ConsumeBool()) {
    int last_ref_updates = 0;
    res = vpx_codec_control_(&decoder, VP8D_GET_LAST_REF_UPDATES, &last_ref_updates);
    // Result may be error for uninitialized decoder, that's OK for fuzzing
  }
  
  if (fdp.ConsumeBool()) {
    int frame_corrupted = 0;
    res = vpx_codec_control_(&decoder, VP8D_GET_FRAME_CORRUPTED, &frame_corrupted);
    // Result may be error for uninitialized decoder, that's OK for fuzzing
  }
  
  if (fdp.ConsumeBool()) {
    int last_ref_used = 0;
    res = vpx_codec_control_(&decoder, VP8D_GET_LAST_REF_USED, &last_ref_used);
    // Result may be error for uninitialized decoder, that's OK for fuzzing
  }
  
  // Test VP9-specific controls if using VP9 decoder
  if (decoder_choice == 1) {
    if (fdp.ConsumeBool()) {
      int frame_size[2] = {0, 0};
      res = vpx_codec_control_(&decoder, VP9D_GET_FRAME_SIZE, frame_size);
    }
    
    if (fdp.ConsumeBool()) {
      int display_size[2] = {0, 0};
      res = vpx_codec_control_(&decoder, VP9D_GET_DISPLAY_SIZE, display_size);
    }
    
    if (fdp.ConsumeBool()) {
      unsigned int bit_depth = 0;
      res = vpx_codec_control_(&decoder, VP9D_GET_BIT_DEPTH, &bit_depth);
    }
    
    if (fdp.ConsumeBool()) {
      int byte_alignment = fdp.ConsumeIntegralInRange(0, 32);
      res = vpx_codec_control_(&decoder, VP9_SET_BYTE_ALIGNMENT, byte_alignment);
    }
    
    if (fdp.ConsumeBool()) {
      int invert_tile_order = fdp.ConsumeBool();
      res = vpx_codec_control_(&decoder, VP9_INVERT_TILE_DECODE_ORDER, invert_tile_order);
    }
    
    if (fdp.ConsumeBool()) {
      int skip_loop_filter = fdp.ConsumeBool();
      res = vpx_codec_control_(&decoder, VP9_SET_SKIP_LOOP_FILTER, skip_loop_filter);
    }
    
    if (fdp.ConsumeBool()) {
      int spatial_layer = fdp.ConsumeIntegralInRange(0, 3);
      res = vpx_codec_control_(&decoder, VP9_DECODE_SVC_SPATIAL_LAYER, spatial_layer);
    }
    
    if (fdp.ConsumeBool()) {
      int last_quantizer = 0;
      res = vpx_codec_control_(&decoder, VPXD_GET_LAST_QUANTIZER, &last_quantizer);
    }
    
    if (fdp.ConsumeBool()) {
      int row_mt = fdp.ConsumeBool();
      res = vpx_codec_control_(&decoder, VP9D_SET_ROW_MT, row_mt);
    }
    
    if (fdp.ConsumeBool()) {
      int loop_filter_opt = fdp.ConsumeBool();
      res = vpx_codec_control_(&decoder, VP9D_SET_LOOP_FILTER_OPT, loop_filter_opt);
    }
  }
  
  // Test VP8 post-processing controls
  if (fdp.ConsumeBool()) {
    vp8_postproc_cfg_t pp_cfg;
    pp_cfg.post_proc_flag = fdp.ConsumeIntegralInRange(0, 15);
    pp_cfg.deblocking_level = fdp.ConsumeIntegralInRange(0, 16);
    pp_cfg.noise_level = fdp.ConsumeIntegralInRange(0, 16);
    res = vpx_codec_control_(&decoder, VP8_SET_POSTPROC, &pp_cfg);
  }
  
  // Get remaining bytes for decode testing
  std::vector<uint8_t> decode_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (decode_data.size() < 32) {
    vpx_codec_destroy(&decoder);
    return 0;
  }
  
  // Try to decode the data (may fail, that's OK for fuzzing)
  res = vpx_codec_decode(&decoder, decode_data.data(), decode_data.size(), 
                        nullptr, 0);
  
  // After decoding, test controls that might now return valid data
  if (fdp.ConsumeBool() && res == VPX_CODEC_OK) {
    int last_ref_updates = 0;
    vpx_codec_control_(&decoder, VP8D_GET_LAST_REF_UPDATES, &last_ref_updates);
  }
  
  if (fdp.ConsumeBool() && res == VPX_CODEC_OK) {
    int frame_corrupted = 0;
    vpx_codec_control_(&decoder, VP8D_GET_FRAME_CORRUPTED, &frame_corrupted);
  }
  
  // Get decoded frames if available
  if (res == VPX_CODEC_OK) {
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* img;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
      // Frame retrieved, test additional controls with valid frame context
      if (fdp.ConsumeBool()) {
        int last_ref_used = 0;
        vpx_codec_control_(&decoder, VP8D_GET_LAST_REF_USED, &last_ref_used);
      }
      
      // Test getting frame properties
      if (decoder_choice == 1 && fdp.ConsumeBool()) {
        int frame_size[2] = {0, 0};
        vpx_codec_control_(&decoder, VP9D_GET_FRAME_SIZE, frame_size);
      }
    }
  }
  
  // Test error handling controls
  if (fdp.ConsumeBool()) {
    const char* error_str = vpx_codec_error(&decoder);
    (void)error_str;
    
    const char* error_detail = vpx_codec_error_detail(&decoder);
    (void)error_detail;
  }
  
  // Cleanup
  vpx_codec_destroy(&decoder);
  
  return 0;
}

} // namespace harness_006
