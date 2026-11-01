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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

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

  ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return NULL;

  res = aom_codec_enc_config_default(iface, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return NULL;
  }

  // Fuzz some config values based on input data to explore states
  if (size > 0) cfg.g_w = (data[0] % 256) + 16;
  if (size > 1) cfg.g_h = (data[1] % 256) + 16;
  if (size > 2) cfg.rc_target_bitrate = (data[2] % 1000) + 100;
  if (size > 3) cfg.g_timebase.num = (data[3] % 100) + 1;
  if (size > 4) cfg.g_timebase.den = (data[4] % 100) + 1;
  cfg.g_threads = 1;

  res = aom_codec_enc_init(ctx, iface, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return NULL;
  }

  return ctx;
}

int LLVMFuzzerTestOneInput_312(const uint8_t *Data, size_t Size) {
  // Ensure we have enough data for the most complex operation (SVC config)
  // We need at least 10 (base) + 7 (ref) + 7 (ref_idx) + 8 (refresh) = 32 bytes
  if (Size < 32) return 0;

  aom_codec_ctx_t *ctx = init_encoder(Data, Size);
  if (!ctx) return 0;

  // Fuzz AV1E_SET_INTER_DCT_ONLY
  int inter_dct_only = (Data[5] % 2);
  aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

  // Fuzz AV1E_SET_NOISE_SENSITIVITY
  int noise_sensitivity = (Data[6] % 100);
  aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

  // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
  int reduced_tx_set = (Data[7] % 2);
  aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, reduced_tx_set);

  // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
  int denoise_level = (Data[8] % 100);
  aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, denoise_level);

  // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
  int enable_dnl = (Data[9] % 2);
  aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable_dnl);

  // Fuzz AV1E_SET_SVC_REF_FRAME_CONFIG
  {
    aom_svc_ref_frame_config_t svc_cfg;
    memset(&svc_cfg, 0, sizeof(svc_cfg));

    // Fill arrays with fuzzed data
    // reference[7]
    for (int i = 0; i < 7; i++) {
      svc_cfg.reference[i] = (Data[10 + i] % 2);
    }
    // ref_idx[7]
    for (int i = 0; i < 7; i++) {
      svc_cfg.ref_idx[i] = (Data[10 + i + 7] % 8);
    }
    // refresh[8]
    for (int i = 0; i < 8; i++) {
      svc_cfg.refresh[i] = (Data[10 + i + 14] % 2);
    }

    aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_cfg);
  }

  cleanup_codec(ctx);
  return 0;
}