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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
  if (!ctx) return 0;
  int ret = aom_codec_control(ctx, control_id, value);
  (void)ret;
  return 0;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, 0);

  // Use input data to configure encoder settings
  cfg.g_w = (Data[0] % 512) + 16;
  cfg.g_h = (Data[1] % 512) + 16;
  cfg.g_timebase.num = (Data[2] % 100) + 1;
  cfg.g_timebase.den = (Data[3] % 100) + 1;
  cfg.g_threads = (Data[4 % Size] % 16) + 1;
  cfg.g_lag_in_frames = (Data[(4 + 1) % Size] % 25);
  cfg.rc_target_bitrate = (Data[(4 + 2) % Size] * 256) + (Data[(4 + 3) % Size]);

  aom_codec_ctx_t ctx;
  aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    return 0;
  }

  // Fuzz various control functions with values derived from input
  if (Size > 4) {
    int sharpness = (int)(Data[4] % 7);
    fuzz_codec_control(&ctx, AOME_SET_SHARPNESS, sharpness);
  }
  if (Size > 5) {
    int enable_obmc = (int)(Data[5] % 2);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_OBMC, enable_obmc);
  }
  if (Size > 6) {
    int enable_adaptive_sharpness = (int)(Data[6] % 2);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, enable_adaptive_sharpness);
  }
  if (Size > 7) {
    int enable_palette = (int)(Data[7] % 2);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);
  }
  if (Size > 8) {
    int enable_restoration = (int)(Data[8] % 2);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
  }
  if (Size > 9) {
    int allow_warped_motion = (int)(Data[9] % 2);
    fuzz_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, allow_warped_motion);
  }

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}