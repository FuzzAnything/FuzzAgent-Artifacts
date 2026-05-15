#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
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
 * Fuzzing harness for libaom AV1 codec library
 * Targets both decoder and encoder APIs
 * Based on existing av1_dec_fuzzer.cc and simple_encoder.c patterns
 */



#define IVF_FRAME_HDR_SZ (4 + 8) /* 4 byte size + 8 byte timestamp */
#define IVF_FILE_HDR_SZ 32

// Based on build_av1_dec_fuzzer.sh, FORCE_HIGHBITDEPTH_DECODING is set to 0
#ifndef FORCE_HIGHBITDEPTH_DECODING
#define FORCE_HIGHBITDEPTH_DECODING 0
#endif

// Simple implementation of mem_get_le32 (reads 32-bit little-endian value)
static inline uint32_t mem_get_le32(const uint8_t* mem) {
    return (uint32_t)mem[0] |
           ((uint32_t)mem[1] << 8) |
           ((uint32_t)mem[2] << 16) |
           ((uint32_t)mem[3] << 24);
}

// Simple YUV image buffer structure
struct YuvBuffer {
  std::vector<uint8_t> y_plane;
  std::vector<uint8_t> u_plane;
  std::vector<uint8_t> v_plane;
  size_t width;
  size_t height;
  size_t y_stride;
  size_t uv_stride;
};

// Create a simple YUV image from fuzzer data
YuvBuffer create_test_yuv_image(FuzzedDataProvider& fdp, size_t min_width, size_t max_width, size_t min_height, size_t max_height) {
  YuvBuffer img;
  
  // Get random dimensions
  img.width = fdp.ConsumeIntegralInRange<size_t>(min_width, max_width);
  img.height = fdp.ConsumeIntegralInRange<size_t>(min_height, max_height);
  
  // Calculate strides (aligned to 16 bytes for compatibility)
  img.y_stride = (img.width + 15) & ~15;
  img.uv_stride = ((img.width / 2) + 15) & ~15;
  
  // Allocate planes
  size_t y_size = img.y_stride * img.height;
  size_t uv_size = img.uv_stride * (img.height / 2);
  
  img.y_plane = fdp.ConsumeBytes<uint8_t>(y_size);
  img.u_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
  img.v_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
  
  // If not enough data, pad with zeros
  if (img.y_plane.size() < y_size) img.y_plane.resize(y_size, 0);
  if (img.u_plane.size() < uv_size) img.u_plane.resize(uv_size, 0);
  if (img.v_plane.size() < uv_size) img.v_plane.resize(uv_size, 0);
  
  return img;
}

// Create aom_image_t from YuvBuffer
aom_image_t* create_aom_image_from_yuv(const YuvBuffer& yuv) {
  aom_image_t* img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                    static_cast<unsigned int>(yuv.width), 
                                    static_cast<unsigned int>(yuv.height), 16);
  if (!img) return nullptr;
  
  // Copy Y plane
  for (size_t i = 0; i < yuv.height; ++i) {
    memcpy(img->planes[AOM_PLANE_Y] + i * img->stride[AOM_PLANE_Y],
           yuv.y_plane.data() + i * yuv.y_stride,
           yuv.width);
  }
  
  // Copy U and V planes
  for (size_t i = 0; i < yuv.height / 2; ++i) {
    memcpy(img->planes[AOM_PLANE_U] + i * img->stride[AOM_PLANE_U],
           yuv.u_plane.data() + i * yuv.uv_stride,
           yuv.width / 2);
    memcpy(img->planes[AOM_PLANE_V] + i * img->stride[AOM_PLANE_V],
           yuv.v_plane.data() + i * yuv.uv_stride,
           yuv.width / 2);
  }
  
  return img;
}

// Decoder fuzzing function based on av1_dec_fuzzer.cc
void fuzz_decoder(FuzzedDataProvider& fdp) {
  // Check minimum size for IVF header
  if (fdp.remaining_bytes() <= IVF_FILE_HDR_SZ) {
    return;
  }
  
  // Get decoder configuration parameters from fuzzer input
  unsigned int tile_mode = fdp.ConsumeBool();
  unsigned int ext_tile_debug = fdp.ConsumeBool();
  unsigned int is_annexb = fdp.ConsumeBool();
  int output_all_layers = fdp.ConsumeBool();
  int operating_point = fdp.ConsumeIntegralInRange<int>(0, 31);
  
  // Initialize decoder
  aom_codec_iface_t *codec_interface = aom_codec_av1_dx();
  aom_codec_ctx_t codec;
  
  // Set thread count in the range [1, 64]
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
  aom_codec_dec_cfg_t cfg = { threads, 0, 0, !FORCE_HIGHBITDEPTH_DECODING };
  
  if (aom_codec_dec_init(&codec, codec_interface, &cfg, 0)) {
    return; // Initialization failed
  }
  
  // Set decoder controls based on fuzzed parameters
  AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1_SET_TILE_MODE, tile_mode);
  AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_EXT_TILE_DEBUG, ext_tile_debug);
  AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_IS_ANNEXB, is_annexb);
  AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_OUTPUT_ALL_LAYERS, output_all_layers);
  AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_OPERATING_POINT, operating_point);
  
  // Get remaining data for decoding (simulating IVF frames)
  std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
  const uint8_t* data = remaining_data.data();
  size_t size = remaining_data.size();
  
  // Simple frame decoding loop
  size_t offset = 0;
  while (offset + IVF_FRAME_HDR_SZ < size) {
    // Simulate frame header (4 byte size + 8 byte timestamp)
    size_t frame_size = std::min<size_t>(
        mem_get_le32(data + offset), 
        size - offset - IVF_FRAME_HDR_SZ);
    
    offset += IVF_FRAME_HDR_SZ;
    
    if (frame_size == 0 || offset + frame_size > size) {
      break;
    }
    
    // Try to peek stream info
    aom_codec_stream_info_t stream_info;
    stream_info.is_annexb = is_annexb;
    aom_codec_peek_stream_info(codec_interface, data + offset, frame_size, &stream_info);
    
    // Decode the frame
    aom_codec_decode(&codec, data + offset, frame_size, nullptr);
    
    // Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    while ((img = aom_codec_get_frame(&codec, &iter)) != nullptr) {
      // Frame retrieved, do nothing with it for fuzzing
    }
    
    offset += frame_size;
  }
  
  aom_codec_destroy(&codec);
}

