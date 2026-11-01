// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * 1. AOME_SET_ENABLEAUTOALTREF
 * 2. AV1E_SET_MAX_REFERENCE_FRAMES
 * 3. AV1E_SET_S_FRAME_MODE
 * 4. AV1E_SET_NOISE_SENSITIVITY
 * 5. AV1E_ENABLE_RATE_GUIDE_DELTAQ
 * 6. AV1E_SET_CHROMA_SUBSAMPLING_Y
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
  aom_codec_enc_cfg_t cfg;
  aom_codec_iface_t *iface;
  int w, h;
  int err;

  if (size < 4) return 0;

  w = *(const uint16_t *)(data);
  h = *(const uint16_t *)(data + 2);

  if (w == 0 || h == 0 || w > 8192 || h > 8192) {
    w = 128;
    h = 128;
  }

  iface = aom_codec_av1_cx();
  if (!iface) return 0;

  err = aom_codec_enc_config_default(iface, &cfg, 0);
  if (err != AOM_CODEC_OK) return 0;

  cfg.g_w = w;
  cfg.g_h = h;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.g_usage = AOM_USAGE_REALTIME;
  cfg.rc_target_bitrate = 1000;

  err = aom_codec_enc_init(ctx, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    aom_codec_destroy(ctx);
    return 0;
  }

  return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx->iface) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size) {
  aom_codec_ctx_t ctx = {0};
  int initialized = 0;
  int ret;
  int val;

  if (Size == 0) return 0;

  initialized = init_encoder(&ctx, Data, Size);
  if (!initialized) return 0;

  /* Extract a value from input for parameter variation */
  if (Size >= 4) {
    val = *(const int32_t *)Data;
  } else {
    val = 0;
  }

  /* 1. AOME_SET_ENABLEAUTOALTREF */
  ret = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, val);
  (void)ret;

  /* 2. AV1E_SET_MAX_REFERENCE_FRAMES */
  ret = aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val);
  (void)ret;

  /* 3. AV1E_SET_S_FRAME_MODE */
  ret = aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val);
  (void)ret;

  /* 4. AV1E_SET_NOISE_SENSITIVITY */
  ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val);
  (void)ret;

  /* 5. AV1E_ENABLE_RATE_GUIDE_DELTAQ */
  ret = aom_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val);
  (void)ret;

  /* 6. AV1E_SET_CHROMA_SUBSAMPLING_Y */
  ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
  (void)ret;

  cleanup_encoder(&ctx);
  return 0;
}