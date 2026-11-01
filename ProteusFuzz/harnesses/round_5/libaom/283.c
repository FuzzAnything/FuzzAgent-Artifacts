// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void free_image_data(aom_image_t *img) {
  if (img && img->img_data) {
    aom_img_free(img);
  }
}

int LLVMFuzzerTestOneInput_283(const uint8_t *Data, size_t Size) {
  if (Size < 10) {
    return 0;
  }

  // Parse input to determine configuration
  uint8_t *ptr = (uint8_t *)Data;
  uint32_t width = (ptr[0] % 16) + 16;
  uint32_t height = (ptr[1] % 16) + 16;
  uint8_t bit_depth = (ptr[2] % 5) + 8; // 8, 9, 10, 11, 12
  uint8_t format_idx = ptr[3] % 4;
  uint8_t do_flush = (ptr[4] % 2);
  uint8_t use_flags = (ptr[5] % 2);
  uint8_t random_pts = (ptr[6] % 2);
  uint8_t random_duration = (ptr[7] % 2);
  uint8_t random_flags = (ptr[8] % 2);
  uint8_t invalid_param = (ptr[9] % 2);

  aom_img_fmt_t fmt;
  switch (format_idx) {
    case 0: fmt = AOM_IMG_FMT_I420; break;
    case 1: fmt = AOM_IMG_FMT_I42016; break;
    case 2: fmt = AOM_IMG_FMT_NV12; break;
    case 3: fmt = AOM_IMG_FMT_I444; break;
    default: fmt = AOM_IMG_FMT_I420;
  }

  // Initialize encoder configuration
  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(NULL, &cfg, 0);
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_bit_depth = bit_depth;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;
  cfg.rc_end_usage = AOM_CBR;

  // Initialize encoder context
  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    return 0;
  }

  aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    return 0;
  }

  // Prepare image
  aom_image_t img;
  aom_img_alloc(&img, fmt, width, height, 16);

  // Fill image with fuzz data
  size_t img_size = img.sz;
  if (Size > 10 + img_size) {
    memcpy(img.planes[0], Data + 10, img_size);
  } else {
    // Fill with pattern if not enough data
    for (int i = 0; i < img_size; i++) {
      img.planes[0][i] = (Data[(10 + i) % Size] ^ i);
    }
  }

  // Generate PTS and Duration
  aom_codec_pts_t pts = 0;
  unsigned long duration = 1;

  if (random_pts) {
    pts = (aom_codec_pts_t)((ptr[10] << 8) | ptr[11]);
  }
  if (random_duration) {
    duration = (unsigned long)((ptr[12] << 8) | ptr[13]);
    if (duration == 0) duration = 1; // Duration must be non-zero for encoding
    if (duration > UINT32_MAX) duration = UINT32_MAX;
  }

  aom_enc_frame_flags_t flags = 0;
  if (random_flags) {
    flags = (aom_enc_frame_flags_t)((ptr[14] << 8) | ptr[15]);
  }
  if (use_flags) {
    flags |= AOM_EFLAG_FORCE_KF;
  }

  // Invoke aom_codec_encode
  if (invalid_param) {
    // Test with NULL image but non-zero duration (should fail or be handled)
    // Or test with invalid duration
    err = aom_codec_encode(&ctx, &img, pts, 0, flags);
  } else {
    if (do_flush) {
      // Flush mode
      err = aom_codec_encode(&ctx, NULL, 0, 0, flags);
    } else {
      // Encode mode
      err = aom_codec_encode(&ctx, &img, pts, duration, flags);
    }
  }

  // Handle output packets if any
  aom_codec_iter_t iter = NULL;
  const aom_codec_cx_pkt_t *pkt;
  while ((pkt = aom_codec_get_cx_data(&ctx, &iter))) {
    // Consume data to avoid memory leaks in real usage, though fuzzing often ignores
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      // Optionally process pkt->data.frame
    }
  }

  // Cleanup
  aom_codec_destroy(&ctx);
  free_image_data(&img);

  return 0;
}