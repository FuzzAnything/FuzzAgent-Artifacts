// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1D_GET_FRAME_SIZE at aomdx.h:492:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_TILE_INFO at aomdx.h:570:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES at aomdx.h:483:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1_INVERT_TILE_DECODE_ORDER at aomdx.h:510:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA at aomcx.h:2135:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER at aomdx.h:519:1 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

/* Helper to initialize a decoder context */
static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
  aom_codec_dec_cfg_t cfg;
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (!iface) return 0;

  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;

  if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  /* Feed some data to the decoder to ensure it is in a valid state for control calls */
  if (size > 0) {
    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame;
    
    aom_codec_decode(ctx, data, size, NULL);
    
    /* Drain decoder */
    while ((frame = aom_codec_get_frame(ctx, &iter)) != NULL) {
      (void)frame;
    }
  }

  return 1;
}

/* Helper to initialize an encoder context */
static int init_encoder(aom_codec_ctx_t *ctx) {
  aom_codec_enc_cfg_t cfg;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  cfg.g_w = 64;
  cfg.g_h = 64;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;

  if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  return 1;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t ctx_dec;
  aom_codec_ctx_t ctx_enc;
  int ret;
  int val_int;
  aom_tile_info tile_info;
  int ref_updates[8]; /* Assuming max 8 refs */

  /* Initialize Decoder */
  memset(&ctx_dec, 0, sizeof(ctx_dec));
  if (init_decoder(&ctx_dec, Data, Size)) {
    /* 1. AOMD_GET_LAST_QUANTIZER */
    ret = aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER(&ctx_dec, AOMD_GET_LAST_QUANTIZER, &val_int);
    (void)ret; (void)val_int;

    /* 2. AV1D_GET_FRAME_SIZE */
    /* Note: AV1D_GET_FRAME_SIZE typically takes a struct aom_frame_size_t* or similar. 
       Since the exact type wasn't fully defined in the prompt's snippet but the error mentioned aom_frame_size_t,
       we use a dummy struct or int array if the type is opaque. 
       However, based on standard libaom, it returns width/height. Let's assume aom_frame_size_t exists or use a struct.
       If aom_frame_size_t is not defined in the provided headers, we might need to define it or use a workaround.
       Given the error 'unknown type name aom_frame_size_t', we will define a compatible struct locally if needed,
       but usually it's in aom_image.h. Let's try to use it. If it fails, we might need to cast.
       Actually, the error log says 'use of undeclared identifier aom_frame_size_t'. 
       We will define a simple struct to match expected usage if it's missing, or rely on the fact that 
       the macro expansion might expect a specific pointer type.
       Let's assume the standard definition: struct { unsigned int width; unsigned int height; };
    */
    struct { unsigned int width; unsigned int height; } frame_size_dummy;
    ret = aom_codec_control_typechecked_AV1D_GET_FRAME_SIZE(&ctx_dec, AV1D_GET_FRAME_SIZE, &frame_size_dummy);
    (void)ret; (void)frame_size_dummy;

    /* 3. AOMD_GET_TILE_INFO */
    ret = aom_codec_control_typechecked_AOMD_GET_TILE_INFO(&ctx_dec, AOMD_GET_TILE_INFO, &tile_info);
    (void)ret; (void)tile_info;

    /* 6. AOMD_GET_LAST_REF_UPDATES */
    ret = aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES(&ctx_dec, AOMD_GET_LAST_REF_UPDATES, ref_updates);
    (void)ret; (void)ref_updates;

    aom_codec_destroy(&ctx_dec);
  }

  /* Initialize Encoder */
  memset(&ctx_enc, 0, sizeof(ctx_enc));
  if (init_encoder(&ctx_enc)) {
    /* 4. AV1_INVERT_TILE_DECODE_ORDER (Boolean flag) */
    int invert_flag = (Data[0] % 2);
    ret = aom_codec_control_typechecked_AV1_INVERT_TILE_DECODE_ORDER(&ctx_enc, AV1_INVERT_TILE_DECODE_ORDER, invert_flag);
    (void)ret;

    /* 5. AV1E_SET_ENABLE_FILTER_INTRA (Boolean flag) */
    int filter_intra = (Size > 1 ? (Data[1] % 2) : 0);
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA(&ctx_enc, AV1E_SET_ENABLE_FILTER_INTRA, filter_intra);
    (void)ret;

    aom_codec_destroy(&ctx_enc);
  }

  return 0;
}