// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
    free(ctx);
  }
}

int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return 0;

  memset(ctx, 0, sizeof(aom_codec_ctx_t));

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    cleanup_codec(ctx);
    return 0;
  }

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return 0;
  }

  cfg.g_w = 64;
  cfg.g_h = 64;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 100;

  aom_codec_err_t res = aom_codec_enc_init(ctx, iface, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return 0;
  }

  uint32_t value = 0;
  if (Size >= 4) {
    memcpy(&value, Data, 4);
  }

  // Fuzz AV1E_SET_ENABLE_ORDER_HINT
  aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, value & 1);

  // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
  aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value & 1);

  // Fuzz AV1E_GET_SEQ_LEVEL_IDX (read-only control)
  int32_t level_idx;
  aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);

  // Fuzz AV1E_SET_ENABLE_SUPERRES
  aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, value & 1);

  // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
  aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value & 1);

  // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
  aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, value & 1);

  // Try to encode a dummy frame to exercise more code paths
  aom_image_t img;
  aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
  
  // Calculate safe strides and sizes for I420 format
  // Y plane: 64x64
  // U plane: 32x32
  // V plane: 32x32
  size_t y_size = 64 * 64;
  size_t uv_size = 32 * 32;

  // Fill Y plane
  for (size_t i = 0; i < y_size; i++) {
    img.planes[0][i] = Data[i % Size];
  }

  // Fill U plane
  for (size_t i = 0; i < uv_size; i++) {
    img.planes[1][i] = Data[(i + 1) % Size];
  }

  // Fill V plane
  for (size_t i = 0; i < uv_size; i++) {
    img.planes[2][i] = Data[(i + 2) % Size];
  }

  // Fixed: aom_codec_encode takes 5 arguments
  aom_codec_encode(ctx, &img, 0, 1, 0);

  aom_img_free(&img);
  cleanup_codec(ctx);

  return 0;
}