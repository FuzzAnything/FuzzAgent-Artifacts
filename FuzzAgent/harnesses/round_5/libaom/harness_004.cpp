#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"

namespace harness_004 {
/*
 * Fuzzing harness for libaom AV1 codec information & error handling APIs
 * Targets: aom_codec_error, aom_codec_err_to_string, aom_codec_error_detail,
 *          aom_codec_get_caps, aom_codec_iface_name, aom_codec_get_preview_frame,
 *          aom_codec_get_global_headers, aom_codec_get_stream_info,
 *          aom_codec_set_option, aom_codec_set_frame_buffer_functions
 * Follows coverage guidance for comprehensive error handling and codec information testing
 */



// Frame buffer callback functions for aom_codec_set_frame_buffer_functions
static int frame_buffer_get_cb(void *priv, size_t min_size, 
                               aom_codec_frame_buffer_t *fb) {
  // Allocate memory for frame buffer
  uint8_t *buffer = (uint8_t *)malloc(min_size);
  if (!buffer) {
    return -1;
  }
  
  // Initialize with zeros as required
  memset(buffer, 0, min_size);
  
  fb->data = buffer;
  fb->size = min_size;
  fb->priv = priv;  // Store private data pointer
  return 0;
}

static int frame_buffer_release_cb(void *priv, aom_codec_frame_buffer_t *fb) {
  // Free the allocated memory
  if (fb->data) {
    free(fb->data);
    fb->data = nullptr;
  }
  fb->size = 0;
  fb->priv = nullptr;
  return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for decoder testing with error handling
  if (size < 128) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume decoder configuration parameters
  uint32_t thread_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 8);
  uint32_t operating_point = fdp.ConsumeIntegral<uint8_t>() & 0x1F;
  uint32_t tile_mode = fdp.ConsumeBool() ? 1 : 0;
  uint32_t is_annexb = fdp.ConsumeBool() ? 1 : 0;
  uint32_t byte_alignment = fdp.PickValueInArray({0, 32, 64, 128});
  
