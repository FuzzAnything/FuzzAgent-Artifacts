// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzzer_init_encoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface,
                               const aom_codec_enc_cfg_t *cfg, aom_codec_flags_t flags) {
  if (ctx == NULL || iface == NULL || cfg == NULL) {
    return -1;
  }
  aom_codec_err_t res = aom_codec_enc_init(ctx, iface, cfg, flags);
  return (res == AOM_CODEC_OK) ? 0 : -1;
}

static int fuzzer_init_decoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface,
                               const aom_codec_dec_cfg_t *cfg, aom_codec_flags_t flags) {
  if (ctx == NULL || iface == NULL || cfg == NULL) {
    return -1;
  }
  aom_codec_err_t res = aom_codec_dec_init(ctx, iface, cfg, flags);
  return (res == AOM_CODEC_OK) ? 0 : -1;
}

static void fuzzer_cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

static void fuzzer_cleanup_decoder(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  uint32_t seed = *(const uint32_t *)Data;
  Data += 4;
  Size -= 4;

  // Determine if we test encoder or decoder based on seed
  int use_encoder = (seed & 1);

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const aom_codec_iface_t *iface;
  if (use_encoder) {
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz configuration parameters
    cfg.g_w = (seed >> 8) & 0x3FF;
    cfg.g_h = (seed >> 16) & 0x3FF;
    if (cfg.g_w < 8 || cfg.g_h < 8) {
      cfg.g_w = 64;
      cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (seed >> 24) & 0xFF;
    if (cfg.rc_target_bitrate == 0) cfg.rc_target_bitrate = 1000;

    if (fuzzer_init_encoder(&ctx, iface, &cfg, 0) != 0) return 0;

    // Fuzz tile columns
    int tile_cols = (seed >> 2) & 0xF;
    if (tile_cols == 0) tile_cols = 1;
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);

    // Fuzz enable ext tile debug
    int enable_debug = (seed >> 12) & 0x1;
    aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable_debug);

    // Fuzz single tile decoding (encoder side might ignore but safe to call)
    int single_tile = (seed >> 14) & 0x1;
    aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);

    // Fuzz tile mode
    int tile_mode = (seed >> 16) & 0x3;
    aom_codec_control(&ctx, AV1_SET_TILE_MODE, tile_mode);

    fuzzer_cleanup_encoder(&ctx);
  } else {
    iface = aom_codec_av1_dx();
    if (iface == NULL) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; // Fixed: was allow_highbitdepth

    if (fuzzer_init_decoder(&ctx, iface, &cfg, 0) != 0) return 0;

    // Fuzz invert tile decode order
    int invert_order = (seed >> 2) & 0x1;
    aom_codec_control(&ctx, AV1_INVERT_TILE_DECODE_ORDER, invert_order);

    // Fuzz get tile info
    aom_tile_info tile_info;
    memset(&tile_info, 0, sizeof(tile_info));
    aom_codec_control(&ctx, AOMD_GET_TILE_INFO, &tile_info);

    // Fuzz tile columns (decoder might ignore but safe to call)
    int tile_cols = (seed >> 8) & 0xF;
    if (tile_cols == 0) tile_cols = 1;
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);

    // Fuzz enable ext tile debug
    int enable_debug = (seed >> 12) & 0x1;
    aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable_debug);

    // Fuzz single tile decoding
    int single_tile = (seed >> 14) & 0x1;
    aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);

    // Fuzz tile mode
    int tile_mode = (seed >> 16) & 0x3;
    aom_codec_control(&ctx, AV1_SET_TILE_MODE, tile_mode);

    fuzzer_cleanup_decoder(&ctx);
  }

  return 0;
}