// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

// Helper function defined outside LLVMFuzzerTestOneInput to avoid C syntax errors
static int call_control(aom_codec_ctx_t *c, int ctrl, void *arg) {
  aom_codec_err_t err = aom_codec_control(c, ctrl, arg);
  // We ignore the return value here to allow fuzzing to continue even on errors
  return 1;
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                        const uint8_t *data, size_t size) {
  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 1;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return 1;
  }

  // Fuzz configuration parameters based on input data
  if (size > 0) cfg->g_w = (data[0] % 1024) + 16;
  if (size > 1) cfg->g_h = (data[1] % 1024) + 16;
  
  // Fix: g_time_base -> g_timebase
  if (size > 2) cfg->g_timebase.num = (data[2] % 100) + 1;
  if (size > 3) cfg->g_timebase.den = (data[3] % 100) + 1;
  
  if (size > 4) cfg->g_threads = (data[4] % 4) + 1;
  
  // Fix: gc_lag_in_frames -> g_lag_in_frames
  if (size > 5) cfg->g_lag_in_frames = (data[5] % 20);
  
  if (size > 6) cfg->rc_target_bitrate = (data[6] % 1000) + 100;
  if (size > 7) cfg->rc_min_quantizer = (data[7] % 40) + 1;
  if (size > 8) cfg->rc_max_quantizer = (data[8] % 40) + 1;
  
  // Use int instead of aom_rc_mode_t to avoid type issues
  if (size > 9) {
    int mode = data[9] % 4;
    cfg->rc_end_usage = (int)mode;
  }

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return 1;
  }

  return 0;
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  aom_codec_ctx_t ctx = {0};
  aom_codec_enc_cfg_t cfg = {0};

  if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
    return 0;
  }

  // 1. AV1E_SET_EXTERNAL_RATE_CONTROL
  // Requires aom_rc_funcs_t structure
  if (Size > 10) {
    aom_rc_funcs_t rc_funcs = {0};
    rc_funcs.rc_type = (aom_rc_type_t)(Data[10] % 7); // 0 to 6
    // Callbacks are left NULL for fuzzing purposes to test error handling
    // Fix: Use AV1E_SET_EXTERNAL_RATE_CONTROL instead of AOM_CODEC_SET_...
    call_control(&ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
  }

  // 2. AV1E_SET_ENABLE_INTRABC
  // Takes an int (0 or 1)
  if (Size > 11) {
    int enable_intrabc = (Data[11] % 2);
    call_control(&ctx, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);
  }

  // 3. AV1E_SET_ENABLE_CFL_INTRA
  // Takes an int (0 or 1)
  if (Size > 12) {
    int enable_cfl = (Data[12] % 2);
    call_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, &enable_cfl);
  }

  // 4. AV1E_SET_CHROMA_SAMPLE_POSITION
  // Takes an int (AOM_CSP_UNKNOWN, AOM_CSP_VERTICAL, AOM_CSP_COLOCATED)
  if (Size > 13) {
    int csp = (Data[13] % 3); // 0, 1, or 2
    call_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, &csp);
  }

  // 5. AV1E_SET_TUNE_CONTENT
  // Takes an int (AOM_CONTENT_DEFAULT, AOM_CONTENT_SCREEN, AOM_CONTENT_FILM)
  if (Size > 14) {
    int tune_content = (Data[14] % 3);
    call_control(&ctx, AV1E_SET_TUNE_CONTENT, &tune_content);
  }

  // 6. AOME_SET_TUNING
  // Takes an int (AOM_TUNE_UNKNOWN, AOM_TUNE_PSNR, AOM_TUNE_SSIM)
  if (Size > 15) {
    int tuning = (Data[15] % 3);
    call_control(&ctx, AOME_SET_TUNING, &tuning);
  }

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}