// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static void setup_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
  if (!cfg || !data || size < 4) return;
  
  // Initialize config structure manually since vpx_codec_enc_cfg_init_ver might not be available in all builds
  memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
  
  // Fuzz parameters based on input data
  cfg->g_w = data[0] ? data[0] : 16;
  cfg->g_h = data[1] ? data[1] : 16;
  cfg->g_threads = (size > 2 ? data[2] : 1) % 8;
  cfg->g_profile = (size > 3 ? data[3] : 0) % 4;
  
  if (size > 4) cfg->rc_target_bitrate = data[4] * 100 + 100;
  if (size > 5) cfg->rc_min_quantizer = data[5] % 64;
  if (size > 6) cfg->rc_max_quantizer = (data[6] % 64) + cfg->rc_min_quantizer;
  
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = (size > 7 ? data[7] : 30) + 1;
  
  cfg->rc_end_usage = (size > 8 ? data[8] : 0) % 4;
  cfg->kf_mode = (size > 9 ? data[9] : 0) % 3;
  cfg->kf_min_dist = (size > 10 ? data[10] : 120) + 1;
  cfg->kf_max_dist = (size > 11 ? data[11] : 240) + 1;
  
  cfg->g_pass = VPX_RC_ONE_PASS;
  cfg->g_error_resilient = 0;
  cfg->g_lag_in_frames = (size > 12 ? data[12] : 0) % 16;
}

static vpx_image_t *create_test_image(const uint8_t *data, size_t size, unsigned int w, unsigned int h) {
  vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 1);
  if (!img) return NULL;
  
  if (data && size > 0) {
    size_t plane_size = w * h;
    size_t total_size = plane_size * 3 / 2; // I420 size
    size_t copy_size = (size < total_size) ? size : total_size;
    
    if (img->planes[0]) {
      memcpy(img->planes[0], data, copy_size);
    }
    if (img->planes[1] && copy_size > plane_size) {
      size_t uv_size = copy_size - plane_size;
      if (uv_size > plane_size/2) uv_size = plane_size/2;
      memcpy(img->planes[1], data + plane_size, uv_size);
    }
    if (img->planes[2] && copy_size > plane_size * 1.5) {
      size_t uv2_size = copy_size - plane_size * 1.5;
      if (uv2_size > plane_size/2) uv2_size = plane_size/2;
      memcpy(img->planes[2], data + plane_size + plane_size/2, uv2_size);
    }
  }
  return img;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;
  
  vpx_codec_ctx_t ctx;
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
  
  if (!iface) return 0;
  
  setup_encoder_config(&cfg, Data, Size);
  
  // Test single encoder initialization
  vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    // Try with different flags
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, VPX_CODEC_USE_PSNR, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
      // Test error string conversion
      const char *err_str = vpx_codec_err_to_string(err);
      (void)err_str;
      return 0;
    }
  }
  
  if (err == VPX_CODEC_OK) {
    // Create test image
    vpx_image_t *img = create_test_image(Data + 4, Size - 4, cfg.g_w, cfg.g_h);
    
    if (img) {
      // Encode a few frames
      vpx_codec_pts_t pts = 0;
      for (int i = 0; i < 5; i++) {
        err = vpx_codec_encode(&ctx, img, pts, 1, 0, VPX_DL_REALTIME);
        if (err != VPX_CODEC_OK) {
          const char *err_str = vpx_codec_err_to_string(err);
          (void)err_str;
        }
        pts += 1;
        
        // Try to reconfigure
        if (i == 2) {
          vpx_codec_enc_cfg_t new_cfg = cfg;
          if (Size > 10) new_cfg.rc_target_bitrate = Data[10] * 50 + 100;
          vpx_codec_enc_config_set(&ctx, &new_cfg);
        }
      }
      
      // Flush encoder
      err = vpx_codec_encode(&ctx, NULL, pts, 1, 0, VPX_DL_REALTIME);
      (void)err;
      
      vpx_img_free(img);
    }
    
    vpx_codec_destroy(&ctx);
  }
  
  // Test multi-encoder initialization if we have enough data
  if (Size > 20) {
    int num_enc = (Data[20] % 3) + 1; // 1 to 3 encoders
    vpx_codec_ctx_t *ctxs = (vpx_codec_ctx_t *)malloc(num_enc * sizeof(vpx_codec_ctx_t));
    vpx_codec_enc_cfg_t *cfgs = (vpx_codec_enc_cfg_t *)malloc(num_enc * sizeof(vpx_codec_enc_cfg_t));
    vpx_rational_t *dsfs = (vpx_rational_t *)malloc(num_enc * sizeof(vpx_rational_t));
    
    if (ctxs && cfgs && dsfs) {
      for (int i = 0; i < num_enc; i++) {
        setup_encoder_config(&cfgs[i], Data + 21 + (i * 10), Size - 21 - (i * 10));
        dsfs[i].num = 1;
        dsfs[i].den = 1;
      }
      
      // Try multi-encoder init
      err = vpx_codec_enc_init_multi_ver(ctxs, iface, cfgs, num_enc, 0, dsfs, VPX_ENCODER_ABI_VERSION);
      if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
      } else {
        // Clean up multi-encoder contexts
        for (int i = 0; i < num_enc; i++) {
          vpx_codec_destroy(&ctxs[i]);
        }
      }
      
      free(ctxs);
      free(cfgs);
      free(dsfs);
    }
  }
  
  return 0;
}