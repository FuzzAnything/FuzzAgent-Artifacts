// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int setup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

  cfg->g_w = 128;
  cfg->g_h = 128;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->rc_target_bitrate = 1000;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;

  return 1;
}

static int create_dummy_image(aom_image_t *img) {
  if (aom_img_alloc(img, AOM_IMG_FMT_I420, 128, 128, 1) != 0) return 0;
  
  // Fill with dummy data using stride (stride[0]) instead of d_bpl
  memset(img->planes[0], 0, img->stride[0] * img->h);
  memset(img->planes[1], 128, img->stride[1] * (img->h / 2));
  memset(img->planes[2], 128, img->stride[2] * (img->h / 2));
  return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_image_t img;
  aom_rc_funcs_t rc_funcs = {0};
  int ret;
  int use_external_rc = Data[0] & 1;
  int cq_level = (Data[0] >> 1) & 0xFF;
  int transfer_char = (Data[0] >> 9) & 0xFF;
  int enable_filter_intra = (Data[0] >> 17) & 1;
  int enable_tx64 = (Data[0] >> 18) & 1;
  int allow_warped_motion = (Data[0] >> 19) & 1;

  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));
  memset(&img, 0, sizeof(img));

  if (!setup_encoder(&ctx, &cfg)) return 0;
  if (!create_dummy_image(&img)) {
    cleanup_encoder(&ctx);
    return 0;
  }

  // Fuzz AV1E_SET_EXTERNAL_RATE_CONTROL
  if (use_external_rc) {
    rc_funcs.rc_type = AOM_RC_QP; 
    ret = aom_codec_control(&ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
    (void)ret;
  }

  // Fuzz AOME_SET_CQ_LEVEL
  ret = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);
  (void)ret;

  // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
  ret = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);
  (void)ret;

  // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra);
  (void)ret;

  // Fuzz AV1E_SET_ENABLE_TX64
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX64, enable_tx64);
  (void)ret;

  // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
  ret = aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, allow_warped_motion);
  (void)ret;

  // Attempt to encode a frame to exercise the state
  aom_enc_frame_flags_t flags = 0;
  aom_codec_cx_pkt_t pkt;
  memset(&pkt, 0, sizeof(pkt));
  
  ret = aom_codec_encode(&ctx, &img, 0, 1, flags);
  if (ret == AOM_CODEC_OK) {
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
      if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
        // No need to explicitly destroy frame data in this context, 
        // the library manages the memory for the packet data.
        // Just ensure we don't leak if we were holding a reference, 
        // but aom_codec_get_cx_data returns a pointer to internal data.
      }
    }
  }

  aom_img_free(&img);
  cleanup_encoder(&ctx);

  return 0;
}