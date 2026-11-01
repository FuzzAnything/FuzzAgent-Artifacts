// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
  if (!ctx) return 0;
  return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  
  // Use the correct function to get the AV1 encoder interface
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use input data to configure basic parameters
  cfg.g_w = Data[0] ? Data[0] : 64;
  cfg.g_h = Data[1] ? Data[1] : 64;
  // Fixed member name from g_time_base to g_timebase
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.g_threads = 1;
  cfg.g_error_resilient = 0;
  cfg.rc_target_bitrate = 1000;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Extract values from input data for control parameters
  int32_t val1 = (Size > 2) ? (int32_t)Data[2] : 0;
  int32_t val2 = (Size > 3) ? (int32_t)Data[3] : 0;
  int32_t val3 = (Size > 4) ? (int32_t)Data[4] : 0;
  int32_t val4 = (Size > 5) ? (int32_t)Data[5] : 0;
  int32_t val5 = (Size > 6) ? (int32_t)Data[6] : 0;
  int32_t val6 = (Size > 7) ? (int32_t)Data[7] : 0;

  // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
  // Fixed identifier: removed AOM_CODEC_ prefix
  fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val1);

  // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
  fuzz_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val2);

  // Fuzz AV1E_SET_TILE_ROWS
  fuzz_codec_control(&ctx, AV1E_SET_TILE_ROWS, val3);

  // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
  fuzz_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val4);

  // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
  fuzz_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val5);

  // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
  fuzz_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val6);

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}