// Encoder fuzzing function based on simple_encoder.c
void fuzz_encoder(FuzzedDataProvider& fdp) {
  // Need enough data for basic configuration
  if (fdp.remaining_bytes() < 100) {
    return;
  }
  
  // Get encoder configuration from fuzzer input
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
  int usage = fdp.ConsumeIntegralInRange<int>(0, 1); // 0 = good quality, 1 = realtime
  int speed = fdp.ConsumeIntegralInRange<int>(0, 9);
  int threads = fdp.ConsumeIntegralInRange<int>(1, 8);
  int keyframe_interval = fdp.ConsumeIntegralInRange<int>(0, 100);
  int max_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
  
  // Initialize encoder
  aom_codec_iface_t *encoder = aom_codec_av1_cx();
  aom_codec_ctx_t codec;
  aom_codec_enc_cfg_t cfg;
  
  // Get default configuration
  aom_codec_err_t res = aom_codec_enc_config_default(encoder, &cfg, usage);
  if (res != AOM_CODEC_OK) {
    return;
  }
  
  // Update configuration with fuzzed parameters
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_threads = threads;
  cfg.g_error_resilient = fdp.ConsumeBool() ? 1 : 0;
  cfg.g_pass = AOM_RC_ONE_PASS;
  cfg.g_lag_in_frames = 0;
  
  // Set bitrate
  cfg.rc_end_usage = AOM_VBR;
  cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
  cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(2, 60);
  cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(2, 60);
  
  // Initialize encoder
  res = aom_codec_enc_init(&codec, encoder, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    return;
  }
  
  // Set encoder controls
  aom_codec_control(&codec, AOME_SET_CPUUSED, speed);
  aom_codec_control(&codec, AOME_SET_ENABLEAUTOALTREF, fdp.ConsumeBool());
  aom_codec_control(&codec, AV1E_SET_ENABLE_CDEF, fdp.ConsumeBool());
  aom_codec_control(&codec, AV1E_SET_ENABLE_RESTORATION, fdp.ConsumeBool());
  
  // Create test YUV images for encoding
  std::vector<YuvBuffer> test_images;
  for (int i = 0; i < max_frames && fdp.remaining_bytes() > 1000; ++i) {
    test_images.push_back(create_test_yuv_image(fdp, 16, 256, 16, 256));
  }
  
  // Encode frames
  for (size_t frame_idx = 0; frame_idx < test_images.size(); ++frame_idx) {
    aom_image_t* img = create_aom_image_from_yuv(test_images[frame_idx]);
    if (!img) continue;
    
    // Set flags (force keyframe if interval reached)
    int flags = 0;
    if (keyframe_interval > 0 && (frame_idx % keyframe_interval) == 0) {
      flags |= AOM_EFLAG_FORCE_KF;
    }
    
    // Encode the frame
    res = aom_codec_encode(&codec, img, frame_idx, 1, flags);
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Encoded frame available, do nothing with it for fuzzing
      }
    }
    
    aom_img_free(img);
  }
  
  // Flush encoder
  res = aom_codec_encode(&codec, NULL, -1, 1, 0);
  
  aom_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 50) {
    return 0; // Need minimum data for meaningful fuzzing
  }
  
  FuzzedDataProvider fdp(data, size);
  
  // Decide whether to test decoder or encoder based on first byte
  bool test_decoder = fdp.ConsumeBool();
  
  if (test_decoder) {
    fuzz_decoder(fdp);
  } else {
    // For encoder, we need to split the data since encoder needs more structured input
    // Recreate FuzzedDataProvider with remaining data
    std::vector<uint8_t> encoder_data = fdp.ConsumeRemainingBytes<uint8_t>();
    FuzzedDataProvider encoder_fdp(encoder_data.data(), encoder_data.size());
    fuzz_encoder(encoder_fdp);
  }
  
  return 0;
}

} // namespace harness_000
