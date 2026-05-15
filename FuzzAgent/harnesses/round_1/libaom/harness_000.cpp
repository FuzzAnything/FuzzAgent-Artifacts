#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"

namespace harness_000 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Comprehensive fuzzing harness for libaom AV1 codec library
 * Targets both decoder and encoder APIs to maximize coverage
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size < 32) {
    return 0;  // Insufficient input for meaningful testing
  }

  FuzzedDataProvider fdp(data, size);

  // Consume operation type: 0 for decoder, 1 for encoder, 2 for both
  uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 3;
  
  // Consume configuration parameters
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
  bool allow_lowbitdepth = fdp.ConsumeBool();
  bool is_annexb = fdp.ConsumeBool();
  bool tile_mode = fdp.ConsumeBool();
  bool ext_tile_debug = fdp.ConsumeBool();
  int output_all_layers = fdp.ConsumeBool() ? 1 : 0;
  int operating_point = fdp.ConsumeIntegralInRange<int>(0, 31);
  
  // Get remaining data for bitstream/frames
  std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (remaining_data.size() < 32) {
    return 0;  // Not enough data for bitstream
  }

  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  
  if (operation_type == 0 || operation_type == 2) {
    // DECODER PATH
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {threads, 0, 0, allow_lowbitdepth ? 1u : 0u};
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) == AOM_CODEC_OK) {
      // Set decoder controls based on fuzzed data
      AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1_SET_TILE_MODE, tile_mode);
      AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_EXT_TILE_DEBUG, ext_tile_debug);
      AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_IS_ANNEXB, is_annexb);
      AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_OUTPUT_ALL_LAYERS, output_all_layers);
      AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_OPERATING_POINT, operating_point);
      
      // Parse stream info
      aom_codec_stream_info_t stream_info;
      memset(&stream_info, 0, sizeof(stream_info));
      stream_info.is_annexb = is_annexb;
      aom_codec_peek_stream_info(decoder_iface, remaining_data.data(), 
                                  remaining_data.size(), &stream_info);
      
      // Try to decode the data
      size_t decode_size = std::min(remaining_data.size(), (size_t)4096);
      aom_codec_err_t decode_err = aom_codec_decode(&decoder, 
                                                     remaining_data.data(), 
                                                     decode_size, 
                                                     nullptr);
      
      // Get decoded frames if any
      if (decode_err == AOM_CODEC_OK) {
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img = nullptr;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
          // Frame successfully decoded, do nothing with it for fuzzing
        }
        
        // Try to get stream info after decoding
        aom_codec_get_stream_info(&decoder, &stream_info);
      }
      
      aom_codec_destroy(&decoder);
    }
  }
  
  if (operation_type == 1 || operation_type == 2) {
    // ENCODER PATH
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
      // Configure encoder with fuzzed parameters
      enc_cfg.g_w = width;
      enc_cfg.g_h = height;
      enc_cfg.g_timebase.num = 1;
      enc_cfg.g_timebase.den = 30;  // 30 fps
      enc_cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
      enc_cfg.g_error_resilient = fdp.ConsumeBool() ? AOM_ERROR_RESILIENT_DEFAULT : 0;
      
      // Initialize encoder
      if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        // Set encoder controls
        int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
        AOM_CODEC_CONTROL_TYPECHECKED(&encoder, AOME_SET_CPUUSED, cpu_used);
        
        int enable_auto_alt_ref = fdp.ConsumeBool() ? 1 : 0;
        AOM_CODEC_CONTROL_TYPECHECKED(&encoder, AOME_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
        
        // Create a test image
        aom_image_t test_img;
        aom_img_alloc(&test_img, AOM_IMG_FMT_I420, width, height, 1);
        
        if (test_img.sz > 0) {
          // Fill with some data from remaining input
          size_t fill_size = std::min(test_img.sz, remaining_data.size());
          if (fill_size > 0 && test_img.planes[0] != nullptr) {
            memcpy(test_img.planes[0], remaining_data.data(), fill_size);
          }
          
          // Encode the frame
          int flags = 0;
          if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_FORCE_KF;  // Force keyframe
          }
          
          aom_codec_err_t encode_err = aom_codec_encode(&encoder, &test_img, 
                                                        0, 1, flags);
          
          if (encode_err == AOM_CODEC_OK) {
            // Get encoded data
            const aom_codec_cx_pkt_t *pkt = nullptr;
            aom_codec_iter_t iter = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
              if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Encoded frame available
              }
            }
          }
          
          // Try to flush encoder
          while (aom_codec_encode(&encoder, nullptr, 0, 1, 0) == AOM_CODEC_OK) {
            // Get remaining packets
            const aom_codec_cx_pkt_t *pkt = nullptr;
            aom_codec_iter_t iter = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
              // Process packets
            }
          }
          
          aom_img_free(&test_img);
        }
        
        aom_codec_destroy(&encoder);
      }
    }
  }
  
  // Test image creation and manipulation APIs
  if (fdp.ConsumeBool() && remaining_data.size() > 100) {
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                  fdp.ConsumeIntegralInRange<unsigned int>(16, 256),
                  fdp.ConsumeIntegralInRange<unsigned int>(16, 256), 
                  1);
    
    if (img.sz > 0) {
      // Test simple image metadata operations
      aom_image_t *img_copy = aom_img_alloc(NULL, AOM_IMG_FMT_I420, img.d_w, img.d_h, 1);
      if (img_copy) {
        // Test image plane dimensions
        int plane_width = aom_img_plane_width(&img, 0);
        int plane_height = aom_img_plane_height(&img, 0);
        
        // Free the copy
        aom_img_free(img_copy);
      }
      
      // Test image flip
      aom_img_flip(&img);
      
      aom_img_free(&img);
    }
  }
  
  return 0;
}

} // namespace harness_000
