// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static void init_encoder_cfg(vpx_codec_enc_cfg_t *cfg) {
  memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
  cfg->g_w = 128;
  cfg->g_h = 128;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_error_resilient = 0;
  cfg->g_pass = VPX_RC_ONE_PASS;
  cfg->g_threads = 1;
  cfg->g_profile = 0;
  cfg->g_bit_depth = VPX_BITS_8;
  cfg->g_input_bit_depth = 8;
  cfg->rc_end_usage = VPX_CBR;
  cfg->rc_target_bitrate = 100;
  cfg->rc_min_quantizer = 2;
  cfg->rc_max_quantizer = 63;
  cfg->rc_buf_sz = 1000;
  cfg->rc_buf_initial_sz = 500;
  cfg->rc_buf_optimal_sz = 750;
  cfg->kf_mode = VPX_KF_DISABLED;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  vpx_codec_ctx_t ctx;
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

  // Initialize configuration
  init_encoder_cfg(&cfg);

  // Fuzz error string conversion directly
  vpx_codec_err_t err_code = (vpx_codec_err_t)(Data[0] % 100);
  const char *err_str = vpx_codec_err_to_string(err_code);
  if (err_str) {
    // Just consume the string to avoid unused variable warnings
    (void)err_str[0];
  }

  // Test vpx_codec_error with NULL context
  const char *null_err = vpx_codec_error(NULL);
  if (null_err) {
    (void)null_err[0];
  }

  // Initialize encoder context
  if (vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
    // If init fails, we can still test error reporting
    const char *init_err = vpx_codec_error(&ctx);
    if (init_err) (void)init_err[0];
    const char *init_detail = vpx_codec_error_detail(&ctx);
    if (init_detail) (void)init_detail[0];
    return 0;
  }

  // Test vpx_codec_error_detail on a valid context (might be NULL if no error)
  const char *detail = vpx_codec_error_detail(&ctx);
  if (detail) (void)detail[0];

  // Create a dummy image for encoding
  vpx_image_t img;
  memset(&img, 0, sizeof(vpx_image_t));
  img.fmt = VPX_IMG_FMT_I420;
  img.w = cfg.g_w;
  img.h = cfg.g_h;
  img.d_w = cfg.g_w;
  img.d_h = cfg.g_h;
  img.bit_depth = 8;
  img.x_chroma_shift = 1;
  img.y_chroma_shift = 1;

  // Allocate planes
  size_t y_size = img.w * img.h;
  size_t uv_size = (img.w / 2) * (img.h / 2);
  unsigned char *y_plane = malloc(y_size);
  unsigned char *u_plane = malloc(uv_size);
  unsigned char *v_plane = malloc(uv_size);

  if (!y_plane || !u_plane || !v_plane) {
    vpx_codec_destroy(&ctx);
    free(y_plane);
    free(u_plane);
    free(v_plane);
    return 0;
  }

  // Fill with fuzzed data
  size_t total_size = y_size + uv_size + uv_size;
  size_t copy_size = (Size < total_size) ? Size : total_size;
  memcpy(y_plane, Data, copy_size);
  if (copy_size > y_size) {
    memcpy(u_plane, Data + y_size, (copy_size - y_size) > uv_size ? uv_size : (copy_size - y_size));
    if (copy_size > y_size + uv_size) {
      memcpy(v_plane, Data + y_size + uv_size, (copy_size - y_size - uv_size) > uv_size ? uv_size : (copy_size - y_size - uv_size));
    }
  }

  img.planes[0] = y_plane;
  img.planes[1] = u_plane;
  img.planes[2] = v_plane;
  img.stride[0] = img.w;
  img.stride[1] = img.w / 2;
  img.stride[2] = img.w / 2;

  // Fuzz encoding parameters
  vpx_codec_pts_t pts = (Size > 1) ? ((Data[1] << 8) | Data[2]) : 0;
  unsigned long duration = (Size > 3) ? ((Data[3] << 8) | Data[4]) : 1;
  vpx_enc_frame_flags_t flags = (Size > 5) ? (vpx_enc_frame_flags_t)Data[5] : 0;
  vpx_enc_deadline_t deadline = (Size > 6) ? (vpx_enc_deadline_t)((Data[6] << 8) | Data[7]) : 0;

  // Attempt to encode
  vpx_codec_err_t enc_res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
  if (enc_res != VPX_CODEC_OK) {
    const char *enc_err = vpx_codec_error(&ctx);
    if (enc_err) (void)enc_err[0];
    const char *enc_detail = vpx_codec_error_detail(&ctx);
    if (enc_detail) (void)enc_detail[0];
  }

  // Try reconfiguring
  vpx_codec_err_t cfg_res = vpx_codec_enc_config_set(&ctx, &cfg);
  if (cfg_res != VPX_CODEC_OK) {
    const char *cfg_err = vpx_codec_error(&ctx);
    if (cfg_err) (void)cfg_err[0];
  }

  // Flush the encoder
  vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);

  // Cleanup
  free(y_plane);
  free(u_plane);
  free(v_plane);
  vpx_codec_destroy(&ctx);

  return 0;
}