// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

/* Helper to initialize configuration with fuzzing data */
static void init_cfg_from_data(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
  memset(cfg, 0, sizeof(*cfg));
  // Use VP9 encoder interface name to get default config
  vpx_codec_enc_config_default(&vpx_codec_vp9_cx, cfg, 0);

  if (size > 0) {
    cfg->g_w = (data[0] & 0x0F) * 16 + 16;
    if (cfg->g_w > 4096) cfg->g_w = 4096;
    
    if (size > 1) {
      cfg->g_h = (data[1] & 0x0F) * 16 + 16;
      if (cfg->g_h > 4096) cfg->g_h = 4096;
    }
    
    if (size > 2) cfg->g_threads = (data[2] % 16) + 1;
    if (size > 3) cfg->g_profile = data[3] % 4;
    if (size > 4) cfg->g_bit_depth = (data[4] % 3) + 8; // 8, 10, 12
    if (size > 5) cfg->g_input_bit_depth = cfg->g_bit_depth;
    
    if (size > 6) cfg->rc_target_bitrate = (data[6] * 100) + 100;
    if (size > 7) cfg->rc_min_quantizer = data[7] % 64;
    if (size > 8) cfg->rc_max_quantizer = (data[8] % 64) + cfg->rc_min_quantizer;
    
    if (size > 9) cfg->kf_mode = (data[9] % 3); // 0, 1, 2
    if (size > 10) cfg->kf_min_dist = (data[10] % 100) + 1;
    if (size > 11) cfg->kf_max_dist = (data[11] % 100) + cfg->kf_min_dist;
  }
}

/* Helper to create a dummy image */
static vpx_image_t *create_dummy_image(unsigned int w, unsigned int h, vpx_bit_depth_t bit_depth) {
  vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 1);
  if (!img) return NULL;

  img->bit_depth = bit_depth;
  
  // Fill with deterministic data based on size to avoid random memory
  for (int p = 0; p < 3; p++) {
    int plane_h = h >> img->y_chroma_shift;
    int plane_w = w >> img->x_chroma_shift;
    for (int y = 0; y < plane_h; y++) {
      for (int x = 0; x < plane_w; x++) {
        img->planes[p][y * img->stride[p] + x] = (x + y) & 0xFF;
      }
    }
  }
  return img;
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  vpx_codec_ctx_t ctx;
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_iter_t iter = NULL;
  vpx_image_t *img = NULL;
  int ret;

  // 1. Prepare environment
  init_cfg_from_data(&cfg, Data, Size);

  // Get VP9 encoder interface
  const vpx_codec_iface_t *iface = &vpx_codec_vp9_cx;
  if (!iface) return 0;

  // Initialize encoder
  ret = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
  if (ret != VPX_CODEC_OK) {
    // Initialization failed, nothing to clean up per docs
    return 0;
  }

  // 2. Explore program states with control commands
  // Try VP9E_SET_SVC (even if implementation details are unknown, try the control ID)
  vpx_codec_control_(&ctx, VP9E_SET_SVC, 1); 
  vpx_codec_control_(&ctx, VP9E_SET_SVC, 0);
  
  // Try VP9E_SET_ROI_MAP
  // Construct a minimal ROI map
  vpx_roi_map_t roi_map;
  memset(&roi_map, 0, sizeof(roi_map));
  roi_map.enabled = (Size > 12) ? (Data[12] % 2) : 0;
  if (roi_map.enabled) {
    // Allocate a small map for testing
    unsigned char *map_data = malloc(100);
    if (map_data) {
      memset(map_data, 0, 100);
      roi_map.roi_map = map_data;
      roi_map.rows = 1;
      roi_map.cols = 1;
      vpx_codec_control_(&ctx, VP9E_SET_ROI_MAP, &roi_map);
      free(map_data);
    }
  }

  // 3. Encode frames
  img = create_dummy_image(cfg.g_w, cfg.g_h, cfg.g_bit_depth);
  if (img) {
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    
    // Encode a few frames with varying flags
    for (int i = 0; i < 3; i++) {
      vpx_enc_frame_flags_t flags = 0;
      if (Size > 13 + i) flags = Data[13 + i];
      
      ret = vpx_codec_encode(&ctx, img, pts, duration, flags, VPX_DL_REALTIME);
      if (ret != VPX_CODEC_OK) break;
      
      // Retrieve output packets
      const vpx_codec_cx_pkt_t *pkt;
      while ((pkt = vpx_codec_get_cx_data(&ctx, &iter))) {
        // Just iterate to trigger internal logic, ignore data for fuzzing stability
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
          // Access frame data to ensure validity
          (void)pkt->data.frame.buf;
          (void)pkt->data.frame.sz;
        }
      }
      
      pts += duration;
    }

    // Flush the encoder
    ret = vpx_codec_encode(&ctx, NULL, pts, duration, 0, VPX_DL_REALTIME);
    if (ret == VPX_CODEC_OK) {
      const vpx_codec_cx_pkt_t *pkt;
      while ((pkt = vpx_codec_get_cx_data(&ctx, &iter))) {
        (void)pkt;
      }
    }
    
    vpx_img_free(img);
  }

  // 4. Cleanup
  vpx_codec_destroy(&ctx);

  return 0;
}