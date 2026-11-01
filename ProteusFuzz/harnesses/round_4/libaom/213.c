// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
    free(ctx);
  }
}

int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return 0;
  memset(ctx, 0, sizeof(aom_codec_ctx_t));

  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    cleanup_codec(ctx);
    return 0;
  }

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return 0;
  }

  // Use fuzzed data to configure width/height within reasonable bounds
  cfg.g_w = (Size > 0) ? (Data[0] % 256 + 16) : 16;
  cfg.g_h = (Size > 1) ? (Data[1] % 256 + 16) : 16;
  cfg.g_timebase.num = (Size > 2) ? (Data[2] % 255 + 1) : 1;
  cfg.g_timebase.den = (Size > 3) ? (Data[3] % 255 + 1) : 1;

  if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return 0;
  }

  // Fuzz various control functions using correct enum names
  if (Size > 4) {
    int intra_dct_only = Data[4] % 2;
    aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);
  }

  if (Size > 5) {
    int color_range = Data[5] % 2; // 0: limited, 1: full
    aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, color_range);
  }

  if (Size > 6) {
    int chroma_sample_pos = Data[6] % 3; // 0: unknown, 1: vertical, 2: collocated
    aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_sample_pos);
  }

  if (Size > 7) {
    int matrix_coefficients = Data[7] % 9; // 0-8 valid range
    aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coefficients);
  }

  if (Size > 8) {
    int color_primaries = Data[8] % 9; // 0-8 valid range
    aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);
  }

  if (Size > 9) {
    int transfer_characteristics = Data[9] % 9; // 0-8 valid range
    aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_characteristics);
  }

  // Cleanup
  cleanup_codec(ctx);
  return 0;
}