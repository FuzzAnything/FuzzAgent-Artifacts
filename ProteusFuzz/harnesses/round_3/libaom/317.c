// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static int init_decoder(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface,
                        const aom_codec_dec_cfg_t *cfg, int flags) {
  if (!ctx || !iface) return -1;
  return aom_codec_dec_init(ctx, iface, cfg, flags);
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface,
                        const aom_codec_enc_cfg_t *cfg, int flags) {
  if (!ctx || !iface) return -1;
  return aom_codec_enc_init(ctx, iface, cfg, flags);
}

int LLVMFuzzerTestOneInput_317(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  aom_codec_ctx_t decoder_ctx = {0};
  aom_codec_ctx_t encoder_ctx = {0};
  aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
  aom_codec_iface_t *enc_iface = aom_codec_av1_cx();

  if (!dec_iface || !enc_iface) return 0;

  aom_codec_dec_cfg_t dec_cfg = {0};
  dec_cfg.threads = (Data[0] % 16) + 1;
  dec_cfg.w = (Data[1] % 1024) + 16;
  dec_cfg.h = ((Data[1] >> 4) % 1024) + 16;

  aom_codec_enc_cfg_t enc_cfg = {0};
  aom_codec_enc_config_default(enc_iface, &enc_cfg, 0);
  enc_cfg.g_w = (Data[0] % 1024) + 16;
  enc_cfg.g_h = ((Data[0] >> 4) % 1024) + 16;
  enc_cfg.g_threads = (Data[1] % 16) + 1;

  if (init_decoder(&decoder_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
    cleanup_codec(&decoder_ctx);
    decoder_ctx = (aom_codec_ctx_t){0};
  }

  if (init_encoder(&encoder_ctx, enc_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
    cleanup_codec(&encoder_ctx);
    encoder_ctx = (aom_codec_ctx_t){0};
  }

  // Fuzz AOMD_GET_LAST_QUANTIZER
  if (decoder_ctx.priv) {
    int quantizer;
    aom_codec_control(&decoder_ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
  }

  // Fuzz AOME_GET_LAST_QUANTIZER
  if (encoder_ctx.priv) {
    int quantizer;
    aom_codec_control(&encoder_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
  }

  // Fuzz AOMD_GET_BASE_Q_IDX
  if (decoder_ctx.priv) {
    int base_q_idx;
    aom_codec_control(&decoder_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
  }

  // Fuzz AOME_GET_LAST_QUANTIZER_64
  if (encoder_ctx.priv) {
    int64_t quantizer64;
    aom_codec_control(&encoder_ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer64);
  }

  // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
  if (encoder_ctx.priv) {
    int cdef_strength;
    aom_codec_control(&encoder_ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &cdef_strength);
  }

  // Fuzz AV1D_SET_EXT_REF_PTR
  if (decoder_ctx.priv && Size > 4) {
    av1_ext_ref_frame_t ext_ref = {0};
    // Create a dummy image for external reference
    aom_image_t img = {0};
    img.w = 64;
    img.h = 64;
    img.fmt = AOM_IMG_FMT_I420;
    img.bit_depth = 8;
    img.planes[0] = malloc(64 * 64);
    img.planes[1] = malloc(64 * 32);
    img.planes[2] = malloc(64 * 32);
    if (img.planes[0] && img.planes[1] && img.planes[2]) {
      img.stride[0] = 64;
      img.stride[1] = 32;
      img.stride[2] = 32;
      ext_ref.img = &img;
      ext_ref.num = 1;
      aom_codec_control(&decoder_ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
      free(img.planes[0]);
      free(img.planes[1]);
      free(img.planes[2]);
    }
  }

  cleanup_codec(&decoder_ctx);
  cleanup_codec(&encoder_ctx);

  return 0;
}