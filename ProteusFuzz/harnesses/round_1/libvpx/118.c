// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_TPL__value at vp8cx.h:1087:1 in vp8cx.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

// Define VP9 control IDs if not present in the included headers
#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 0x1000
#endif

#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 0x1001
#endif

#ifndef VP9E_SET_TPL
#define VP9E_SET_TPL 0x1002
#endif

static void init_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
  memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
  
  cfg->g_w = (size > 0) ? (data[0] % 256) : 16;
  cfg->g_h = (size > 1) ? (data[1] % 256) : 16;
  if (cfg->g_w == 0) cfg->g_w = 16;
  if (cfg->g_h == 0) cfg->g_h = 16;
  
  cfg->g_bit_depth = VPX_BITS_8;
  cfg->g_input_bit_depth = 8;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = (size > 2) ? (data[2] % 60 + 1) : 30;
  cfg->g_threads = (size > 3) ? (data[3] % 4 + 1) : 1;
  cfg->g_profile = 0;
  cfg->g_usage = 0;
  cfg->g_error_resilient = 0;
  cfg->g_pass = VPX_RC_ONE_PASS;
  cfg->g_lag_in_frames = 0;
  
  cfg->rc_dropframe_thresh = 0;
  cfg->rc_resize_allowed = 0;
  cfg->rc_scaled_width = cfg->g_w;
  cfg->rc_scaled_height = cfg->g_h;
  cfg->rc_resize_up_thresh = 0;
  cfg->rc_resize_down_thresh = 0;
  cfg->rc_end_usage = VPX_CBR;
  cfg->rc_twopass_stats_in.sz = 0;
  cfg->rc_twopass_stats_in.buf = NULL;
  cfg->rc_firstpass_mb_stats_in.sz = 0;
  cfg->rc_firstpass_mb_stats_in.buf = NULL;
  cfg->rc_target_bitrate = (size > 4) ? (data[4] % 1000 + 100) : 500;
  
  cfg->rc_min_quantizer = 0;
  cfg->rc_max_quantizer = 63;
  
  cfg->rc_undershoot_pct = 50;
  cfg->rc_overshoot_pct = 50;
  
  cfg->rc_buf_sz = 1000;
  cfg->rc_buf_initial_sz = 500;
  cfg->rc_buf_optimal_sz = 500;
  
  cfg->rc_2pass_vbr_bias_pct = 0;
  cfg->rc_2pass_vbr_minsection_pct = 0;
  cfg->rc_2pass_vbr_maxsection_pct = 0;
  cfg->rc_2pass_vbr_corpus_complexity = 0;
  
  cfg->kf_mode = VPX_KF_AUTO;
  cfg->kf_min_dist = 12;
  cfg->kf_max_dist = 120;
  
  cfg->ss_number_layers = 1;
  for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
    cfg->ss_enable_auto_alt_ref[i] = 0;
    cfg->ss_target_bitrate[i] = cfg->rc_target_bitrate;
  }
  
  cfg->ts_number_layers = 1;
  for (int i = 0; i < VPX_TS_MAX_LAYERS; i++) {
    cfg->ts_target_bitrate[i] = cfg->rc_target_bitrate;
    cfg->ts_rate_decimator[i] = 1;
  }
  cfg->ts_periodicity = 1;
  for (int i = 0; i < VPX_TS_MAX_PERIODICITY; i++) {
    cfg->ts_layer_id[i] = 0;
  }
  
  for (int i = 0; i < VPX_MAX_LAYERS; i++) {
    cfg->layer_target_bitrate[i] = cfg->rc_target_bitrate;
  }
  
  cfg->temporal_layering_mode = 0;
  cfg->use_vizier_rc_params = 0;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  vpx_codec_ctx_t ctx;
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_err_t err;
  const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

  if (!iface) return 0;

  init_encoder_config(&cfg, Data, Size);

  err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
  if (err != VPX_CODEC_OK) return 0;

  // Fuzz vpx_codec_control_ via macro
  // Try different control IDs based on input data
  int ctrl_id = 0;
  if (Size > 2) {
    ctrl_id = Data[2] % 100; // Use first byte modulo 100 as control ID
  }
  
  // VP9E_SET_TILE_COLUMNS
  if (Size > 3) {
    int tile_cols = Data[3] % 8 + 1;
    vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);
  }
  
  // VP9E_SET_SVC
  if (Size > 4) {
    int svc_mode = Data[4] % 4;
    vpx_codec_control(&ctx, VP9E_SET_SVC, svc_mode);
  }
  
  // VP9E_SET_TPL
  if (Size > 5) {
    int tpl_mode = Data[5] % 4;
    vpx_codec_control(&ctx, VP9E_SET_TPL, tpl_mode);
  }

  // Fuzz vpx_codec_enc_config_set
  vpx_codec_enc_cfg_t new_cfg;
  init_encoder_config(&new_cfg, Data, Size);
  // Modify some fields based on input
  if (Size > 6) new_cfg.g_w = (Data[6] % 256) ? (Data[6] % 256) : 16;
  if (Size > 7) new_cfg.g_h = (Data[7] % 256) ? (Data[7] % 256) : 16;
  if (Size > 8) new_cfg.g_threads = (Data[8] % 4) + 1;
  
  vpx_codec_enc_config_set(&ctx, &new_cfg);

  // Fuzz vpx_codec_encode
  vpx_image_t img;
  memset(&img, 0, sizeof(img));
  img.w = cfg.g_w;
  img.h = cfg.g_h;
  img.d_w = cfg.g_w;
  img.d_h = cfg.g_h;
  img.r_w = cfg.g_w;
  img.r_h = cfg.g_h;
  img.bit_depth = cfg.g_bit_depth;
  img.fmt = VPX_IMG_FMT_I420;
  img.cs = VPX_CS_BT_601;
  img.range = VPX_CR_STUDIO_RANGE;
  img.x_chroma_shift = 1;
  img.y_chroma_shift = 1;
  img.bps = 8;
  
  // Allocate planes
  size_t y_size = img.w * img.h;
  size_t uv_size = (img.w / 2) * (img.h / 2);
  
  unsigned char *y_plane = malloc(y_size);
  unsigned char *u_plane = malloc(uv_size);
  unsigned char *v_plane = malloc(uv_size);
  
  if (!y_plane || !u_plane || !v_plane) {
    free(y_plane); free(u_plane); free(v_plane);
    vpx_codec_destroy(&ctx);
    return 0;
  }
  
  // Fill with fuzzed data
  if (Size > 9) {
    size_t fill_size = (Size - 9) < y_size ? (Size - 9) : y_size;
    memcpy(y_plane, Data + 9, fill_size);
  }
  memset(u_plane, 128, uv_size);
  memset(v_plane, 128, uv_size);
  
  img.planes[0] = y_plane;
  img.planes[1] = u_plane;
  img.planes[2] = v_plane;
  img.stride[0] = img.w;
  img.stride[1] = img.w / 2;
  img.stride[2] = img.w / 2;
  
  vpx_codec_pts_t pts = 0;
  unsigned long duration = 1;
  vpx_enc_frame_flags_t flags = 0;
  vpx_enc_deadline_t deadline = 0;
  
  // Encode a few frames
  for (int i = 0; i < 3; i++) {
    vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
    pts += duration;
    
    // Check for output
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
      // Consume data
      if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
        // Process frame data if needed
      }
      vpx_codec_iter_t iter = NULL;
      vpx_codec_get_cx_data(&ctx, &iter);
    }
  }
  
  // Flush encoder
  vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
  const vpx_codec_cx_pkt_t *pkt;
  while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
    vpx_codec_iter_t iter = NULL;
    vpx_codec_get_cx_data(&ctx, &iter);
  }

  // Cleanup
  free(y_plane);
  free(u_plane);
  free(v_plane);
  vpx_codec_destroy(&ctx);

  return 0;
}