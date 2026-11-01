// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void free_image(aom_image_t *img) {
  if (img) {
    aom_img_free(img);
  }
}

static aom_codec_ctx_t *create_encoder_context(aom_codec_iface_t *iface,
                                               aom_codec_enc_cfg_t *cfg) {
  aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return NULL;

  memset(ctx, 0, sizeof(aom_codec_ctx_t));

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    free(ctx);
    return NULL;
  }

  return ctx;
}

static void destroy_encoder_context(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
    free(ctx);
  }
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  // Use fuzz data to configure encoder settings safely
  uint32_t width = (Data[0] % 256) + 16;
  uint32_t height = (Data[1] % 256) + 16;
  cfg.g_w = (width + 15) & ~15;
  cfg.g_h = (height + 15) & ~15;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = (Data[2] % 60) + 1;
  cfg.rc_target_bitrate = (Data[3] % 1000) + 100;
  cfg.g_threads = 1;
  cfg.g_error_resilient = 0;
  cfg.rc_end_usage = AOM_CBR;
  cfg.rc_min_quantizer = 0;
  cfg.rc_max_quantizer = 63;

  aom_codec_ctx_t *ctx = create_encoder_context(iface, &cfg);
  if (!ctx) return 0;

  // Fuzz aom_codec_control calls if data allows
  if (Size > 4) {
    int val = Data[4] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
    
    if (Size > 5) {
      int csp_val = Data[5] % 3;
      aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, csp_val);
    }

    if (Size > 6) {
      int level_idx = Data[6] % 10;
      aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);
    }
  }

  // Prepare image for encoding
  aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
  if (!img) {
    destroy_encoder_context(ctx);
    return 0;
  }

  // Fill image planes with fuzz data
  for (int i = 0; i < 3; i++) {
    if (img->planes[i]) {
      size_t plane_size = img->stride[i] * img->h;
      if (Size > 4) {
        size_t copy_size = (Size - 4) / 3;
        if (copy_size > plane_size) copy_size = plane_size;
        memcpy(img->planes[i], Data + 4 + (i * copy_size), copy_size);
      }
    }
  }

  aom_codec_pts_t pts = 0;
  unsigned long duration = (Data[Size - 1] % 100) + 1;

  // Encode frames
  for (int i = 0; i < 5 && Size > 4; i++) {
    aom_enc_frame_flags_t flags = 0;
    if (Size > 5 + i) flags = Data[5 + i] % 4;

    aom_codec_err_t res = aom_codec_encode(ctx, img, pts, duration, flags);
    if (res != AOM_CODEC_OK) {
      // Continue to flush
    }

    // Collect output
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, NULL)) != NULL) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Process encoded frame if needed
      }
      // No need to destroy pkt explicitly as it's managed by the codec context
    }

    pts += duration;
  }

  // Flush encoder
  while (1) {
    aom_codec_err_t res = aom_codec_encode(ctx, NULL, 0, 0, 0);
    if (res != AOM_CODEC_OK) break;

    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, NULL)) != NULL) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Process final frames
      }
      // No need to destroy pkt explicitly
    }

    if (!aom_codec_get_cx_data(ctx, NULL)) break;
  }

  free_image(img);
  destroy_encoder_context(ctx);

  return 0;
}