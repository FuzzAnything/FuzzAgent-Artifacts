// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

// Define CDEF_MAX_STRENGTHS if not defined to ensure correct buffer size
#ifndef CDEF_MAX_STRENGTHS
#define CDEF_MAX_STRENGTHS 8
#endif

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
  if (!ctx || !data || size < 1) return 0;
  
  const aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (!iface) return 0;

  aom_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  cfg.w = 1920;
  cfg.h = 1080;

  if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }
  return 1;
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
  if (!ctx || !data || size < 1) return 0;

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.g_w = 1920;
  cfg.g_h = 1080;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;
  cfg.g_lag_in_frames = 0;

  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }
  return 1;
}

int LLVMFuzzerTestOneInput_290(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  aom_codec_ctx_t ctx_dec;
  aom_codec_ctx_t ctx_enc;
  memset(&ctx_dec, 0, sizeof(ctx_dec));
  memset(&ctx_enc, 0, sizeof(ctx_enc));

  int dec_init = init_decoder(&ctx_dec, Data, Size);
  int enc_init = init_encoder(&ctx_enc, Data, Size);

  // Fuzz AOMD_GET_LAST_QUANTIZER
  if (dec_init) {
    int last_quantizer = 0;
    aom_codec_control(&ctx_dec, AOMD_GET_LAST_QUANTIZER, &last_quantizer);
    (void)last_quantizer;
  }

  // Fuzz AOMD_GET_BASE_Q_IDX
  if (dec_init) {
    int base_q_idx = 0;
    aom_codec_control(&ctx_dec, AOMD_GET_BASE_Q_IDX, &base_q_idx);
    (void)base_q_idx;
  }

  // Fuzz AOME_GET_LAST_QUANTIZER
  if (enc_init) {
    int last_quantizer = 0;
    aom_codec_control(&ctx_enc, AOME_GET_LAST_QUANTIZER, &last_quantizer);
    (void)last_quantizer;
  }

  // Fuzz AOME_GET_LAST_QUANTIZER_64
  if (enc_init) {
    uint64_t last_quantizer_64 = 0;
    aom_codec_control(&ctx_enc, AOME_GET_LAST_QUANTIZER_64, &last_quantizer_64);
    (void)last_quantizer_64;
  }

  // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
  // Fixed: Allocate buffer dynamically to ensure it is large enough for CDEF_MAX_STRENGTHS
  if (enc_init) {
    int *cdef_strengths = (int *)malloc(CDEF_MAX_STRENGTHS * sizeof(int));
    if (cdef_strengths) {
      memset(cdef_strengths, 0, CDEF_MAX_STRENGTHS * sizeof(int));
      aom_codec_control(&ctx_enc, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);
      free(cdef_strengths);
    }
  }

  // Fuzz AV1D_SET_EXT_REF_PTR
  if (dec_init) {
    av1_ext_ref_frame_t ext_ref;
    memset(&ext_ref, 0, sizeof(ext_ref));
    
    // Create a dummy image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.w = 64;
    img.h = 64;
    img.bit_depth = 8;
    img.fmt = AOM_IMG_FMT_I420;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    
    // Allocate planes
    size_t plane_size = 64 * 64;
    unsigned char *y_data = malloc(plane_size);
    unsigned char *u_data = malloc(plane_size / 4);
    unsigned char *v_data = malloc(plane_size / 4);
    
    if (y_data && u_data && v_data) {
      img.planes[0] = y_data;
      img.planes[1] = u_data;
      img.planes[2] = v_data;
      img.stride[0] = 64;
      img.stride[1] = 32;
      img.stride[2] = 32;
      
      ext_ref.img = &img;
      ext_ref.num = 1;
      
      aom_codec_control(&ctx_dec, AV1D_SET_EXT_REF_PTR, &ext_ref);
      
      free(y_data);
      free(u_data);
      free(v_data);
    }
  }

  cleanup_codec(&ctx_dec);
  cleanup_codec(&ctx_enc);

  return 0;
}