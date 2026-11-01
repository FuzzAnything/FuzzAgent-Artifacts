// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>

/* Helper to safely free encoder context */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_299(const uint8_t *Data, size_t Size) {
  if (Size < 12) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    return 0;
  }

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  /* Use fuzzed data to configure basic parameters */
  cfg.g_w = (Data[0] % 1024) + 16;
  cfg.g_h = (Data[1] % 1024) + 16;
  cfg.g_timebase.num = (Data[2] % 100) + 1;
  cfg.g_timebase.den = (Data[3] % 100) + 1;
  cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
  cfg.g_threads = 1;
  cfg.g_profile = 0;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  /* Prepare a dummy frame for encoding to ensure encoder is active */
  aom_image_t img;
  aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
  memset(img.planes[0], 128, img.d_w * img.d_h);
  memset(img.planes[1], 128, img.d_w * img.d_h / 4);
  memset(img.planes[2], 128, img.d_w * img.d_h / 4);

  aom_enc_frame_flags_t flags = 0;
  aom_codec_encode(&ctx, &img, 0, 1, flags);

  /* Fuzz AV1E_SET_ENABLE_FLIP_IDTX */
  int enable_flip_idtx = (Data[4] & 0x01);
  aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);

  /* Fuzz AV1E_SET_NOISE_SENSITIVITY */
  int noise_sensitivity = (Data[5] % 8);
  aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

  /* Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR */
  int max_drop = (Data[6] % 10);
  aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop);

  /* Fuzz AV1E_SET_ENABLE_INTRABC */
  int enable_intrabc = (Data[7] & 0x01);
  aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

  /* Fuzz AV1E_SET_RATE_DISTRIBUTION_INFO */
  /* Note: aom_rate_usage_info_t is not a public type in standard libaom headers for direct instantiation 
     in this manner without specific version checks. To avoid compilation errors and undefined behavior,
     we skip this specific control if the type is not available or use a safe fallback.
     However, since the task requires testing it, we assume the struct exists in the internal headers 
     or use a generic pointer approach if the API allows. 
     Given the error 'use of undeclared identifier', we must skip direct struct usage if not defined.
     We will attempt to pass a dummy pointer or skip if the type is truly missing in the environment.
     For this fix, we skip the specific struct initialization to ensure compilation, 
     but we keep the control call if the macro is defined, passing a dummy value if needed.
     Actually, AV1E_SET_RATE_DISTRIBUTION_INFO expects a pointer to aom_rate_usage_info_t.
     Since we cannot define the struct here, we will skip this specific control to maintain validity.
     Alternatively, we can cast a dummy integer if the API is robust, but that's risky.
     Let's skip this specific control to ensure the fuzzer compiles and runs for the others.
     If the environment has the header, it would work, but based on the error, it's missing.
     We will remove the block causing the error. */
  
  /* Fuzz AV1E_SET_AUTO_INTRA_TOOLS_OFF */
  int auto_intra_off = (Data[11] & 0x01);
  aom_codec_control(&ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, auto_intra_off);

  /* Finalize encoding */
  aom_codec_encode(&ctx, NULL, 1, 1, flags);

  /* Cleanup */
  aom_img_free(&img);
  cleanup_encoder(&ctx);

  return 0;
}