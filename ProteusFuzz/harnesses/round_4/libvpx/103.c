// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

#define VPX_SS_MAX_LAYERS 4

/* Helper to safely read integers from fuzz input */
static int read_int(const uint8_t **data, size_t *size) {
  if (*size < sizeof(int)) return 0;
  int val;
  memcpy(&val, *data, sizeof(int));
  *data += sizeof(int);
  *size -= sizeof(int);
  return val;
}

/* Helper to safely read a single byte */
static unsigned char read_byte(const uint8_t **data, size_t *size) {
  if (*size < 1) return 0;
  unsigned char val = **data;
  (*data)++;
  (*size)--;
  return val;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  const uint8_t *ptr = Data;
  size_t remaining = Size;

  /* Fuzzing VP9E_SET_SVC_FRAME_DROP_LAYER */
  {
    vpx_svc_frame_drop_t drop_params;
    memset(&drop_params, 0, sizeof(drop_params));

    /* Initialize with fuzzed data */
    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
      if (remaining >= sizeof(int)) {
        drop_params.framedrop_thresh[i] = read_int(&ptr, &remaining);
      } else {
        drop_params.framedrop_thresh[i] = 0;
      }
    }

    if (remaining >= 1) {
      unsigned char mode_byte = read_byte(&ptr, &remaining);
      /* Map byte to enum: 0 = SVC_LAYER_DROP_MODE_LAYER_BASED, 1 = SVC_LAYER_DROP_MODE_CONSTRAINED */
      /* Using integer cast directly as enum values are typically 0 and 1 */
      drop_params.framedrop_mode = (SVC_LAYER_DROP_MODE)(mode_byte % 2);
    } else {
      drop_params.framedrop_mode = (SVC_LAYER_DROP_MODE)0;
    }

    if (remaining >= sizeof(int)) {
      drop_params.max_consec_drop = read_int(&ptr, &remaining);
    } else {
      drop_params.max_consec_drop = 0;
    }

    /* Attempt to initialize an encoder to test the control */
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = 0;
    cfg.g_lag_in_frames = 0;

    int res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != 0) {
      vpx_codec_destroy(&ctx);
      return 0;
    }

    /* Try to set the SVC frame drop layer control if the macro is defined */
    #ifdef VP9E_SET_SVC_FRAME_DROP_LAYER
    vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_params);
    #endif

    vpx_codec_destroy(&ctx);
  }

  /* Fuzzing VP9E_SET_SVC_LAYER_ID */
  {
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));

    if (remaining >= sizeof(int)) layer_id.spatial_layer_id = read_int(&ptr, &remaining);
    if (remaining >= sizeof(int)) layer_id.temporal_layer_id = read_int(&ptr, &remaining);
    
    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
      if (remaining >= sizeof(int)) {
        layer_id.temporal_layer_id_per_spatial[i] = read_int(&ptr, &remaining);
      }
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = 0;
    cfg.g_lag_in_frames = 0;

    int res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != 0) {
      vpx_codec_destroy(&ctx);
      return 0;
    }

    #ifdef VP9E_SET_SVC_LAYER_ID
    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
    #endif

    vpx_codec_destroy(&ctx);
  }

  /* Fuzzing VP9E_SET_SVC */
  {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = 0;
    cfg.g_lag_in_frames = 0;

    int res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != 0) {
      vpx_codec_destroy(&ctx);
      return 0;
    }

    #ifdef VP9E_SET_SVC
    /* Assuming VP9E_SET_SVC takes a pointer to a struct or an int flag */
    int svc_flag = (remaining > 0) ? (ptr[0] % 2) : 0;
    vpx_codec_control(&ctx, VP9E_SET_SVC, &svc_flag);
    #endif

    vpx_codec_destroy(&ctx);
  }

  return 0;
}