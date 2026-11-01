// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
    free(ctx);
  }
}

static int init_encoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
  aom_codec_iface_t *iface;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t err;
  aom_codec_ctx_t *c;

  if (size < 4) return -1;

  iface = aom_codec_av1_cx();
  if (!iface) return -1;

  c = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!c) return -1;
  memset(c, 0, sizeof(aom_codec_ctx_t));

  err = aom_codec_enc_config_default(iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    cleanup_codec(c);
    return -1;
  }

  // Use fuzzing data to configure encoder settings safely
  cfg.g_w = (data[0] % 1024) + 16;
  cfg.g_h = (data[1] % 1024) + 16;
  cfg.g_timebase.num = (data[2] % 100) + 1;
  cfg.g_timebase.den = (data[3] % 100) + 1;
  cfg.rc_target_bitrate = (data[0] + data[1]) * 1000;
  cfg.g_pass = AOM_RC_ONE_PASS;
  cfg.g_lag_in_frames = 0;

  err = aom_codec_enc_init(c, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    cleanup_codec(c);
    return -1;
  }

  *ctx = c;
  return 0;
}

int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size) {
  aom_codec_ctx_t *ctx = NULL;
  aom_codec_err_t err;
  int intra_dct_only;
  int enable_tx_size_search;
  int delta_lf_mode;
  int color_primaries;
  int color_range;
  int matrix_coefficients;

  if (Size < 6) return 0;

  // Initialize encoder context
  if (init_encoder(&ctx, Data, Size) != 0) {
    return 0;
  }

  // Extract values from fuzzing input for control parameters
  intra_dct_only = (Data[0] % 2);
  enable_tx_size_search = (Data[1] % 2);
  delta_lf_mode = (Data[2] % 3);
  color_primaries = (Data[3] % 10);
  color_range = (Data[4] % 2);
  matrix_coefficients = (Data[5] % 10);

  // Call target control functions using correct enum names
  err = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);
  if (err != AOM_CODEC_OK) {
    // Ignore errors, continue testing other states
  }

  err = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);
  if (err != AOM_CODEC_OK) {
    // Ignore errors
  }

  err = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, delta_lf_mode);
  if (err != AOM_CODEC_OK) {
    // Ignore errors
  }

  err = aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);
  if (err != AOM_CODEC_OK) {
    // Ignore errors
  }

  err = aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, color_range);
  if (err != AOM_CODEC_OK) {
    // Ignore errors
  }

  err = aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coefficients);
  if (err != AOM_CODEC_OK) {
    // Ignore errors
  }

  // Cleanup
  cleanup_codec(ctx);

  return 0;
}