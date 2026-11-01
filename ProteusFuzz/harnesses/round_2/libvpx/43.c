// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
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
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>

/* Static callback function for frame completion */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Static callback function for slice completion */
/* vpx_image_rect is a struct without a typedef in some versions, so we use struct vpx_image_rect */
static void put_slice_cb(void *user_priv, const vpx_image_t *img, 
                         const struct vpx_image_rect *slice_rect, 
                         const struct vpx_image_rect *frame_rect) {
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)frame_rect;
}

/* Helper to initialize decoder context with fuzzed config */
static vpx_codec_ctx_t* init_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));
    
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    /* Fuzz configuration parameters safely */
    if (size >= 2) {
        cfg.threads = data[0] % 8 + 1; /* 1 to 8 threads */
        cfg.w = (data[1] * 16) + 16;   /* Minimum 16x16 */
        cfg.h = (data[1] * 16) + 16;
    } else {
        cfg.threads = 1;
        cfg.w = 16;
        cfg.h = 16;
    }
    
    /* Use VP8 decoder interface as it's commonly available */
    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    /* Initialize decoder */
    vpx_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;
    
    /* Register frame callback */
    vpx_codec_register_put_frame_cb(ctx, put_frame_cb, (void *)(uintptr_t)(Data[0]));
    
    /* Register slice callback */
    vpx_codec_register_put_slice_cb(ctx, put_slice_cb, (void *)(uintptr_t)(Data[1]));
    
    /* Decode data */
    /* Use a portion of the input as coded data */
    size_t data_len = Size > 100 ? 100 : Size;
    vpx_codec_decode(ctx, Data, (unsigned int)data_len, NULL, 0);
    
    /* Get frames */
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(ctx, &iter)) != NULL) {
        /* Process frame if needed */
        (void)img;
    }
    
    /* Flush decoder */
    vpx_codec_decode(ctx, NULL, 0, NULL, 0);
    
    /* Cleanup */
    vpx_codec_destroy(ctx);
    free(ctx);
    
    return 0;
}