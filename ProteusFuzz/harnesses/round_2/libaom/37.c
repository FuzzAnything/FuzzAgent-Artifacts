// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Target functions:
 * - AOME_SET_CQ_LEVEL
 * - AV1E_SET_ENABLE_TPL_MODEL
 * - AV1E_SET_DELTAQ_MODE
 * - AOME_SET_ENABLEAUTOBWDREF
 * - AV1E_SET_FRAME_PERIODIC_BOOST
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 */

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                                 const uint8_t *data, size_t size) {
  const aom_codec_iface_t *iface;
  aom_codec_err_t res;

  /* Initialize configuration with defaults */
  aom_codec_enc_config_default(&aom_codec_av1_cx, cfg, 0);

  /* Use fuzzing data to tweak configuration slightly */
  if (size > 0) {
    cfg->g_w = (data[0] % 512) + 16;
    cfg->g_h = (data[1] % 512) + 16;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = (data[2] % 60) + 1;
    cfg->rc_target_bitrate = (data[3] % 1000) + 100;
    cfg->g_lag_in_frames = (data[4] % 20) + 1;
  }

  iface = &aom_codec_av1_cx;
  if (!iface) {
    return;
  }

  res = aom_codec_enc_init(ctx, iface, cfg, 0);
  if (res != AOM_CODEC_OK) {
    return;
  }
}

static void encode_one_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
  aom_codec_err_t res;
  aom_image_t img;
  unsigned int i;

  /* Prepare a dummy image from fuzzing data */
  aom_img_alloc(&img, AOM_IMG_FMT_I420,
                ctx->config.enc->g_w,
                ctx->config.enc->g_h,
                16);

  /* Fill image planes with fuzzing data */
  /* AOM_PLANE_Y is 0, img.h is height, img.stride[0] is Y stride */
  for (i = 0; i < img.h; i++) {
    size_t row_size = img.stride[0];
    size_t copy_size = (size > row_size) ? row_size : size;
    memcpy(img.planes[AOM_PLANE_Y] + i * row_size, data, copy_size);
    data += copy_size;
    size -= copy_size;
    if (size == 0) break;
  }

  /* Encode the frame - fixed signature: 5 arguments */
  res = aom_codec_encode(ctx, &img, 0, 1, 0);
  if (res != AOM_CODEC_OK) {
    return;
  }

  /* Drain packets */
  aom_codec_iter_t iter = NULL;
  const aom_codec_cx_pkt_t *pkt;
  while ((pkt = aom_codec_get_cx_data(ctx, &iter))) {
    /* Do nothing with the output, just consume it */
  }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t res;
  int cq_level;
  int tpl_model;
  int deltaq_mode;
  int autobwdref;
  int frame_periodic_boost;
  int coeff_cost_upd_freq;

  if (Size < 6) {
    return 0;
  }

  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));

  /* Initialize encoder */
  init_encoder_context(&ctx, &cfg, Data, Size);
  if (ctx.iface == NULL) {
    return 0;
  }

  /* Encode a frame to ensure encoder is in a valid state */
  encode_one_frame(&ctx, Data, Size);

  /*
   * Extract parameters from fuzzing data
   * We use modulo to keep values within reasonable ranges for the codec
   */
  cq_level = (Data[0] % 64) + 1;
  tpl_model = (Data[1] % 2);
  deltaq_mode = (Data[2] % 4);
  autobwdref = (Data[3] % 2);
  frame_periodic_boost = (Data[4] % 100);
  coeff_cost_upd_freq = (Data[5] % 10);

  /*
   * Invoke target control functions
   */

  /* 1. AOME_SET_CQ_LEVEL */
  res = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);
  (void)res;

  /* 2. AV1E_SET_ENABLE_TPL_MODEL */
  res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, tpl_model);
  (void)res;

  /* 3. AV1E_SET_DELTAQ_MODE */
  res = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, deltaq_mode);
  (void)res;

  /* 4. AOME_SET_ENABLEAUTOBWDREF */
  res = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, autobwdref);
  (void)res;

  /* 5. AV1E_SET_FRAME_PERIODIC_BOOST */
  res = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, frame_periodic_boost);
  (void)res;

  /* 6. AV1E_SET_COEFF_COST_UPD_FREQ */
  res = aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);
  (void)res;

  /* Cleanup */
  aom_codec_destroy(&ctx);

  return 0;
}