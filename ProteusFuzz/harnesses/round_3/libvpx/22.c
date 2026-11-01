// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9_DECODE_SVC_SPATIAL_LAYER__value at vp8dx.h:212:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void free_encoder(vpx_codec_ctx_t *ctx) {
  if (ctx) {
    vpx_codec_destroy(ctx);
    free(ctx);
  }
}

static void free_decoder(vpx_codec_ctx_t *ctx) {
  if (ctx) {
    vpx_codec_destroy(ctx);
    free(ctx);
  }
}

static vpx_svc_frame_drop_t *create_svc_frame_drop(const uint8_t *data, size_t size) {
  vpx_svc_frame_drop_t *svc_drop = malloc(sizeof(vpx_svc_frame_drop_t));
  if (!svc_drop) return NULL;
  memset(svc_drop, 0, sizeof(vpx_svc_frame_drop_t));

  if (size >= sizeof(int) * 4) {
    memcpy(svc_drop->framedrop_thresh, data, sizeof(int) * 4);
    if (size >= sizeof(int) * 5) {
      svc_drop->framedrop_mode = *(int *)(data + sizeof(int) * 4);
    }
    if (size >= sizeof(int) * 6) {
      svc_drop->max_consec_drop = *(int *)(data + sizeof(int) * 5);
    }
  }
  return svc_drop;
}

static vpx_svc_layer_id_t *create_svc_layer_id(const uint8_t *data, size_t size) {
  vpx_svc_layer_id_t *svc_layer = malloc(sizeof(vpx_svc_layer_id_t));
  if (!svc_layer) return NULL;
  memset(svc_layer, 0, sizeof(vpx_svc_layer_id_t));

  if (size >= sizeof(int)) {
    svc_layer->spatial_layer_id = *(int *)data;
  }
  if (size >= sizeof(int) * 2) {
    svc_layer->temporal_layer_id = *(int *)(data + sizeof(int));
  }
  if (size >= sizeof(int) * 5) {
    memcpy(svc_layer->temporal_layer_id_per_spatial, data + sizeof(int) * 2, sizeof(int) * 3);
  }
  return svc_layer;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
  vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
  if (!enc_iface || !dec_iface) return 0;

  vpx_codec_ctx_t *enc_ctx = malloc(sizeof(vpx_codec_ctx_t));
  vpx_codec_ctx_t *dec_ctx = malloc(sizeof(vpx_codec_ctx_t));
  if (!enc_ctx || !dec_ctx) {
    free(enc_ctx);
    free(dec_ctx);
    return 0;
  }

  vpx_codec_enc_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  vpx_codec_enc_config_default(enc_iface, &cfg, 0);
  cfg.g_w = 128;
  cfg.g_h = 128;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 100;
  cfg.g_error_resilient = 1;

  if (vpx_codec_enc_init(enc_ctx, enc_iface, &cfg, 0) != VPX_CODEC_OK) {
    free_encoder(enc_ctx);
    free_decoder(dec_ctx);
    return 0;
  }

  if (vpx_codec_dec_init(dec_ctx, dec_iface, NULL, 0) != VPX_CODEC_OK) {
    free_encoder(enc_ctx);
    free_decoder(dec_ctx);
    return 0;
  }

  // Fuzz VP9E_SET_SVC
  if (Size >= 1) {
    int svc_value = Data[0];
    vpx_codec_control(enc_ctx, VP9E_SET_SVC, svc_value);
  }

  // Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
  // Requires vpx_svc_frame_drop_t pointer
  if (Size >= 1) {
    vpx_svc_frame_drop_t svc_drop;
    memset(&svc_drop, 0, sizeof(svc_drop));
    svc_drop.framedrop_mode = Data[0]; 
    vpx_codec_control(enc_ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &svc_drop);
  }

  // Fuzz VP9E_SET_SVC_LAYER_ID
  vpx_svc_layer_id_t *svc_layer = create_svc_layer_id(Data, Size);
  if (svc_layer) {
    vpx_codec_control(enc_ctx, VP9E_SET_SVC_LAYER_ID, svc_layer);
    free(svc_layer);
  }

  // Fuzz VP9E_SET_SVC_INTER_LAYER_PRED
  if (Size >= 2) {
    int pred_value = Data[1];
    vpx_codec_control(enc_ctx, VP9E_SET_SVC_INTER_LAYER_PRED, pred_value);
  }

  // Fuzz VP9_DECODE_SVC_SPATIAL_LAYER
  if (Size >= 3) {
    int spatial_layer = Data[2];
    vpx_codec_control(dec_ctx, VP9_DECODE_SVC_SPATIAL_LAYER, spatial_layer);
  }

  // Fuzz VP9E_SET_SVC_PARAMETERS safely
  // This control expects a vpx_svc_layer_params_t or similar struct.
  // Passing raw data directly caused the overflow.
  // We will skip this specific control unless we can construct a valid struct.
  // Since the exact struct definition for parameters isn't fully provided in the prompt
  // and passing raw data is unsafe, we omit this call to prevent crashes.
  // If a valid struct type was known (e.g., vpx_svc_layer_params_t), we would construct it here.

  free_encoder(enc_ctx);
  free_decoder(dec_ctx);

  return 0;
}