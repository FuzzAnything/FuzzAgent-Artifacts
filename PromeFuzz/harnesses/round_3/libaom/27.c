// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static int get_frame_buffer_cb(void *cb_priv, size_t min_size,
                               aom_codec_frame_buffer_t *fb) {
  (void)cb_priv;
  (void)min_size;
  (void)fb;
  return 0;
}

static int release_frame_buffer_cb(void *cb_priv,
                                   aom_codec_frame_buffer_t *fb) {
  (void)cb_priv;
  (void)fb;
  return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  aom_codec_dec_cfg_t cfg = {0};
  aom_codec_flags_t flags = 0;

  cfg.threads = (Data[0] % 8) + 1;
  cfg.w = 128;
  cfg.h = 128;

  if (Size % 2 == 0) {
    aom_codec_err_t init_err = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags,
                                                      AOM_DECODER_ABI_VERSION);
    if (init_err != AOM_CODEC_OK) {
      return 0;
    }
  } else {
    aom_codec_err_t init_err = aom_codec_dec_init_ver(&ctx, iface, NULL, flags,
                                                      AOM_DECODER_ABI_VERSION);
    if (init_err != AOM_CODEC_OK) {
      return 0;
    }
  }

  if (Size % 3 == 0) {
    aom_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb,
                                         release_frame_buffer_cb, NULL);
  }

  aom_codec_err_t decode_err = aom_codec_decode(&ctx, Data, Size, NULL);
  if (decode_err == AOM_CODEC_OK) {
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
      (void)img;
    }
  }

  aom_codec_destroy(&ctx);
  return 0;
}