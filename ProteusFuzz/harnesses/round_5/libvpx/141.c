// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8.h>

static vpx_codec_ctx_t g_ctx_storage;
static vpx_codec_ctx_t *g_ctx = NULL;
static vpx_codec_iter_t g_iter = NULL;
static int g_initialized = 0;

static void put_frame_callback(void *user_priv, const vpx_image_t *img) {
  (void)user_priv;
  (void)img;
}

static void cleanup_context(void) {
  if (g_ctx != NULL) {
    vpx_codec_destroy(g_ctx);
    g_ctx = NULL;
  }
  g_initialized = 0;
  g_iter = NULL;
}

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  if (!g_initialized) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (iface == NULL) {
      return 0;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 1920;
    cfg.h = 1080;

    g_ctx = &g_ctx_storage;
    memset(g_ctx, 0, sizeof(*g_ctx));

    if (vpx_codec_dec_init(g_ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
      return 0;
    }

    if (vpx_codec_register_put_frame_cb(g_ctx, put_frame_callback, NULL) != VPX_CODEC_OK) {
      cleanup_context();
      return 0;
    }

    g_initialized = 1;
  }

  if (g_ctx == NULL) {
    return 0;
  }

  vpx_codec_err_t res = vpx_codec_decode(g_ctx, Data, Size, NULL, 0);
  if (res != VPX_CODEC_OK && res != VPX_CODEC_ERROR) {
    if (res == VPX_CODEC_MEM_ERROR || res == VPX_CODEC_CORRUPT_FRAME) {
      cleanup_context();
      return 0;
    }
  }

  g_iter = NULL;
  while (vpx_codec_get_frame(g_ctx, &g_iter) != NULL) {
  }

  if (Size > 4) {
    uint32_t action = *(const uint32_t *)Data;
    if (action % 3 == 0) {
      vpx_codec_iter_t iter = NULL;
      vpx_image_t *img = vpx_codec_get_frame(g_ctx, &iter);
      (void)img;
    }
  }

  return 0;
}