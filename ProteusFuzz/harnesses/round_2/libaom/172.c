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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_172(const uint8_t *Data, size_t Size) {
  // Require enough data to perform all operations safely
  if (Size < 16) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Initialize configuration from fuzz data safely
  cfg.g_w = (Data[0] % 128) + 16;
  cfg.g_h = (Data[1] % 128) + 16;
  cfg.g_timebase.num = Data[2] + 1;
  cfg.g_timebase.den = Data[3] + 1;
  cfg.rc_target_bitrate = (Data[4] % 100) + 1000;
  cfg.g_error_resilient = Data[5] % 2;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Fuzz AV1E_SET_EXTERNAL_RATE_CONTROL
  {
    aom_rc_funcs_t rc_funcs;
    memset(&rc_funcs, 0, sizeof(rc_funcs));
    rc_funcs.rc_type = (aom_rc_type_t)(Data[6] % 7);
    rc_funcs.priv = (void *)(uintptr_t)(Data[7] << 16 | Data[8] << 8 | Data[9]);
    aom_codec_control(&ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
  }

  // Fuzz AV1E_SET_ENABLE_RECT_TX
  {
    int enable_rect_tx = Data[10] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, &enable_rect_tx);
  }

  // Fuzz AV1E_SET_TUNE_CONTENT
  {
    int tune_content = Data[11] % 4; // Assuming valid range 0-3
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, &tune_content);
  }

  // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
  {
    int enable_diff_wtd_comp = Data[12] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, &enable_diff_wtd_comp);
  }

  // Fuzz AOME_SET_TUNING
  {
    int tuning = Data[13] % 4; // Assuming valid range 0-3
    aom_codec_control(&ctx, AOME_SET_TUNING, &tuning);
  }

  // Fuzz AV1E_SET_ENABLE_TX64
  {
    int enable_tx64 = Data[14] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX64, &enable_tx64);
  }

  // Cleanup
  cleanup_codec(&ctx);

  return 0;
}