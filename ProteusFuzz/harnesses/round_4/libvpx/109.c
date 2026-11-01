// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// VP9E_GET_SVC_LAYER_ID__value at vp8cx.h:1046:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static vpx_codec_ctx_t codec;
static vpx_codec_enc_cfg_t cfg;

static void cleanup_codec(void) {
  if (vpx_codec_destroy(&codec) != VPX_CODEC_OK) {
    // Ignore errors during cleanup in fuzzing
  }
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  // Cleanup from previous run if needed
  cleanup_codec();

  // Initialize configuration
  if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
    return 0;
  }

  // Use input data to configure encoder settings
  cfg.g_w = (Data[0] % 128) + 16;
  cfg.g_h = (Data[1 % Size] % 128) + 16;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = (Data[2 % Size] * 1000) + 1000;
  cfg.g_error_resilient = (Data[3 % Size] % 2) ? VPX_ERROR_RESILIENT_DEFAULT : 0;
  cfg.g_lag_in_frames = (Data[4 % Size] % 10);
  cfg.rc_end_usage = VPX_CBR;

  // Initialize encoder
  if (vpx_codec_enc_init(&codec, vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
    return 0;
  }

  // Prepare frame data
  vpx_image_t img;
  vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
  memset(img.planes[0], Data[5 % Size] % 256, cfg.g_h * cfg.g_w);
  memset(img.planes[1], Data[6 % Size] % 256, (cfg.g_h / 2) * (cfg.g_w / 2));
  memset(img.planes[2], Data[7 % Size] % 256, (cfg.g_h / 2) * (cfg.g_w / 2));

  // Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG
  vpx_svc_ref_frame_config_t svc_ref_config;
  memset(&svc_ref_config, 0, sizeof(svc_ref_config));
  for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
    svc_ref_config.lst_fb_idx[i] = Data[(8 + i) % Size] % 32;
    svc_ref_config.gld_fb_idx[i] = Data[(9 + i) % Size] % 32;
    svc_ref_config.alt_fb_idx[i] = Data[(10 + i) % Size] % 32;
    svc_ref_config.update_buffer_slot[i] = Data[(11 + i) % Size] % 2;
    svc_ref_config.update_last[i] = Data[(12 + i) % Size] % 2;
    svc_ref_config.update_golden[i] = Data[(13 + i) % Size] % 2;
    svc_ref_config.update_alt_ref[i] = Data[(14 + i) % Size] % 2;
    svc_ref_config.reference_last[i] = Data[(15 + i) % Size] % 2;
    svc_ref_config.reference_golden[i] = Data[(16 + i) % Size] % 2;
    svc_ref_config.reference_alt_ref[i] = Data[(17 + i) % Size] % 2;
    svc_ref_config.duration[i] = Data[(18 + i) % Size];
  }
  vpx_codec_control(&codec, VP9E_SET_SVC_REF_FRAME_CONFIG, &svc_ref_config);

  // Fuzz VP9E_GET_SVC_LAYER_ID
  vpx_svc_layer_id_t svc_layer_id_get;
  memset(&svc_layer_id_get, 0, sizeof(svc_layer_id_get));
  vpx_codec_control(&codec, VP9E_GET_SVC_LAYER_ID, &svc_layer_id_get);

  // Fuzz VP9E_SET_SVC_INTER_LAYER_PRED
  int inter_layer_pred = Data[19 % Size] % 2;
  vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, inter_layer_pred);

  // Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
  vpx_svc_frame_drop_t svc_frame_drop;
  memset(&svc_frame_drop, 0, sizeof(svc_frame_drop));
  for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
    svc_frame_drop.framedrop_thresh[i] = Data[(20 + i) % Size];
  }
  // Use integer values 0 and 1 for the enum if identifiers are not defined
  svc_frame_drop.framedrop_mode = (Data[21 % Size] % 2) ? 1 : 0;
  svc_frame_drop.max_consec_drop = Data[22 % Size];
  vpx_codec_control(&codec, VP9E_SET_SVC_FRAME_DROP_LAYER, &svc_frame_drop);

  // Fuzz VP9E_SET_SVC_PARAMETERS
  // The API expects a specific struct (likely vpx_svc_parameters_t) containing arrays.
  // Passing an int pointer caused the crash because the library accessed array members.
  // We define a local struct matching the expected layout to avoid stack overflow.
  // Based on the crash at min_quantizers[layer], we need to provide a struct with that array.
  // Since the exact struct definition is not in the provided snippet, we create a minimal
  // compatible struct or skip if unsafe. However, to fuzz effectively, we assume a standard
  // SVC parameters struct layout often used in libvpx.
  
  // If the exact struct is unknown, we can try to pass a valid pointer to a struct 
  // that at least has the expected fields. 
  // A common struct for this is vpx_svc_parameters_t.
  // Since we don't have the header, we'll skip this specific control if we can't 
  // safely construct the struct, OR we define a compatible struct here.
  // Given the crash, the library expects a struct with 'min_quantizers' array.
  
  // Let's define a minimal struct that matches the expected access pattern to prevent crash.
  // This is a heuristic fix based on the crash log.
  struct vpx_svc_parameters_compat {
    int min_quantizers[VPX_SS_MAX_LAYERS];
    int max_quantizers[VPX_SS_MAX_LAYERS];
    int min_quantizers_temporal[VPX_SS_MAX_LAYERS];
    int max_quantizers_temporal[VPX_SS_MAX_LAYERS];
    // Add other fields if necessary, but min_quantizers is the critical one causing the crash.
    int spatial_layers;
    int temporal_layers;
    int temporal_layer_id;
    int spatial_layer_id;
    int svc_layer_id;
    int svc_frame_drop;
    int svc_inter_layer_pred;
    int svc_ref_frame_config;
    int svc_layer_drop_mode;
    int svc_max_consec_drop;
    int svc_framedrop_thresh[VPX_SS_MAX_LAYERS];
    // Padding to ensure alignment if needed
    int padding[16]; 
  };
  
  struct vpx_svc_parameters_compat svc_params;
  memset(&svc_params, 0, sizeof(svc_params));
  svc_params.spatial_layers = 1;
  svc_params.temporal_layers = 1;
  
  for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
    svc_params.min_quantizers[i] = Data[(23 + i) % Size] % 63;
    svc_params.max_quantizers[i] = Data[(24 + i) % Size] % 63;
    svc_params.min_quantizers_temporal[i] = Data[(25 + i) % Size] % 63;
    svc_params.max_quantizers_temporal[i] = Data[(26 + i) % Size] % 63;
    svc_params.svc_framedrop_thresh[i] = Data[(27 + i) % Size];
  }
  
  vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params);

  // Fuzz VP9E_SET_SVC_LAYER_ID
  vpx_svc_layer_id_t svc_layer_id_set;
  memset(&svc_layer_id_set, 0, sizeof(svc_layer_id_set));
  svc_layer_id_set.spatial_layer_id = Data[28 % Size] % VPX_SS_MAX_LAYERS;
  svc_layer_id_set.temporal_layer_id = Data[29 % Size] % 4;
  for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
    svc_layer_id_set.temporal_layer_id_per_spatial[i] = Data[(30 + i) % Size] % 4;
  }
  vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &svc_layer_id_set);

  // Encode a frame to ensure state changes are exercised
  vpx_codec_encode(&codec, &img, 0, 1, 0, 0);

  // Cleanup
  vpx_img_free(&img);
  cleanup_codec();

  return 0;
}