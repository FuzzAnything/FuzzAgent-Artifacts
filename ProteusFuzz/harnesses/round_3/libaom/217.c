// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
  if (!ctx || !cfg) return 0;

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  cfg->g_w = 64;
  cfg->g_h = 64;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_threads = 1;
  cfg->g_lag_in_frames = 0;
  cfg->rc_target_bitrate = 1000;
  cfg->rc_min_quantizer = 0;
  cfg->rc_max_quantizer = 63;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_217(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));

  if (!initialize_encoder(&ctx, &cfg)) {
    return 0;
  }

  uint32_t value = 0;
  if (Size >= 4) {
    memcpy(&value, Data, 4);
  }

  // Fuzz AV1E_SET_TIER_MASK
  aom_codec_control(&ctx, AV1E_SET_TIER_MASK, value);

  // Fuzz AV1E_SET_FORCE_VIDEO_MODE
  aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, value);

  // Fuzz AV1E_SET_ENABLE_CDEF
  aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, value);

  // Fuzz AV1E_SET_ENABLE_INTRABC
  aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, value);

  // Fuzz AV1E_SET_SUPERBLOCK_SIZE
  // Superblock size must be 1 (64x64) or 2 (128x128)
  int sb_size = (value % 2) + 1;
  aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);

  // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING
  aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, value);

  cleanup_encoder(&ctx);
  return 0;
}