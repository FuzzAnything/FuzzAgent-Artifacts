// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

static uint32_t get_uint32(const uint8_t *data, size_t size, size_t offset) {
  if (offset + sizeof(uint32_t) > size) {
    return 0;
  }
  uint32_t val = 0;
  memcpy(&val, data + offset, sizeof(uint32_t));
  return val;
}

int LLVMFuzzerTestOneInput_248(const uint8_t *Data, size_t Size) {
  if (Size < 16) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (iface == NULL) {
    return 0;
  }

  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, 0);
  cfg.g_w = 128;
  cfg.g_h = 128;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 100;

  aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
  if (ret != AOM_CODEC_OK) {
    return 0;
  }

  uint32_t val1 = get_uint32(Data, Size, 0);
  uint32_t val2 = get_uint32(Data, Size, 4);
  uint32_t val3 = get_uint32(Data, Size, 8);
  uint32_t val4 = get_uint32(Data, Size, 12);

  // Fuzz AV1E_SET_MAX_PARTITION_SIZE
  ret = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val1);
  (void)ret;

  // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val2);
  (void)ret;

  // Fuzz AV1E_SET_MIN_PARTITION_SIZE
  ret = aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, val3);
  (void)ret;

  // Fuzz AV1E_SET_SVC_REF_FRAME_COMP_PRED
  aom_svc_ref_frame_comp_pred_t svc_pred;
  svc_pred.use_comp_pred[0] = (val1 & 0xFF) ? 1 : 0;
  svc_pred.use_comp_pred[1] = ((val1 >> 8) & 0xFF) ? 1 : 0;
  svc_pred.use_comp_pred[2] = ((val1 >> 16) & 0xFF) ? 1 : 0;
  ret = aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &svc_pred);
  (void)ret;

  // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val4);
  (void)ret;

  // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val4);
  (void)ret;

  cleanup_codec(&ctx);
  return 0;
}