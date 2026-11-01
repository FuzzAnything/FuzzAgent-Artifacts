// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int create_dummy_image(vpx_image_t *img, unsigned int w, unsigned int h) {
  if (!img || w == 0 || h == 0) return 0;

  img->fmt = VPX_IMG_FMT_I420;
  img->w = w;
  img->h = h;
  img->d_w = w;
  img->d_h = h;
  img->bit_depth = 8;
  img->x_chroma_shift = 1;
  img->y_chroma_shift = 1;
  img->cs = VPX_CS_BT_601;
  img->range = VPX_CR_STUDIO_RANGE;
  img->bps = 8;
  img->user_priv = NULL;
  img->img_data = NULL;
  img->img_data_owner = 0;
  img->self_allocd = 0;
  img->fb_priv = NULL;

  size_t y_size = w * h;
  size_t uv_size = (w / 2) * (h / 2);
  size_t total_size = y_size + 2 * uv_size;

  img->img_data = (unsigned char *)malloc(total_size);
  if (!img->img_data) return 0;

  img->planes[0] = img->img_data;
  img->planes[1] = img->img_data + y_size;
  img->planes[2] = img->img_data + y_size + uv_size;
  img->planes[3] = NULL;

  img->stride[0] = w;
  img->stride[1] = w / 2;
  img->stride[2] = w / 2;
  img->stride[3] = 0;

  memset(img->img_data, 128, total_size);
  img->img_data_owner = 1;
  img->self_allocd = 1;

  return 1;
}

static void free_dummy_image(vpx_image_t *img) {
  if (img && img->img_data) {
    free(img->img_data);
    img->img_data = NULL;
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
  }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
  if (Size < 16) return 0;

  vpx_codec_ctx_t ctx;
  vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
  if (!iface) return 0;

  vpx_codec_enc_cfg_t cfg;
  if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) return 0;

  // Parse fuzz data to configure encoder
  unsigned int width = (Data[0] % 256) + 16;
  unsigned int height = (Data[1] % 256) + 16;
  width = (width / 16) * 16;
  height = (height / 16) * 16;
  if (width < 16 || height < 16 || width > 1920 || height > 1080) return 0;

  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.g_threads = 1;
  cfg.g_profile = 0;
  cfg.g_bit_depth = VPX_BITS_8;
  cfg.g_input_bit_depth = 8;
  cfg.rc_end_usage = (enum vpx_rc_mode)(Data[2] % 4);
  cfg.rc_target_bitrate = (Data[3] % 100) + 50;
  cfg.rc_min_quantizer = (Data[4] % 50) + 10;
  cfg.rc_max_quantizer = (Data[5] % 50) + 60;
  cfg.kf_mode = (enum vpx_kf_mode)(Data[6] % 3);
  cfg.kf_min_dist = (Data[7] % 100) + 1;
  cfg.kf_max_dist = (Data[8] % 100) + 10;
  cfg.g_pass = VPX_RC_ONE_PASS;
  cfg.g_error_resilient = 0;
  cfg.g_lag_in_frames = 0;

  if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) return 0;

  vpx_image_t img;
  if (!create_dummy_image(&img, width, height)) {
    vpx_codec_destroy(&ctx);
    return 0;
  }

  // Fuzz vpx_codec_enc_config_set
  vpx_codec_enc_cfg_t new_cfg = cfg;
  if (Size > 9) new_cfg.g_threads = Data[9] % 4 + 1;
  if (Size > 10) new_cfg.rc_target_bitrate = (Data[10] % 100) + 50;
  if (Size > 11) new_cfg.kf_min_dist = Data[11] % 100;
  
  vpx_codec_enc_config_set(&ctx, &new_cfg);

  // Fuzz vpx_codec_set_cx_data_buf
  unsigned char *custom_buf = malloc(Size);
  if (custom_buf) {
    memcpy(custom_buf, Data, Size);
    vpx_fixed_buf_t buf = { .buf = custom_buf, .sz = Size };
    unsigned int pad_before = (Size > 12) ? Data[12] % 10 : 0;
    unsigned int pad_after = (Size > 13) ? Data[13] % 10 : 0;
    vpx_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
    free(custom_buf);
  }

  // Test with NULL buffer to reset
  vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);

  // Fuzz vpx_codec_encode
  vpx_codec_pts_t pts = 0;
  unsigned long duration = (Size > 14) ? (Data[14] % 100) + 1 : 1;
  vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(Size > 15 ? Data[15] % 4 : 0);
  vpx_enc_deadline_t deadline = (Size > 16) ? (Data[16] % 1000000) : 0;

  for (int i = 0; i < 3; i++) {
    vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
    pts += duration;
  }

  // Flush encoder
  vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);

  // Fuzz vpx_codec_get_cx_data
  vpx_codec_iter_t iter = NULL;
  const vpx_codec_cx_pkt_t *pkt;
  while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
    // Process packet if needed, currently just iterating
    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      // Access frame data safely
      (void)pkt->data.frame.buf;
      (void)pkt->data.frame.sz;
      (void)pkt->data.frame.pts;
    }
  }

  free_dummy_image(&img);
  vpx_codec_destroy(&ctx);

  return 0;
}