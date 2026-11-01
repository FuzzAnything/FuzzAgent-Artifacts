// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static int create_dummy_image(aom_image_t *img, unsigned int w, unsigned int h, aom_img_fmt_t fmt) {
  if (!img || w == 0 || h == 0) return 0;
  
  if (aom_img_alloc(img, fmt, w, h, 16) != 0) {
    return 0;
  }
  
  // Fill with random data from fuzzer input is not possible here directly,
  // but we initialize to 0 for safety.
  memset(img->planes[0], 0, img->planes[0] ? img->stride[0] * h : 0);
  if (img->planes[1]) {
    memset(img->planes[1], 0, img->planes[1] ? img->stride[1] * (h >> img->y_chroma_shift) : 0);
  }
  if (img->planes[2]) {
    memset(img->planes[2], 0, img->planes[2] ? img->stride[2] * (h >> img->y_chroma_shift) : 0);
  }
  
  return 1;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, 0);
  
  // Use fuzzer data to configure width/height safely
  unsigned int width = (Data[0] % 128) + 16;
  unsigned int height = (Data[1] % 128) + 16;
  // Ensure alignment
  width = (width + 15) & ~15;
  height = (height + 15) & ~15;
  
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;
  cfg.g_error_resilient = 0;
  cfg.g_lag_in_frames = 0;
  cfg.g_usage = AOM_USAGE_REALTIME;
  
  // Initialize encoder
  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  aom_image_t img;
  memset(&img, 0, sizeof(img));
  if (!create_dummy_image(&img, width, height, AOM_IMG_FMT_I420)) {
    cleanup_codec(&ctx);
    return 0;
  }

  // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_AB_PARTITIONS
  // Pass 0 or 1 based on fuzzer data
  int enable_ab = (Size > 2) ? (Data[2] % 2) : 0;
  aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab);

  // Fuzz aom_codec_control_typechecked_AV1E_SET_VMAF_MODEL_PATH
  // We pass a dummy string or NULL based on fuzzer data
  if (Size > 3) {
    if (Data[3] % 2 == 0) {
      // Pass a dummy path string
      const char *dummy_path = "./dummy_file";
      aom_codec_control(&ctx, AV1E_SET_VMAF_MODEL_PATH, dummy_path);
    } else {
      // Pass NULL or invalid
      aom_codec_control(&ctx, AV1E_SET_VMAF_MODEL_PATH, NULL);
    }
  }

  // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
  int enable_intrabc = (Size > 4) ? (Data[4] % 2) : 0;
  aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

  // Fuzz aom_codec_set_option
  if (Size > 5) {
    const char *opt_name = (Data[5] % 2 == 0) ? "cpu-used" : "invalid-option";
    const char *opt_val = (Data[5] % 3 == 0) ? "0" : (Data[5] % 3 == 1) ? "10" : NULL;
    aom_codec_set_option(&ctx, opt_name, opt_val);
  }

  // Fuzz aom_codec_control_typechecked_AV1E_SET_SVC_PARAMS
  if (Size > 6) {
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = (Data[6] % 3) + 1;
    svc_params.number_temporal_layers = (Data[6] % 3) + 1;
    // Set some dummy values for arrays
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
      svc_params.max_quantizers[i] = 255;
      svc_params.min_quantizers[i] = 0;
      svc_params.layer_target_bitrate[i] = 1000;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
      svc_params.scaling_factor_num[i] = 1;
      svc_params.scaling_factor_den[i] = 1;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
      svc_params.framerate_factor[i] = 1;
    }
    aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
  }

  // Fuzz aom_codec_encode
  aom_codec_pts_t pts = 0;
  unsigned long duration = (Size > 7) ? (Data[7] % 100) + 1 : 1;
  aom_enc_frame_flags_t flags = 0;
  
  // Encode a few frames
  for (int i = 0; i < 3 && Size > (size_t)(8 + i); i++) {
    flags = (Data[8 + i] % 4) << 0; // Simple flag variation
    aom_codec_encode(&ctx, &img, pts, duration, flags);
    pts += duration;
    
    // Flush occasionally
    if (Data[8 + i] % 2 == 0) {
      aom_codec_encode(&ctx, NULL, 0, 0, 0);
    }
  }

  // Flush remaining
  while (1) {
    aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(&ctx, NULL);
    if (!pkt) break;
    aom_codec_get_cx_data(&ctx, pkt); // Consume
  }

  // Cleanup image first to prevent leaks
  aom_img_free(&img);
  
  // Cleanup codec
  cleanup_codec(&ctx);
  return 0;
}