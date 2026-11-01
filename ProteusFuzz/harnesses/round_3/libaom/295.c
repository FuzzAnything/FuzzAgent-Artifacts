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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    return;
  }

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return;
  }

  cfg->g_w = 32;
  cfg->g_h = 32;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return;
  }
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
  if (ctx && ctx->iface) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_295(const uint8_t *Data, size_t Size) {
  aom_codec_ctx_t ctx = {0};
  aom_codec_enc_cfg_t cfg = {0};
  int result;

  if (Size < 1) {
    return 0;
  }

  init_encoder_context(&ctx, &cfg);

  if (!ctx.iface) {
    return 0;
  }

  // Fuzz AV1E_SET_ENABLE_CFL_INTRA
  int enable_cfl = Data[0] % 2;
  result = aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl);
  (void)result;

  // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
  if (Size > 1) {
    int matrix_coeff = Data[1] % 10; // Assuming valid range 0-9 for testing
    result = aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coeff);
    (void)result;
  }

  // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
  if (Size > 2) {
    int enable_gm = Data[2] % 2;
    result = aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_gm);
    (void)result;
  }

  // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
  if (Size > 3) {
    int transfer_char = Data[3] % 10;
    result = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);
    (void)result;
  }

  // Fuzz AV1E_SET_COLOR_RANGE
  if (Size > 4) {
    int color_range = Data[4] % 2; // 0: Limited, 1: Full
    result = aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, color_range);
    (void)result;
  }

  // Fuzz AV1E_SET_COLOR_PRIMARIES
  if (Size > 5) {
    int color_prim = Data[5] % 10;
    result = aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, color_prim);
    (void)result;
  }

  cleanup_encoder_context(&ctx);
  return 0;
}