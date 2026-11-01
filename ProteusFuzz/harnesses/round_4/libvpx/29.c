// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
  (void)user_priv;
  (void)img;
}

static void put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const struct vpx_image_rect *rect,
                         const struct vpx_image_rect *slice_rect) {
  (void)user_priv;
  (void)img;
  (void)rect;
  (void)slice_rect;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
  vpx_codec_ctx_t ctx;
  vpx_codec_dec_cfg_t cfg;
  vpx_codec_iter_t iter = NULL;
  vpx_codec_err_t res;
  const vpx_codec_iface_t *iface;

  // Guard against empty input
  if (Size == 0) {
    return 0;
  }

  // Initialize configuration
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  cfg.w = 128;
  cfg.h = 128;

  // Initialize context
  memset(&ctx, 0, sizeof(ctx));

  // Get VP9 decoder interface
  iface = vpx_codec_vp9_dx();
  if (!iface) {
    return 0;
  }

  // Initialize decoder
  res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
  if (res != VPX_CODEC_OK) {
    return 0;
  }

  // Register callbacks
  vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
  vpx_codec_register_put_slice_cb(&ctx, put_slice_cb, NULL);

  // Decode the data
  res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
  // Ignore return value for fuzzing purposes, we want to explore states

  // Iterate over decoded frames
  while ((vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    // Frame processing happens in callback, nothing to do here
  }

  // Flush the decoder
  res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
  (void)res;

  // Cleanup
  vpx_codec_destroy(&ctx);

  return 0;
}