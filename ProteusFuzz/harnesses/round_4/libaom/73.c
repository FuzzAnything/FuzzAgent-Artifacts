// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int create_codec_context(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface,
                                aom_codec_enc_cfg_t *cfg, int flags) {
  if (ctx == NULL || iface == NULL || cfg == NULL) {
    return 0;
  }
  if (aom_codec_enc_init(ctx, iface, cfg, flags) != AOM_CODEC_OK) {
    return 0;
  }
  return 1;
}

static void destroy_codec_context(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

static aom_ext_part_funcs_t create_dummy_ext_part_funcs(void) {
  aom_ext_part_funcs_t funcs = {0};
  funcs.decision_mode = AOM_EXT_PART_WHOLE_TREE;
  funcs.priv = NULL;
  return funcs;
}

static aom_rc_funcs_t create_dummy_rc_funcs(void) {
  aom_rc_funcs_t funcs = {0};
  funcs.rc_type = AOM_RC_NONE;
  funcs.priv = NULL;
  return funcs;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
  if (Data == NULL || Size == 0) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (iface == NULL) {
    return 0;
  }

  aom_codec_enc_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use fuzzer data to set some config parameters safely
  if (Size > 0) cfg.g_w = (Data[0] % 128) + 16;
  if (Size > 1) cfg.g_h = (Data[1] % 128) + 16;
  // Fix: Use g_timebase instead of g_time_base
  if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
  if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
  // Fix: Remove g_fps_num and g_fps_den as they don't exist in this struct version
  // Use rc_target_bitrate instead
  if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 1000) + 100;

  if (!create_codec_context(&ctx, iface, &cfg, 0)) {
    return 0;
  }

  // Fuzz control commands
  int ctrl_val;
  if (Size > 5) ctrl_val = Data[5] % 2; // 0 or 1

  // 1. AV1E_SET_ENABLE_AB_PARTITIONS
  aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, ctrl_val);

  // 2. AV1E_SET_ENABLE_REF_FRAME_MVS
  aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, ctrl_val);

  // 3. AV1E_SET_EXTERNAL_PARTITION
  // Fix: Use AV1E_SET_EXTERNAL_PARTITION instead of AV1E_SET_EXTERNAL_PARTITION_MODEL
  aom_ext_part_funcs_t ext_part_funcs = create_dummy_ext_part_funcs();
  if (Size > 6) ext_part_funcs.decision_mode = (Data[6] % 2);
  aom_codec_control(&ctx, AV1E_SET_EXTERNAL_PARTITION, &ext_part_funcs);

  // 4. AV1E_SET_ENABLE_FILTER_INTRA
  aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, ctrl_val);

  // 5. AV1E_SET_RTC_EXTERNAL_RC
  aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, ctrl_val);

  // 6. AV1E_SET_EXTERNAL_RATE_CONTROL
  aom_rc_funcs_t rc_funcs = create_dummy_rc_funcs();
  if (Size > 7) rc_funcs.rc_type = (aom_rc_type_t)(Data[7] % 7);
  aom_codec_control(&ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);

  // Cleanup
  destroy_codec_context(&ctx);

  return 0;
}