// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

/* Dummy callback functions */
static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *rect, const vpx_image_rect_t *valid_rect) {
  (void)user_priv;
  (void)img;
  (void)rect;
  (void)valid_rect;
}

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
  (void)user_priv;
  (void)img;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
  /* Minimum size check */
  if (size < 1) return 0;

  /* Initialize decoder context */
  vpx_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  /* Choose codec interface based on input byte */
  vpx_codec_iface_t *iface;
  if (data[0] & 1) {
    iface = vpx_codec_vp9_dx();
  } else {
    iface = vpx_codec_vp8_dx();
  }
  
  /* Initialize decoder configuration */
  vpx_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = (data[0] >> 1) & 3; /* 0-3 threads */
  cfg.w = 640;
  cfg.h = 480;

  /* Initialize decoder */
  vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    /* Initialization failed, skip further processing */
    return 0;
  }

  /* Try registering callbacks based on input data */
  if (size > 1) {
    uint8_t cb_flags = data[1];
    
    if (cb_flags & 0x01) {
      vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    }
    
    if (cb_flags & 0x02) {
      vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    }
  }

  /* Decode the input data */
  if (size > 2) {
    /* Use remaining data for decoding */
    const uint8_t *decode_data = data + 2;
    size_t decode_size = size - 2;
    
    /* Try different decode strategies based on input */
    if (data[0] & 0x04) {
      /* Fragmented decode: split into two parts */
      size_t first_part = decode_size / 2;
      vpx_codec_decode(&ctx, decode_data, (unsigned int)first_part, NULL, 0);
      vpx_codec_decode(&ctx, decode_data + first_part, (unsigned int)(decode_size - first_part), NULL, 0);
    } else {
      /* Single decode */
      vpx_codec_decode(&ctx, decode_data, (unsigned int)decode_size, NULL, 0);
    }
    
    /* Flush decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
  }

  /* Try to get stream information */
  vpx_codec_stream_info_t si;
  memset(&si, 0, sizeof(si));
  si.sz = sizeof(si);
  vpx_codec_get_stream_info(&ctx, &si);

  /* Iterate through decoded frames */
  vpx_codec_iter_t iter = NULL;
  const vpx_image_t *img;
  while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    /* Access some image properties to explore more states */
    (void)img->fmt;
    (void)img->w;
    (void)img->h;
    (void)img->d_w;
    (void)img->d_h;
    (void)img->planes[0];
    (void)img->stride[0];
  }

  /* Clean up */
  vpx_codec_destroy(&ctx);

  return 0;
}