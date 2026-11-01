// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"

static aom_codec_ctx_t *g_ctx = NULL;
static aom_codec_enc_cfg_t g_cfg;
static int g_initialized = 0;

static void cleanup_encoder(void) {
  if (g_ctx) {
    aom_codec_destroy(g_ctx);
    g_ctx = NULL;
  }
  g_initialized = 0;
}

static int init_encoder(const uint8_t *data, size_t size) {
  // Use the correct function name: aom_codec_av1_cx()
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, &g_cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Fuzz configuration parameters based on input data
  if (size > 0) {
    g_cfg.g_w = (data[0] % 256) + 16;
    g_cfg.g_h = (data[1] % 256) + 16;
    g_cfg.g_bit_depth = (data[2] % 3) == 0 ? AOM_BITS_8 : (data[2] % 2 == 0 ? AOM_BITS_10 : AOM_BITS_12);
    g_cfg.g_input_bit_depth = g_cfg.g_bit_depth;
    g_cfg.rc_target_bitrate = (data[3] % 100) + 10;
    g_cfg.g_threads = (data[4] % 4) + 1;
    g_cfg.g_usage = 0;
    g_cfg.g_timebase.num = 1;
    g_cfg.g_timebase.den = (data[5] % 100) + 1;
    g_cfg.rc_end_usage = data[6] % 4;
    g_cfg.kf_mode = data[7] % 3;
    g_cfg.kf_min_dist = (data[8] % 100) + 1;
    g_cfg.kf_max_dist = (data[9] % 100) + 1;
    
    // Set some cfg_options
    g_cfg.encoder_cfg.super_block_size = (data[10] % 3) * 64;
    g_cfg.encoder_cfg.disable_cdef = data[11] % 2;
    g_cfg.encoder_cfg.disable_lr = data[12] % 2;
    g_cfg.encoder_cfg.disable_obmc = data[13] % 2;
    g_cfg.encoder_cfg.disable_warp_motion = data[14] % 2;
    g_cfg.encoder_cfg.disable_global_motion = data[15] % 2;
    g_cfg.encoder_cfg.disable_dist_wtd_comp = data[16] % 2;
    g_cfg.encoder_cfg.disable_diff_wtd_comp = data[17] % 2;
    g_cfg.encoder_cfg.disable_inter_intra_comp = data[18] % 2;
    g_cfg.encoder_cfg.disable_masked_comp = data[19] % 2;
    g_cfg.encoder_cfg.disable_one_sided_comp = data[20] % 2;
    g_cfg.encoder_cfg.disable_palette = data[21] % 2;
    g_cfg.encoder_cfg.disable_intrabc = data[22] % 2;
    g_cfg.encoder_cfg.disable_cfl = data[23] % 2;
    g_cfg.encoder_cfg.disable_smooth_intra = data[24] % 2;
    g_cfg.encoder_cfg.disable_filter_intra = data[25] % 2;
    g_cfg.encoder_cfg.disable_dual_filter = data[26] % 2;
    g_cfg.encoder_cfg.disable_intra_angle_delta = data[27] % 2;
    g_cfg.encoder_cfg.disable_intra_edge_filter = data[28] % 2;
    g_cfg.encoder_cfg.disable_tx_64x64 = data[29] % 2;
    g_cfg.encoder_cfg.disable_smooth_inter_intra = data[30] % 2;
    g_cfg.encoder_cfg.disable_inter_inter_wedge = data[31] % 2;
    g_cfg.encoder_cfg.disable_inter_intra_wedge = data[32] % 2;
    g_cfg.encoder_cfg.disable_paeth_intra = data[33] % 2;
    g_cfg.encoder_cfg.disable_trellis_quant = data[34] % 2;
    g_cfg.encoder_cfg.disable_ref_frame_mv = data[35] % 2;
    g_cfg.encoder_cfg.reduced_reference_set = data[36] % 2;
    g_cfg.encoder_cfg.reduced_tx_type_set = data[37] % 2;
  }

  aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return 0;
  memset(ctx, 0, sizeof(aom_codec_ctx_t));

  if (aom_codec_enc_init(ctx, iface, &g_cfg, 0) != AOM_CODEC_OK) {
    free(ctx);
    return 0;
  }

  g_ctx = ctx;
  g_initialized = 1;
  return 1;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
  if (Size < 38) return 0;

  // Cleanup previous run
  cleanup_encoder();

  // Initialize encoder
  if (!init_encoder(Data, Size)) {
    return 0;
  }

  // Create a dummy image
  aom_image_t img;
  memset(&img, 0, sizeof(img));
  img.fmt = AOM_IMG_FMT_I420;
  img.w = g_cfg.g_w;
  img.h = g_cfg.g_h;
  img.bit_depth = g_cfg.g_bit_depth;
  img.d_w = img.w;
  img.d_h = img.h;
  img.r_w = img.w;
  img.r_h = img.h;
  img.x_chroma_shift = 1;
  img.y_chroma_shift = 1;
  img.stride[0] = img.w;
  img.stride[1] = img.w / 2;
  img.stride[2] = img.w / 2;
  
  size_t plane_size = img.w * img.h;
  size_t uv_size = (img.w / 2) * (img.h / 2);
  
  img.planes[0] = (unsigned char *)malloc(plane_size);
  img.planes[1] = (unsigned char *)malloc(uv_size);
  img.planes[2] = (unsigned char *)malloc(uv_size);
  
  if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    cleanup_encoder();
    return 0;
  }

  // Fill image with fuzz data
  memset(img.planes[0], Data[38] % 256, plane_size);
  memset(img.planes[1], Data[39] % 256, uv_size);
  memset(img.planes[2], Data[40] % 256, uv_size);

  aom_codec_pts_t pts = 0;
  unsigned long duration = (Data[41] % 100) + 1;
  aom_enc_frame_flags_t flags = Data[42] % 8;

  // Encode a few frames
  for (int i = 0; i < 3; i++) {
    aom_codec_err_t res = aom_codec_encode(g_ctx, &img, pts, duration, flags);
    if (res != AOM_CODEC_OK) {
      // Continue to test error handling paths
    }
    
    // Get output data
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(g_ctx, NULL)) != NULL) {
      // Process packet (no-op for fuzzing)
      aom_codec_iter_t iter = NULL;
      // Just consume the packet
    }
    
    pts += duration;
    
    // Reconfigure encoder occasionally
    if (i == 1 && Size > 43) {
      aom_codec_enc_cfg_t new_cfg = g_cfg;
      new_cfg.rc_target_bitrate = (Data[43] % 1000) + 10;
      aom_codec_enc_config_set(g_ctx, &new_cfg);
    }
  }

  // Flush the encoder
  aom_codec_err_t flush_res = aom_codec_encode(g_ctx, NULL, 0, 0, 0);
  if (flush_res != AOM_CODEC_OK) {
    // Expected in some error cases
  }

  // Drain remaining packets
  const aom_codec_cx_pkt_t *pkt;
  while ((pkt = aom_codec_get_cx_data(g_ctx, NULL)) != NULL) {
    // Consume packets
  }

  // Cleanup image
  free(img.planes[0]);
  free(img.planes[1]);
  free(img.planes[2]);

  cleanup_encoder();
  return 0;
}