  // Get AV1 decoder interface
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) {
    return 0;
  }

  // 1. Query codec information BEFORE initialization
  // aom_codec_get_caps - get decoder capabilities
  aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
  
  // aom_codec_iface_name - get interface name
  const char *iface_name = aom_codec_iface_name(decoder_iface);
  (void)iface_name;  // Use to avoid unused variable warning

  // Initialize decoder configuration
  aom_codec_dec_cfg_t cfg;
  cfg.threads = thread_count;
  cfg.w = 0;  // Width will be determined from stream
  cfg.h = 0;  // Height will be determined from stream
  cfg.allow_lowbitdepth = 1;

  // Initialize decoder context
  aom_codec_ctx_t codec;
  aom_codec_err_t res = aom_codec_dec_init(&codec, decoder_iface, &cfg, 0);
  
  // 2. Test error APIs even if initialization fails
  const char *error_str = aom_codec_error(&codec);
  const char *error_detail = aom_codec_error_detail(&codec);
  
  // Convert error code to string
  const char *err_string = aom_codec_err_to_string(res);
  (void)err_string;
  
  if (res != AOM_CODEC_OK) {
    // If initialization failed, we can still test some APIs
    aom_codec_destroy(&codec);
    return 0;
  }

  // 3. Set configuration options with aom_codec_set_option (using control API)
  aom_codec_control(&codec, AV1_SET_TILE_MODE, tile_mode);
  aom_codec_control(&codec, AV1D_SET_IS_ANNEXB, is_annexb);
  aom_codec_control(&codec, AV1D_SET_OPERATING_POINT, operating_point);
  aom_codec_control(&codec, AV1_SET_BYTE_ALIGNMENT, byte_alignment);

  // 4. Set frame buffer functions with aom_codec_set_frame_buffer_functions
  void *cb_priv = nullptr;  // No private data needed
  res = aom_codec_set_frame_buffer_functions(&codec, frame_buffer_get_cb, 
                                             frame_buffer_release_cb, cb_priv);
  
  // Test error APIs after setting frame buffer functions
  error_str = aom_codec_error(&codec);
  error_detail = aom_codec_error_detail(&codec);

  // 5. Perform codec operations (decode) to generate potential errors
  // Get remaining bytes as AV1 bitstream data
  std::vector<uint8_t> bitstream_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (bitstream_data.empty()) {
    // Test error APIs on empty input
    error_str = aom_codec_error(&codec);
    error_detail = aom_codec_error_detail(&codec);
    
    // 8. Clean up with aom_codec_destroy
    aom_codec_destroy(&codec);
    return 0;
  }

  const uint8_t *bitstream = bitstream_data.data();
  size_t bitstream_size = bitstream_data.size();

  // Try to decode the bitstream
  res = aom_codec_decode(&codec, bitstream, bitstream_size, nullptr);
  
  // 5. Query error information after decode operation
  error_str = aom_codec_error(&codec);
  error_detail = aom_codec_error_detail(&codec);
  
  // Convert decode result to string
  err_string = aom_codec_err_to_string(res);

  // 6. Query codec information after decode
  // Get capabilities again (should be same)
  caps = aom_codec_get_caps(decoder_iface);
  
  // Get interface name again
  iface_name = aom_codec_iface_name(decoder_iface);

  // 7. Retrieve frame/stream data
  // Try to get stream information
  aom_codec_stream_info_t stream_info;
  stream_info.is_annexb = is_annexb;
  aom_codec_err_t stream_res = aom_codec_get_stream_info(&codec, &stream_info);
  
  // Test error APIs after stream info query
  if (stream_res != AOM_CODEC_OK) {
    error_str = aom_codec_error(&codec);
    error_detail = aom_codec_error_detail(&codec);
  }

  // Try to get global headers (encoder API - may fail on decoder)
  // This will likely fail since we're using a decoder, but we want to test error paths
  aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec);
  (void)global_headers;  // May be NULL for decoder
  
  // Try to get preview frame (encoder API - may fail on decoder)
  // This will likely fail since we're using a decoder, but we want to test error paths
  const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec);
  (void)preview_frame;  // May be NULL for decoder

  // Also test with encoder interface for completeness
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  if (encoder_iface) {
    // Get encoder capabilities
    aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface);
    (void)enc_caps;
    
    // Get encoder interface name
    const char *enc_name = aom_codec_iface_name(encoder_iface);
    (void)enc_name;
    
    // Initialize minimal encoder to test preview frame and global headers
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_ctx_t enc_codec;
    aom_codec_err_t enc_res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    if (enc_res == AOM_CODEC_OK) {
      enc_cfg.g_w = 16;
      enc_cfg.g_h = 16;
      enc_cfg.g_timebase.num = 1;
      enc_cfg.g_timebase.den = 30;
      enc_cfg.rc_target_bitrate = 100;
      enc_cfg.g_lag_in_frames = 0;
      
      enc_res = aom_codec_enc_init(&enc_codec, encoder_iface, &enc_cfg, 0);
      
      if (enc_res == AOM_CODEC_OK) {
        // Test encoder-specific APIs
        aom_fixed_buf_t *enc_global_headers = aom_codec_get_global_headers(&enc_codec);
        (void)enc_global_headers;
        
        const aom_image_t *enc_preview_frame = aom_codec_get_preview_frame(&enc_codec);
        (void)enc_preview_frame;
        
        // Test error APIs on encoder
        const char *enc_error_str = aom_codec_error(&enc_codec);
        const char *enc_error_detail = aom_codec_error_detail(&enc_codec);
        (void)enc_error_str;
        (void)enc_error_detail;
        
        aom_codec_destroy(&enc_codec);
      }
    }
  }

  // 8. Clean up with aom_codec_destroy
  aom_codec_destroy(&codec);

  return 0;
}

} // namespace harness_004
