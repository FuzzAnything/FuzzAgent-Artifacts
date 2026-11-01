// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>

/* Callback for put_frame matching the expected signature: void (*)(void *, const vpx_image_t *) */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
  (void)user_priv;
  (void)img;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  vpx_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
  if (!decoder_iface) {
    return 0;
  }

  vpx_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  cfg.w = 128;
  cfg.h = 128;

  vpx_codec_err_t err = vpx_codec_dec_init(&ctx, decoder_iface, &cfg, 0);
  if (err != VPX_CODEC_OK) {
    return 0;
  }

  /* Register put_frame callback */
  err = vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
  (void)err;

  /* Decode the fuzzed data */
  err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
  if (err != VPX_CODEC_OK && err != VPX_CODEC_ERROR &&
      err != VPX_CODEC_INVALID_PARAM) {
    /* Some errors are expected with malformed data */
  }

  /* Flush the decoder */
  err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
  (void)err;

  /* Iterate over decoded frames */
  vpx_codec_iter_t iter = NULL;
  vpx_image_t *img;
  while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    /* Frame is valid here, do nothing or minimal checks */
    (void)img;
  }

  /* Cleanup */
  vpx_codec_destroy(&ctx);

  return 0;
}