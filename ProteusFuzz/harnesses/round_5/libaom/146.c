// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include "aom/aomcx.h"
#include "aom/aomdx.h"
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
  }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
  if (Size < 2) {
    return 0;
  }

  aom_codec_ctx_t ctx = {0};
  aom_codec_enc_cfg_t cfg = {0};
  aom_codec_iface_t *iface = NULL;
  aom_codec_err_t err;

  // Get the AV1 encoder interface safely
  iface = aom_codec_av1_cx();
  if (!iface) {
    return 0;
  }

  // Initialize configuration with the specific interface
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use input data to configure encoder settings
  cfg.g_w = Data[0] ? Data[0] : 64;
  cfg.g_h = Data[1] ? Data[1] : 64;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.g_threads = 1;
  cfg.g_lag_in_frames = 0;
  cfg.g_pass = 0;
  cfg.rc_target_bitrate = 1000;
  cfg.rc_min_quantizer = 0;
  cfg.rc_max_quantizer = 63;

  // Initialize codec context
  err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    return 0;
  }

  // Fuzz AV1E_SET_RTC_EXTERNAL_RC
  if (Size > 2) {
    int rtc_val = Data[2] % 2;
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_val);
  }

  // Fuzz AV1E_SET_ENABLE_ORDER_HINT
  if (Size > 3) {
    int order_hint = Data[3] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, order_hint);
  }

  // Fuzz AV1E_SET_MIN_PARTITION_SIZE
  if (Size > 4) {
    int min_part = Data[4];
    if (min_part < 4) min_part = 4;
    if (min_part > 64) min_part = 64;
    // Ensure power of 2
    min_part = (min_part & -min_part);
    if (min_part < 4) min_part = 4;
    if (min_part > 64) min_part = 64;
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_part);
  }

  // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
  if (Size > 5) {
    int intra_edge = Data[5] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, intra_edge);
  }

  // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
  if (Size > 6) {
    int smooth_intra = Data[6] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, smooth_intra);
  }

  // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
  if (Size > 7) {
    int dnl_denoise = Data[7] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, dnl_denoise);
  }

  // Cleanup
  cleanup_codec(&ctx);

  return 0;
}