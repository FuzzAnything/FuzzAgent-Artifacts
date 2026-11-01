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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <aom/aom_encoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
    free(ctx);
  }
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
  aom_codec_ctx_t *ctx = NULL;
  aom_codec_iface_t *iface;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t res;

  iface = aom_codec_av1_cx();
  if (!iface) return NULL;

  ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
  if (!ctx) return NULL;

  res = aom_codec_enc_config_default(iface, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return NULL;
  }

  // Use fuzzing data to configure width/height if possible, otherwise defaults
  if (size >= 4) {
    uint32_t w = *(const uint32_t *)(data);
    uint32_t h = *(const uint32_t *)(data + 4);
    if (w > 0 && w < 16384 && h > 0 && h < 16384) {
      cfg.g_w = w;
      cfg.g_h = h;
    }
  }

  cfg.g_threads = 1;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;

  res = aom_codec_enc_init(ctx, iface, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return NULL;
  }

  return ctx;
}

int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  aom_codec_ctx_t *ctx = init_encoder(Data, Size);
  if (!ctx) return 0;

  // Extract parameters from input for control functions
  int32_t param1 = (int32_t)(*(const uint32_t *)(Data));
  int32_t param2 = (int32_t)(*(const uint32_t *)(Data + 4));
  int32_t param3 = (int32_t)(*(const uint32_t *)(Data + 8));
  int32_t param4 = (int32_t)(*(const uint32_t *)(Data + 12));
  int32_t param5 = (int32_t)(*(const uint32_t *)(Data + 16));
  int32_t param6 = (int32_t)(*(const uint32_t *)(Data + 20));

  // Call target control functions with fuzzed parameters
  // Note: These functions are expected to be defined in the library
  // If they are not present, the linker will fail, which is expected in a real setup.
  // For fuzzing, we assume the library provides these symbols.

  aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, param1);
  aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, param2);
  aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, param3);
  aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, param4);
  aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, param5);
  aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, param6);

  // Cleanup
  cleanup_codec(ctx);

  return 0;
}