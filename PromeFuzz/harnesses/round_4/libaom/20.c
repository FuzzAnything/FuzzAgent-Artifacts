// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aomdx.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"

/* Frame buffer callback functions for external frame buffer management */
static int get_frame_buffer_cb(void *cb_priv, size_t min_size,
                               aom_codec_frame_buffer_t *fb) {
    /* Allocate a buffer with the requested minimum size */
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) {
        return -1;
    }
    fb->size = min_size;
    /* Use cb_priv as a simple counter for tracking */
    int *counter = (int *)cb_priv;
    if (counter) {
        (*counter)++;
    }
    return 0;
}

static int release_frame_buffer_cb(void *cb_priv,
                                   aom_codec_frame_buffer_t *fb) {
    if (fb->data) {
        free(fb->data);
        fb->data = NULL;
    }
    /* Use cb_priv as a simple counter for tracking */
    int *counter = (int *)cb_priv;
    if (counter) {
        (*counter)--;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    /* Initialize decoder context */
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    /* Get AV1 decoder interface */
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }
    
    /* Initialize decoder configuration (can be NULL) */
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1; /* Use single thread for fuzzing */
    
    /* Try different initialization flags based on input data */
    aom_codec_flags_t flags = 0;
    /* Note: Not using AOM_CODEC_USE_POSTPROC or AOM_CODEC_USE_ERROR_CONCEALMENT
     * as they are not defined in the provided headers */
    
    /* Initialize decoder with version checking */
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&codec_ctx, iface, &cfg,
                                                         flags, AOM_DECODER_ABI_VERSION);
    
    /* If initialization failed, cleanup and return */
    if (init_result != AOM_CODEC_OK) {
        /* Try to destroy even if init failed (should handle gracefully) */
        aom_codec_destroy(&codec_ctx);
        return 0;
    }
    
    /* Optionally set external frame buffer functions based on input */
    if (size > 1) {
        /* Use second byte to decide whether to use external buffers */
        if (data[1] & 0x01) {
            int cb_priv_counter = 0;
            aom_codec_set_frame_buffer_functions(&codec_ctx,
                                                 get_frame_buffer_cb,
                                                 release_frame_buffer_cb,
                                                 &cb_priv_counter);
        }
    }
    
    /* Decode the input data (skip first few bytes if used for flags) */
    size_t data_offset = (size > 2) ? 2 : 0;
    size_t decode_size = (size > data_offset) ? size - data_offset : 0;
    
    if (decode_size > 0) {
        /* Try multiple decode patterns to explore different states */
        for (int i = 0; i < 2; i++) {
            /* First pass: decode entire buffer */
            if (i == 0) {
                aom_codec_decode(&codec_ctx, data + data_offset, decode_size, NULL);
            }
            /* Second pass: decode in chunks if buffer is large enough */
            else if (decode_size > 10) {
                size_t chunk_size = decode_size / 2;
                aom_codec_decode(&codec_ctx, data + data_offset, chunk_size, NULL);
                aom_codec_decode(&codec_ctx, data + data_offset + chunk_size,
                                 decode_size - chunk_size, NULL);
            }
            
            /* Iterate through decoded frames */
            aom_codec_iter_t iter = NULL;
            aom_image_t *img;
            while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != NULL) {
                /* Access some image properties to explore more code paths */
                (void)img->fmt;
                (void)img->w;
                (void)img->h;
                (void)img->bit_depth;
            }
        }
    } else {
        /* Even with empty data, try decode to test edge cases */
        aom_codec_decode(&codec_ctx, NULL, 0, NULL);
    }
    
    /* Cleanup - destroy codec instance */
    aom_codec_destroy(&codec_ctx);
    
    return 0;
}