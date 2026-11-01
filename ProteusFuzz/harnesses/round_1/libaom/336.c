// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx != NULL && ctx->iface != NULL) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_336(const uint8_t *Data, size_t Size) {
  if (Size < 8) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t res;
  int8_t val_int8;
  int32_t val_int32;
  aom_svc_ref_frame_comp_pred_t svc_pred;

  // Initialize configuration
  res = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Initialize codec context
  res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Extract values from input data to drive fuzzing
  val_int8 = (int8_t)Data[0];
  val_int32 = (int32_t)(Data[1] << 24 | Data[2] << 16 | Data[3] << 8 | Data[4]);

  // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH
  // Expected: int32_t value (0 or 1)
  res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val_int8);
  (void)res;

  // 2. aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE
  // Expected: int32_t value (partition size)
  res = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val_int32);
  (void)res;

  // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
  // Expected: int32_t value (0 or 1)
  res = aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val_int8);
  (void)res;

  // 4. aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_COMP_PRED
  // Expected: aom_svc_ref_frame_comp_pred_t pointer
  svc_pred.use_comp_pred[0] = (int)Data[5];
  svc_pred.use_comp_pred[1] = (int)Data[6];
  svc_pred.use_comp_pred[2] = (int)Data[7];
  res = aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &svc_pred);
  (void)res;

  // 5. aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR
  // Expected: const char * (file path or data)
  // We use a dummy file as per instructions if needed, but here we pass a pointer to data if valid
  if (Size > 10) {
    // Ensure null termination for safety if treating as string
    char *dummy_str = malloc(Size - 8);
    if (dummy_str) {
      memcpy(dummy_str, &Data[8], Size - 8);
      dummy_str[Size - 8] = '\0';
      res = aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, dummy_str);
      (void)res;
      free(dummy_str);
    }
  }

  // 6. aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY
  // Expected: int32_t value (0 or 1)
  res = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val_int8);
  (void)res;

  cleanup_codec(&ctx);
  return 0;
}