// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

// Define missing control IDs if not present in headers to ensure compilation
#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 0x4200
#endif

#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 0x4201
#endif

#ifndef VP9E_SET_TPL
#define VP9E_SET_TPL 0x4202
#endif

static void setup_image(vpx_image_t *img, unsigned int w, unsigned int h, vpx_img_fmt_t fmt) {
  if (!img || w == 0 || h == 0) return;
  
  img->fmt = fmt;
  img->w = w;
  img->h = h;
  img->d_w = w;
  img->d_h = h;
  img->r_w = w;
  img->r_h = h;
  img->bit_depth = 8;
  img->cs = VPX_CS_BT_601;
  img->range = VPX_CR_STUDIO_RANGE;
  img->x_chroma_shift = (fmt == VPX_IMG_FMT_I420 || fmt == VPX_IMG_FMT_I42016 || fmt == VPX_IMG_FMT_NV12) ? 1 : 0;
  img->y_chroma_shift = (fmt == VPX_IMG_FMT_I420 || fmt == VPX_IMG_FMT_I42016 || fmt == VPX_IMG_FMT_NV12) ? 1 : 0;
  img->bps = 8;
  
  // Allocate planes
  size_t y_size = w * h;
  size_t uv_size = (w >> img->x_chroma_shift) * (h >> img->y_chroma_shift);
  
  img->img_data = (unsigned char *)malloc(y_size + 2 * uv_size);
  if (!img->img_data) return;
  
  img->planes[VPX_PLANE_Y] = img->img_data;
  img->stride[VPX_PLANE_Y] = w;
  
  img->planes[VPX_PLANE_U] = img->img_data + y_size;
  img->stride[VPX_PLANE_U] = w >> img->x_chroma_shift;
  
  img->planes[VPX_PLANE_V] = img->img_data + y_size + uv_size;
  img->stride[VPX_PLANE_V] = w >> img->x_chroma_shift;
  
  img->planes[VPX_PLANE_ALPHA] = NULL;
  img->stride[VPX_PLANE_ALPHA] = 0;
  
  // Initialize data with fuzzed content pattern (simple pattern for safety)
  memset(img->img_data, 0, y_size + 2 * uv_size);
}

static void cleanup_image(vpx_image_t *img) {
  if (img && img->img_data) {
    free(img->img_data);
    img->img_data = NULL;
  }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;
  
  vpx_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  
  vpx_codec_enc_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  
  // Initialize configuration with fuzzed values where appropriate
  cfg.g_w = Data[0] == 0 ? 16 : Data[0];
  cfg.g_h = Data[1] == 0 ? 16 : Data[1];
  cfg.g_threads = Size > 2 ? Data[2] % 8 : 1;
  cfg.g_profile = Size > 3 ? Data[3] % 4 : 0;
  cfg.g_bit_depth = VPX_BITS_8;
  cfg.g_input_bit_depth = 8;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = Size > 4 ? (Data[4] % 60) + 1 : 30;
  cfg.g_error_resilient = 0;
  cfg.g_pass = VPX_RC_ONE_PASS;
  cfg.g_lag_in_frames = 0;
  
  cfg.rc_dropframe_thresh = 0;
  cfg.rc_resize_allowed = 0;
  cfg.rc_scaled_width = cfg.g_w;
  cfg.rc_scaled_height = cfg.g_h;
  cfg.rc_resize_up_thresh = 0;
  cfg.rc_resize_down_thresh = 0;
  cfg.rc_end_usage = VPX_VBR;
  cfg.rc_target_bitrate = Size > 5 ? (Data[5] % 1000) + 100 : 1000;
  
  cfg.rc_min_quantizer = 0;
  cfg.rc_max_quantizer = 63;
  
  cfg.rc_undershoot_pct = 0;
  cfg.rc_overshoot_pct = 0;
  
  cfg.rc_buf_sz = 1000;
  cfg.rc_buf_initial_sz = 500;
  cfg.rc_buf_optimal_sz = 750;
  
  cfg.kf_mode = VPX_KF_DISABLED;
  cfg.kf_min_dist = 0;
  cfg.kf_max_dist = 0;
  
  // Initialize encoder
  const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
  if (!iface) return 0;
  
  vpx_codec_err_t res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
  if (res != VPX_CODEC_OK) return 0;
  
  // Create a test image
  vpx_image_t img;
  memset(&img, 0, sizeof(img));
  setup_image(&img, cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420);
  
  if (!img.img_data) {
    vpx_codec_destroy(&ctx);
    return 0;
  }
  
  // Fill image data with fuzzed input if available
  if (Size > 10) {
    size_t img_size = img.stride[VPX_PLANE_Y] * img.h + 
                      img.stride[VPX_PLANE_U] * (img.h >> 1) + 
                      img.stride[VPX_PLANE_V] * (img.h >> 1);
    size_t copy_size = Size - 10;
    if (copy_size > img_size) copy_size = img_size;
    memcpy(img.img_data, Data + 10, copy_size);
  }
  
  // Test vpx_codec_enc_config_set
  vpx_codec_enc_cfg_t new_cfg = cfg;
  if (Size > 12) {
    new_cfg.rc_target_bitrate = Data[12] % 2000 + 100;
  }
  vpx_codec_enc_config_set(&ctx, &new_cfg);
  
  // Test vpx_codec_control_ with various control IDs
  // VP9E_SET_TILE_COLUMNS
  int tile_cols = Size > 13 ? Data[13] % 8 : 1;
  vpx_codec_control_(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);
  
  // VP9E_SET_SVC (if supported)
  if (Size > 14) {
    int svc_mode = Data[14] % 2;
    vpx_codec_control_(&ctx, VP9E_SET_SVC, svc_mode);
  }
  
  // VP9E_SET_TPL (if supported)
  if (Size > 15) {
    int tpl_mode = Data[15] % 4;
    vpx_codec_control_(&ctx, VP9E_SET_TPL, tpl_mode);
  }
  
  // Test vpx_codec_encode with valid frame
  vpx_codec_pts_t pts = 0;
  unsigned long duration = Size > 16 ? Data[16] % 100 + 1 : 33;
  vpx_enc_frame_flags_t flags = 0;
  vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
  
  res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
  (void)res; // Ignore return value for fuzzing
  
  // Test vpx_codec_encode with NULL image (flush)
  res = vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
  (void)res;
  
  // Cleanup
  cleanup_image(&img);
  vpx_codec_destroy(&ctx);
  
  return 0;
}