// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
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
#include <vpx/vpx_codec.h>

/* Dummy callback for frame buffer allocation */
/* The signature must match vpx_get_frame_buffer_cb_fn_t:
   int (*)(void *cb_priv, unsigned long frame_id, vpx_codec_frame_buffer_t *buf)
*/
static int get_frame_buffer_cb(void *cb_priv, unsigned long frame_id, vpx_codec_frame_buffer_t *buf) {
  (void)cb_priv;
  (void)frame_id;
  (void)buf;
  /* Return 0 to indicate success in providing a buffer.
     In a real scenario, we would allocate memory and fill 'buf'.
     For fuzzing, returning success allows the decoder to proceed,
     potentially exposing issues with uninitialized buffer handling.
  */
  return 0;
}

/* Dummy callback for frame buffer release */
static int release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *buf) {
  (void)cb_priv;
  (void)buf;
  return 0;
}

/* Dummy callback for frame completion notification */
static void put_frame_cb(void *cb_priv, const vpx_image_t *img) {
  (void)cb_priv;
  (void)img;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  vpx_codec_ctx_t ctx;
  vpx_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  cfg.w = 128;
  cfg.h = 128;

  vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
  if (!iface) {
    return 0;
  }

  vpx_codec_err_t res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
  if (res != VPX_CODEC_OK) {
    return 0;
  }

  /* Register frame buffer functions */
  res = vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb,
                                             release_frame_buffer_cb, (void *)0x1234);
  if (res == VPX_CODEC_OK) {
    /* Register put frame callback */
    vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, (void *)0x5678);
  }

  /* Decode the fuzzed data */
  res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
  
  /* Check for errors and get details */
  if (res != VPX_CODEC_OK) {
    const char *detail = vpx_codec_error_detail(&ctx);
    (void)detail; /* Suppress unused warning, detail is just for inspection */
  }

  /* Iterate over decoded frames */
  vpx_codec_iter_t iter = NULL;
  vpx_image_t *img;
  while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    /* Process frame if needed, here we just ignore it */
    (void)img;
  }

  /* Flush the decoder */
  res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
  if (res != VPX_CODEC_OK) {
    (void)vpx_codec_error_detail(&ctx);
  }

  /* Get any remaining frames after flush */
  iter = NULL;
  while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    (void)img;
  }

  vpx_codec_destroy(&ctx);
  return 0;
}