// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aomdx.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
  if (!ctx || !cfg) return 0;

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

  cfg->g_w = width;
  cfg->g_h = height;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_lag_in_frames = 0;
  cfg->rc_target_bitrate = 1000;
  cfg->rc_min_quantizer = 0;
  cfg->rc_max_quantizer = 63;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;

  return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
  if (Size < 10) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));

  int width = (Data[0] % 16) + 16;
  int height = (Data[1] % 16) + 16;

  if (!init_encoder(&ctx, &cfg, width, height)) return 0;

  // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
  int max_ref = (Data[2] % 8) + 1;
  aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref);

  // Fuzz AOME_SET_CQ_LEVEL
  int cq_level = (Data[3] % 64);
  aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);

  // Fuzz AOME_SET_ENABLEAUTOALTREF
  int enable_auto_altref = (Data[4] % 2);
  aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_altref);

  // Fuzz AV1E_SET_QM_MAX
  int qm_max = (Data[5] % 64);
  aom_codec_control(&ctx, AV1E_SET_QM_MAX, qm_max);

  // Fuzz AV1_GET_REFERENCE
  av1_ref_frame_t ref_get;
  memset(&ref_get, 0, sizeof(ref_get));
  ref_get.idx = (Data[6] % 8);
  ref_get.use_external_ref = 0;
  aom_codec_control(&ctx, AV1_GET_REFERENCE, &ref_get);

  // Fuzz AV1_SET_REFERENCE
  av1_ref_frame_t ref_set;
  memset(&ref_set, 0, sizeof(ref_set));
  ref_set.idx = (Data[7] % 8);
  ref_set.use_external_ref = (Data[8] % 2);
  
  // Initialize a dummy image for ref_set.img
  aom_image_t img;
  memset(&img, 0, sizeof(img));
  aom_image_t *img_ptr = aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
  if (img_ptr) {
    ref_set.img = *img_ptr;
    aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_set);
    aom_img_free(img_ptr);
  }

  cleanup_encoder(&ctx);

  return 0;
}