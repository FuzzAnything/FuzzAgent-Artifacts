// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void cleanup_image(aom_image_t *img) {
  if (img && img->img_data) {
    aom_img_free(img);
  }
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  // Parse input to determine test scenario
  uint8_t mode = Data[0] % 5;
  uint32_t width = (Data[1] % 64) + 16;
  uint32_t height = ((Data[1] >> 4) % 64) + 16;
  uint8_t bit_depth_val = (Data[2] % 3) + 1; // 1->8, 2->10, 3->12
  aom_bit_depth_t bit_depth = (bit_depth_val == 1) ? AOM_BITS_8 : (bit_depth_val == 2) ? AOM_BITS_10 : AOM_BITS_12;

  aom_codec_ctx_t *ctx = NULL;
  aom_codec_enc_cfg_t cfg;
  aom_image_t img = {0};
  aom_fixed_buf_t cx_buf = {0};
  int cx_buf_size = 1024 * 1024;
  unsigned char *cx_buf_data = NULL;

  // Initialize encoder config
  if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_bit_depth = bit_depth;
  cfg.g_input_bit_depth = bit_depth;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_end_usage = AOM_VBR;
  cfg.rc_target_bitrate = 500;
  cfg.g_threads = 1;

  // Initialize codec context
  if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Allocate custom buffer for cx_data if mode requires it
  if (mode == 0 || mode == 1) {
    cx_buf_data = malloc(cx_buf_size);
    if (cx_buf_data) {
      cx_buf.buf = cx_buf_data;
      cx_buf.sz = cx_buf_size;
    }
  }

  // Fuzz aom_codec_set_cx_data_buf
  if (mode == 0) {
    unsigned int pad_before = (Data[3] % 100);
    unsigned int pad_after = (Data[4] % 100);
    aom_codec_set_cx_data_buf(ctx, &cx_buf, pad_before, pad_after);
  } else if (mode == 1) {
    // Test with NULL to restore default
    aom_codec_set_cx_data_buf(ctx, NULL, 0, 0);
  }

  // Fuzz aom_codec_set_option
  if (mode == 2) {
    const char *options[] = {"cpu-used", "crf", "static-thresh", "sharpness", "arnr-strength"};
    const char *values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "invalid"};
    int opt_idx = Data[3] % 5;
    int val_idx = Data[4] % 11;
    aom_codec_set_option(ctx, options[opt_idx], values[val_idx]);
  }

  // Fuzz aom_codec_enc_config_set
  if (mode == 3) {
    // Modify config slightly
    cfg.rc_min_quantizer = Data[3] % 64;
    cfg.rc_max_quantizer = (Data[4] % 64) + cfg.rc_min_quantizer;
    aom_codec_enc_config_set(ctx, &cfg);
  }

  // Fuzz aom_codec_get_global_headers
  if (mode == 4) {
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(ctx);
    if (headers && headers->buf) {
      free(headers->buf);
      free(headers);
    }
  }

  // Create an image to encode
  if (aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
    cleanup_codec(ctx);
    free(cx_buf_data);
    return 0;
  }
  
  // Fill image with fuzz data
  size_t plane_size = img.planes[0] ? img.stride[0] * img.h : 0;
  if (plane_size > 0) {
    size_t data_len = Size - 5;
    size_t copy_len = (data_len < plane_size) ? data_len : plane_size;
    memcpy(img.planes[0], Data + 5, copy_len);
    // Fill rest with pattern if needed
    if (copy_len < plane_size) {
      memset(img.planes[0] + copy_len, 0x00, plane_size - copy_len);
    }
  }

  // Fuzz aom_codec_encode
  aom_codec_pts_t pts = 0;
  unsigned long duration = (Data[5] % 100) + 1;
  aom_enc_frame_flags_t flags = 0;

  // Encode one frame
  if (aom_codec_encode(ctx, &img, pts, duration, flags) != AOM_CODEC_OK) {
    // Check error
    const char *err = aom_codec_error(ctx);
    (void)err; // Suppress unused warning
  }

  // Flush encoder
  while (1) {
    aom_codec_err_t res = aom_codec_encode(ctx, NULL, 0, 0, 0);
    if (res != AOM_CODEC_OK) break;

    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, NULL)) != NULL) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Process packet if needed
      }
      // aom_codec_get_cx_data returns a pointer to internal data or a packet structure.
      // We do not free the packet structure itself as it is managed by the codec context
      // until the next call or destruction.
    }
    
    // Check if encoder is done
    if (aom_codec_get_cx_data(ctx, NULL) == NULL) break;
  }

  // Cleanup
  cleanup_image(&img);
  cleanup_codec(ctx);
  free(cx_buf_data);

  return 0;
}