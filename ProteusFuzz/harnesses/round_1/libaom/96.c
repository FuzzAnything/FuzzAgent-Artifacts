// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static void cleanup_codec(aom_codec_ctx_t *codec) {
  if (codec) {
    aom_codec_destroy(codec);
  }
}

static int init_encoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
  const aom_codec_iface_t *iface;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t ret;

  // Get the AV1 codec interface (handles both encoder and decoder)
  iface = aom_codec_av1_cx();
  if (!iface) {
    return -1;
  }

  ret = aom_codec_enc_config_default(iface, &cfg, 0);
  if (ret != AOM_CODEC_OK) {
    return -1;
  }

  // Use fuzz data to set some configuration parameters if size permits
  if (size >= 2) {
    cfg.g_w = (data[0] % 2048) + 16;
    cfg.g_h = (data[1] % 2048) + 16;
    if (cfg.g_w > 4096) cfg.g_w = 4096;
    if (cfg.g_h > 4096) cfg.g_h = 4096;
  } else {
    cfg.g_w = 128;
    cfg.g_h = 128;
  }

  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;

  ret = aom_codec_enc_init(codec, iface, &cfg, 0);
  if (ret != AOM_CODEC_OK) {
    return -1;
  }

  return 0;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
  aom_codec_ctx_t codec;
  aom_codec_err_t ret;
  int arg_val = 0;

  if (Size < 1) {
    return 0;
  }

  // Initialize codec context
  memset(&codec, 0, sizeof(codec));

  if (init_encoder(&codec, Data, Size) != 0) {
    return 0;
  }

  // Extract values from fuzz data to use as control arguments
  int idx = 0;
  if (Size > idx) {
    arg_val = Data[idx];
    idx++;
  }

  // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
  ret = aom_codec_control(&codec, AV1E_SET_ENABLE_FLIP_IDTX, arg_val % 2);
  (void)ret;

  if (Size > idx) {
    arg_val = Data[idx];
    idx++;
  }
  // Fuzz AV1E_SET_INTRA_DCT_ONLY
  ret = aom_codec_control(&codec, AV1E_SET_INTRA_DCT_ONLY, arg_val % 2);
  (void)ret;

  if (Size > idx) {
    arg_val = Data[idx];
    idx++;
  }
  // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
  ret = aom_codec_control(&codec, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, arg_val);
  (void)ret;

  if (Size > idx) {
    arg_val = Data[idx];
    idx++;
  }
  // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
  ret = aom_codec_control(&codec, AV1E_SET_ENABLE_PAETH_INTRA, arg_val % 2);
  (void)ret;

  if (Size > idx) {
    arg_val = Data[idx];
    idx++;
  }
  // Fuzz AV1E_SET_LOOPFILTER_CONTROL
  ret = aom_codec_control(&codec, AV1E_SET_LOOPFILTER_CONTROL, arg_val);
  (void)ret;

  if (Size > idx) {
    arg_val = Data[idx];
    idx++;
  }
  // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
  ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, arg_val % 2);
  (void)ret;

  cleanup_codec(&codec);
  return 0;
}