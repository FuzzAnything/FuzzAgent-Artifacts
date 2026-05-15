#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

namespace harness_001 {
/*
 * Fuzzing harness for libaom AV1 decoder
 * Targets core decoder APIs to complement existing encoder fuzzer
 * Follows coverage guidance for comprehensive decoder testing
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for meaningful decoder testing
  if (size < 64) {
    return 0;  // Need enough data for basic decoder initialization
  }

  FuzzedDataProvider fdp(data, size);

  // Consume decoder configuration parameters from fuzzer input
  uint32_t thread_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);  // Threads [1, 64]
  uint32_t operating_point = fdp.ConsumeIntegral<uint8_t>() & 0x1F;     // Operating point [0, 31]
  uint32_t tile_mode = fdp.ConsumeBool() ? 1 : 0;                       // Tile mode enabled
  uint32_t ext_tile_debug = fdp.ConsumeBool() ? 1 : 0;                  // Extended tile debug
  uint32_t is_annexb = fdp.ConsumeBool() ? 1 : 0;                       // Annex B format
  uint32_t output_all_layers = fdp.ConsumeBool() ? 1 : 0;               // Output all layers
  uint32_t skip_loop_filter = fdp.ConsumeBool() ? 1 : 0;                // Skip loop filter
  uint32_t byte_alignment = fdp.PickValueInArray({0, 32, 64, 128, 256, 512, 1024}); // Byte alignment
  
  // Get AV1 decoder interface
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) {
    return 0;
  }

  // Initialize decoder configuration
  aom_codec_dec_cfg_t cfg;
  cfg.threads = thread_count;
  cfg.w = 0;  // Width will be determined from stream
  cfg.h = 0;  // Height will be determined from stream
  cfg.allow_lowbitdepth = 1;  // Allow low bitdepth decoding

  // Initialize decoder context
  aom_codec_ctx_t codec;
  aom_codec_err_t res = aom_codec_dec_init(&codec, decoder_iface, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Set decoder control parameters based on fuzzer input
  aom_codec_control(&codec, AV1_SET_TILE_MODE, tile_mode);
  aom_codec_control(&codec, AV1D_EXT_TILE_DEBUG, ext_tile_debug);
  aom_codec_control(&codec, AV1D_SET_IS_ANNEXB, is_annexb);
  aom_codec_control(&codec, AV1D_SET_OUTPUT_ALL_LAYERS, output_all_layers);
  aom_codec_control(&codec, AV1D_SET_OPERATING_POINT, operating_point);
  aom_codec_control(&codec, AV1_SET_SKIP_LOOP_FILTER, skip_loop_filter);
  aom_codec_control(&codec, AV1_SET_BYTE_ALIGNMENT, byte_alignment);

  // Get remaining bytes as AV1 bitstream data
  std::vector<uint8_t> bitstream_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (bitstream_data.empty()) {
    aom_codec_destroy(&codec);
    return 0;
  }

  const uint8_t *bitstream = bitstream_data.data();
  size_t bitstream_size = bitstream_data.size();

  // Analyze stream info before decoding
  aom_codec_stream_info_t stream_info;
  stream_info.is_annexb = is_annexb;
  
  // Peek at stream information (uses interface pointer)
  aom_codec_peek_stream_info(decoder_iface, bitstream, bitstream_size, &stream_info);

  // Note: aom_codec_get_stream_info requires a codec context pointer and 
  // is typically called after decoding has started. We'll skip it here
  // as it requires valid decoder state.
  // Decode the bitstream
  res = aom_codec_decode(&codec, bitstream, bitstream_size, nullptr);
  
  // Even if decode fails, continue to test error paths
  if (res != AOM_CODEC_OK) {
    // Test decoder controls even on error to exercise error handling paths
    int frame_corrupted = 0;
    aom_codec_control(&codec, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
    
    aom_codec_destroy(&codec);
    return 0;
  }

  // Retrieve all decoded frames
  aom_codec_iter_t iter = nullptr;
  aom_image_t *decoded_frame = nullptr;
  int frame_count = 0;
  
  while ((decoded_frame = aom_codec_get_frame(&codec, &iter)) != nullptr) {
    frame_count++;
    
    // Exercise decoder control functions for each decoded frame
    if (frame_count <= 10) {  // Limit to prevent excessive iterations
      // Get frame properties using decoder control APIs
      int frame_size[2] = {0, 0};
      aom_codec_control(&codec, AV1D_GET_FRAME_SIZE, frame_size);
      
      int display_size[2] = {0, 0};
      aom_codec_control(&codec, AV1D_GET_DISPLAY_SIZE, display_size);
      
      unsigned int bit_depth = 0;
      aom_codec_control(&codec, AV1D_GET_BIT_DEPTH, &bit_depth);
      
      aom_img_fmt_t img_format = AOM_IMG_FMT_NONE;
      aom_codec_control(&codec, AV1D_GET_IMG_FORMAT, &img_format);
      
      // Get reference frame information
      int last_ref_updates = 0;
      aom_codec_control(&codec, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);
      
      int last_ref_used = 0;
      aom_codec_control(&codec, AOMD_GET_LAST_REF_USED, &last_ref_used);
      
      int frame_corrupted = 0;
      aom_codec_control(&codec, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
      
      int last_quantizer = 0;
      aom_codec_control(&codec, AOMD_GET_LAST_QUANTIZER, &last_quantizer);
      
      // Test additional decoder controls based on frame properties
      if (decoded_frame->d_w > 0 && decoded_frame->d_h > 0) {
        // Get frame flags and other properties
        int frame_flags = 0;
        aom_codec_control(&codec, AOMD_GET_FRAME_FLAGS, &frame_flags);
        
        int altref_present = 0;
        aom_codec_control(&codec, AOMD_GET_ALTREF_PRESENT, &altref_present);
        
        int fwd_kf_present = 0;
        aom_codec_control(&codec, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);
        
        // Test tile information controls
        aom_tile_info tile_info;
        aom_codec_control(&codec, AOMD_GET_TILE_INFO, &tile_info);
        
        // Get superblock size
        aom_superblock_size_t sb_size;
        aom_codec_control(&codec, AOMD_GET_SB_SIZE, &sb_size);
        
        // Get frame display properties
        int show_frame_flag = 0;
        aom_codec_control(&codec, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);
        
        int show_existing_frame_flag = 0;
        aom_codec_control(&codec, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing_frame_flag);
        
        // Get quantization information
        int base_q_idx = 0;
        aom_codec_control(&codec, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        
        unsigned int order_hint = 0;
        aom_codec_control(&codec, AOMD_GET_ORDER_HINT, &order_hint);
      }
    }
  }

  // Test decoder controls after all frames are processed
  if (frame_count > 0) {
    // Test still picture info
    aom_still_picture_info still_pic_info;
    aom_codec_control(&codec, AOMD_GET_STILL_PICTURE, &still_pic_info);
    
    // Test screen content tools info
    aom_screen_content_tools_info screen_content_info;
    aom_codec_control(&codec, AOMD_GET_SCREEN_CONTENT_TOOLS_INFO, &screen_content_info);
    
    // Test S-frame info
    aom_s_frame_info s_frame_info;
    aom_codec_control(&codec, AOMD_GET_S_FRAME_INFO, &s_frame_info);
  }

  // Clean up decoder resources
  aom_codec_destroy(&codec);

  return 0;
}

} // namespace harness_001
