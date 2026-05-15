// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/* Dummy frame buffer callbacks for aom_codec_set_frame_buffer_functions */
static int dummy_get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                                     aom_codec_frame_buffer_t *fb) {
    /* Allocate minimum required size */
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    /* Use cb_priv as simple counter */
    int *counter = (int *)cb_priv;
    if (counter) (*counter)++;
    return 0;
}

static int dummy_release_frame_buffer_cb(void *cb_priv, 
                                         aom_codec_frame_buffer_t *fb) {
    if (fb && fb->data) {
        free(fb->data);
        fb->data = NULL;
    }
    int *counter = (int *)cb_priv;
    if (counter) (*counter)--;
    return 0;
}

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    int fb_priv = 0; /* Private data for frame buffer callbacks */
    
    /* Initialize decoder with version checking - use NULL interface for fuzzing */
    res = aom_codec_dec_init_ver(&ctx, NULL, NULL, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        /* Initialization failed, skip further processing */
        return 0;
    }
    
    /* Try setting frame buffer functions (may fail if not supported) */
    aom_codec_set_frame_buffer_functions(&ctx, 
                                         dummy_get_frame_buffer_cb,
                                         dummy_release_frame_buffer_cb,
                                         &fb_priv);
    
    /* Try some codec controls with fuzzed data */
    if (Size > 0) {
        /* Use first byte as control ID (mask to avoid zero) */
        int ctrl_id = (Data[0] & 0x7F) + 1;
        int ctrl_value = 0;
        if (Size > sizeof(int)) {
            memcpy(&ctrl_value, Data + 1, sizeof(int));
        }
        aom_codec_control(&ctx, ctrl_id, ctrl_value);
    }
    
    /* Decode the input data */
    res = aom_codec_decode(&ctx, Data, Size, NULL);
    
    /* Iterate through decoded frames if decode was successful */
    if (res == AOM_CODEC_OK) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            /* Access some image properties to explore state */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
    }
    
    /* Clean up */
    aom_codec_destroy(&ctx);
    
    return 0;
}