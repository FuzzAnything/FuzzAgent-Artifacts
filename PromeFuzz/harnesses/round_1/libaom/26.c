// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_codec.h"

/* Frame buffer callback functions */
static int get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    /* Simple implementation that allocates memory for frame buffers */
    void *buf = malloc(min_size);
    if (!buf) return -1;
    fb->data = (uint8_t *)buf;
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    /* Free the allocated memory */
    free(fb->data);
    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_err_t res;
    
    /* Initialize decoder with default configuration */
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        /* Initialization failed, skip further processing */
        return 0;
    }
    
    /* Randomly decide whether to set external frame buffer functions */
    if (Size > 0 && (Data[0] & 0x01)) {
        void *cb_priv = NULL;
        res = aom_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb, 
                                                   release_frame_buffer_cb, cb_priv);
        /* Ignore result - just testing the API call */
    }
    
    /* Decode the input data */
    void *user_priv = NULL;
    size_t decode_size = Size;
    const uint8_t *decode_data = Data;
    
    /* Use a subset of data for decoding if available */
    if (Size > 1) {
        /* Use first byte to determine how much data to use for decoding */
        size_t max_decode = (Data[0] % (Size - 1)) + 1;
        if (max_decode < Size) {
            decode_size = max_decode;
            decode_data = Data + 1;
        }
    }
    
    res = aom_codec_decode(&ctx, decode_data, decode_size, user_priv);
    
    /* Iterate through decoded frames */
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Access some image properties to explore more states */
        volatile int width = img->d_w;
        volatile int height = img->d_h;
        volatile aom_img_fmt_t fmt = img->fmt;
        (void)width;
        (void)height;
        (void)fmt;
    }
    
    /* Try some control calls with random parameters */
    if (Size > 2) {
        /* Use data bytes to generate control IDs and parameters */
        int ctrl_id = (int)(Data[Size > 2 ? 1 : 0]) | 
                     ((int)(Data[Size > 3 ? 2 : 0]) << 8);
        
        /* Try different parameter types based on data */
        if (Size > 4) {
            int int_param = (int)Data[Size > 4 ? 3 : 0];
            res = aom_codec_control(&ctx, ctrl_id, int_param);
        } else {
            void *ptr_param = NULL;
            res = aom_codec_control(&ctx, ctrl_id, ptr_param);
        }
    }
    
    /* Clean up */
    res = aom_codec_destroy(&ctx);
    
    return 0;
}