// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aomdx.h"

/*
 * Fuzz driver for libaom decoder APIs.
 * Targets:
 * - aom_codec_dec_init_ver
 * - aom_codec_decode
 * - aom_codec_get_frame
 * - aom_codec_get_stream_info
 * - aom_codec_set_frame_buffer_functions
 * - aom_codec_control (specifically AOMD_GET_FRAME_CORRUPTED)
 */

static aom_codec_ctx_t ctx;
static aom_codec_iter_t iter;
static aom_codec_stream_info_t si;
static aom_codec_dec_cfg_t cfg;
static aom_codec_flags_t flags;

/* External buffer management for aom_codec_set_frame_buffer_functions */
static int custom_get_frame_buffer(void *cb_priv, unsigned long align,
                                   struct aom_codec_frame_buffer *fb) {
  (void)cb_priv;
  (void)align;
  (void)fb;
  /* Return 0 to indicate failure to provide buffer, forcing internal allocation if possible */
  return 0;
}

static int custom_release_frame_buffer(void *cb_priv, struct aom_codec_frame_buffer *fb) {
  (void)cb_priv;
  (void)fb;
  /* No-op release */
  return 0;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
  /* Reset state for each input */
  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));
  memset(&si, 0, sizeof(si));
  iter = NULL;
  flags = 0;

  /* Get decoder interface using the correct symbol from aomdx.h */
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (iface == NULL) {
    return 0;
  }

  /* Initialize configuration */
  cfg.threads = 1;
  cfg.w = 0;
  cfg.h = 0;
  cfg.allow_lowbitdepth = 1;

  /* Initialize decoder */
  aom_codec_err_t ret = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
  if (ret != AOM_CODEC_OK) {
    /* Initialization failed, try with different flags or skip */
    if (ret == AOM_CODEC_ABI_MISMATCH || ret == AOM_CODEC_INCAPABLE) {
      return 0;
    }
    /* Other errors might be recoverable or indicate bad input, but we continue to test error paths */
  }

  /* Try to set external frame buffer functions */
  if (ret == AOM_CODEC_OK) {
    aom_codec_set_frame_buffer_functions(&ctx, custom_get_frame_buffer, custom_release_frame_buffer, NULL);
  }

  /* Decode data if available */
  if (Size > 0) {
    /* Attempt to decode the fuzzed input */
    aom_codec_decode(&ctx, Data, Size, NULL);
  }

  /* Get stream info */
  if (ret == AOM_CODEC_OK) {
    aom_codec_get_stream_info(&ctx, &si);
  }

  /* Iterate over decoded frames */
  if (ret == AOM_CODEC_OK) {
    aom_image_t *img = NULL;
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
      /* Process frame if needed, here we just check validity */
      (void)img;
    }
  }

  /* Try to get frame corrupted status via control */
  int is_corrupted = 0;
  if (ret == AOM_CODEC_OK) {
    aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &is_corrupted);
  }

  /* Cleanup */
  aom_codec_destroy(&ctx);

  return 0;
}