// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR at aomcx.h:2177:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS at aomcx.h:1923:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE at aomcx.h:2342:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE at aomcx.h:2360:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID at aomcx.h:1887:1 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT at aomdx.h:549:1 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_169(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  uint32_t seed = 0;
  if (Size >= 4) {
    memcpy(&seed, Data, 4);
  }

  // Test Encoder Control Functions
  aom_codec_ctx_t enc_ctx = {0};
  aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
  if (enc_iface) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) == AOM_CODEC_OK) {
      // 1. AV1E_SET_FILM_GRAIN_TEST_VECTOR
      // The typechecked macro expects (ctx, ctrl, data). 
      // For this specific control, the 'data' argument is typically an int flag or similar in the macro expansion,
      // but the error indicates the macro expects 'int' for the third arg based on the header definition provided in error.
      // However, the error log shows: AOM_CTRL_USE_TYPE(AV1E_SET_FILM_GRAIN_TEST_VECTOR, int)
      // This implies the third argument must be an int. The previous attempt passed a pointer.
      // Let's pass an int derived from the seed.
      int film_grain_val = (seed >> 8) & 0xFFFFFFFF;
      aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR(&enc_ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, film_grain_val);

      // 2. AOME_SET_NUMBER_SPATIAL_LAYERS
      // Signature: aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS(ctx, ctrl, data)
      int num_spatial = (seed >> 16) & 0xFF;
      if (num_spatial > 0 && num_spatial <= 4) {
        aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS(&enc_ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, num_spatial);
      }

      // 3. AV1E_SET_SVC_FRAME_DROP_MODE
      // Signature: aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE(ctx, ctrl, data)
      unsigned int drop_mode = (seed >> 24) & 0xFF;
      aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE(&enc_ctx, AV1E_SET_SVC_FRAME_DROP_MODE, drop_mode);

      // 4. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
      // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE(ctx, ctrl, data)
      unsigned int low_complexity = (seed >> 2) & 0xFF;
      aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE(&enc_ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, low_complexity);

      // 5. AOME_SET_SPATIAL_LAYER_ID
      // Signature: aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID(ctx, ctrl, data)
      int spatial_id = (seed >> 12) & 0x3;
      aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID(&enc_ctx, AOME_SET_SPATIAL_LAYER_ID, spatial_id);

      cleanup_encoder(&enc_ctx);
    }
  }

  // Test Decoder Control Functions
  aom_codec_ctx_t dec_ctx = {0};
  aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
  if (dec_iface) {
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;

    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) == AOM_CODEC_OK) {
      // 6. AV1D_SET_OPERATING_POINT
      // Signature: aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT(ctx, ctrl, data)
      unsigned int operating_point = (seed >> 4) & 0x3F;
      aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT(&dec_ctx, AV1D_SET_OPERATING_POINT, operating_point);

      cleanup_decoder(&dec_ctx);
    }
  }

  return 0;
}