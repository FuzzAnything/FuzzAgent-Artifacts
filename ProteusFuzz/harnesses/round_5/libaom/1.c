// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
    free(ctx);
  }
}

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
  aom_codec_err_t ret;
  if (!ctx) return 0;

  switch (control_id) {
    case AV1E_SET_MAX_PARTITION_SIZE:
      ret = aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, value);
      break;
    case AV1E_SET_FORCE_VIDEO_MODE:
      ret = aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, value);
      break;
    case AV1E_SET_ENABLE_KEYFRAME_FILTERING:
      ret = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, value);
      break;
    case AV1E_SET_ENABLE_CDEF:
      ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, value);
      break;
    case AV1E_SET_ENABLE_QM:
      ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, value);
      break;
    case AV1E_SET_MTU:
      ret = aom_codec_control(ctx, AV1E_SET_MTU, value);
      break;
    default:
      return 0;
  }

  return (ret == AOM_CODEC_OK);
}

static int safe_read_int(const uint8_t *Data, size_t Size, size_t *idx) {
  if (*idx + 2 > Size) return 0;
  int val = (Data[*idx] << 8) | Data[*idx + 1];
  *idx += 2;
  return val;
}

static int safe_read_byte(const uint8_t *Data, size_t Size, size_t *idx) {
  if (*idx + 1 > Size) return 0;
  int val = Data[*idx];
  *idx += 1;
  return val;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
  if (Size < 10) return 0;

  aom_codec_ctx_t *ctx = NULL;
  aom_codec_iface_t *iface = NULL;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t ret;
  int control_id;
  int value;
  size_t idx = 0;

  // Extract control ID and value from input safely
  control_id = safe_read_byte(Data, Size, &idx);
  value = safe_read_int(Data, Size, &idx);
  if (control_id == 0 && value == 0) return 0; // Early exit if parsing failed

  // Initialize encoder config
  aom_codec_enc_config_default(NULL, &cfg, 0);
  
  // Safely read config parameters
  cfg.g_w = safe_read_int(Data, Size, &idx);
  cfg.g_h = safe_read_int(Data, Size, &idx);
  
  int tb_num = safe_read_int(Data, Size, &idx);
  int tb_den = safe_read_int(Data, Size, &idx);
  cfg.g_timebase.num = (tb_num == 0) ? 1 : tb_num;
  cfg.g_timebase.den = (tb_den == 0) ? 1 : tb_den;
  
  cfg.rc_target_bitrate = safe_read_int(Data, Size, &idx);
  cfg.rc_min_quantizer = safe_read_byte(Data, Size, &idx);
  cfg.rc_max_quantizer = safe_read_byte(Data, Size, &idx);

  // Ensure minimum resolution and bitrate to avoid invalid config errors
  if (cfg.g_w < 16) cfg.g_w = 16;
  if (cfg.g_h < 16) cfg.g_h = 16;
  if (cfg.rc_target_bitrate < 1) cfg.rc_target_bitrate = 1;
  if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) cfg.rc_max_quantizer = cfg.rc_min_quantizer;

  // Get AV1 encoder interface
  iface = aom_codec_av1_cx();
  if (!iface) return 0;

  ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return 0;

  ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
  if (ret != AOM_CODEC_OK) {
    cleanup_codec(ctx);
    return 0;
  }

  // Fuzz the target control functions
  fuzz_codec_control(ctx, control_id, value);

  // Additional diverse invocations with varied values
  while (idx + 2 <= Size) {
    int val1 = safe_read_int(Data, Size, &idx);
    fuzz_codec_control(ctx, control_id, val1);
  }

  cleanup_codec(ctx);
  return 0;
}