// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_decoder.h"
#include "vp8dx.h"

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *rect, 
                               const vpx_image_rect_t *clipped_rect) {
  // Dummy callback - do nothing
  (void)user_priv;
  (void)img;
  (void)rect;
  (void)clipped_rect;
}

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
  // Dummy callback - do nothing
  (void)user_priv;
  (void)img;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  // Initialize decoder context
  vpx_codec_ctx_t ctx;
  vpx_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  
  // Choose codec based on input - use VP8 decoder interface
  vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
  
  // Initialize decoder
  vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, 
                                               VPX_DECODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    return 0;
  }
  
  // Register callbacks (if supported)
  if (Data[0] & 2) {
    vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
  }
  if (Data[0] & 4) {
    vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
  }
  
  // Decode the input data
  size_t offset = 1; // Skip first byte used for flags
  while (offset < Size) {
    size_t chunk_size = (Size - offset < 1024) ? Size - offset : 1024;
    vpx_codec_decode(&ctx, Data + offset, (unsigned int)chunk_size, NULL, 0);
    offset += chunk_size;
  }
  
  // Flush decoder
  vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
  
  // Get stream info
  vpx_codec_stream_info_t si;
  memset(&si, 0, sizeof(si));
  si.sz = sizeof(si);
  vpx_codec_get_stream_info(&ctx, &si);
  
  // Get decoded frames
  vpx_codec_iter_t iter = NULL;
  const vpx_image_t *img;
  while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    // Access some image properties to explore more states
    (void)img->fmt;
    (void)img->w;
    (void)img->h;
    (void)img->d_w;
    (void)img->d_h;
    (void)img->bit_depth;
  }
  
  // Cleanup
  vpx_codec_destroy(&ctx);
  
  return 0;
}