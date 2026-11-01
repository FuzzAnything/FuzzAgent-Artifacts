// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  aom_codec_enc_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  // Initialize default encoder configuration
  if (aom_codec_enc_config_default(&ctx, aom_codec_av1_cx(), 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use fuzzer data to tweak configuration slightly if possible
  if (Size > 0) {
    cfg.g_w = (Data[0] % 256) + 16; // Ensure minimum width
    cfg.g_h = (Data[1 % Size] % 256) + 16; // Ensure minimum height
  }

  // Initialize encoder context
  if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
    cleanup_codec(&ctx);
    return 0;
  }

  // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
  // This control typically takes a pointer to a film grain test vector structure.
  // Since we don't have the exact struct, we pass a pointer to our input data.
  aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, Data);

  // Fuzz AV1E_SET_ENABLE_ONESIDED_COMP
  // This control likely takes an integer (0 or 1).
  int enable_onesided = (Size > 0) ? (Data[0] % 2) : 0;
  aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, &enable_onesided);

  // Fuzz AV1E_GET_NUM_OPERATING_POINTS
  // This control likely returns an integer.
  int num_op_points = 0;
  aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_op_points);

  // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
  // This control likely takes an integer (0 or 1).
  int intra_tx_only = (Size > 1) ? (Data[1] % 2) : 0;
  aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &intra_tx_only);

  // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
  // This control likely takes an enum value for matrix coefficients.
  int matrix_coeffs = (Size > 2) ? (Data[2] % 10) : 0; // Limit to a reasonable range
  aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, &matrix_coeffs);

  // Fuzz AV1E_GET_BASELINE_GF_INTERVAL
  // This control likely returns an integer.
  int gf_interval = 0;
  aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval);

  // Cleanup
  cleanup_codec(&ctx);

  return 0;
}