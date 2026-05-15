// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"

/* Frame buffer callback functions */
static int get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    fb->data = (uint8_t *)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

/* Control ID values for aom_codec_control */
enum {
    AOMD_GET_LAST_REF_UPDATES = 0x1002,
    AOMD_GET_FRAME_CORRUPTED = 0x1001,
    AOMD_GET_LAST_QUANTIZER = 0x1003,
    AOME_SET_CPUUSED = 0x2001,
    AOME_SET_ENABLEAUTOALTREF = 0x2002
};

/* Declare the AV1 decoder interface function */
extern aom_codec_iface_t aom_codec_av1_dx_algo;
#define aom_codec_av1_dx() (&aom_codec_av1_dx_algo)

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0};
    aom_codec_stream_info_t si = {0};
    
    /* Initialize decoder */
    aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    /* Use first byte to determine fuzzing path */
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    /* Try setting frame buffer functions (50% chance) */
    if ((selector & 0x01) && offset < Size) {
        aom_codec_set_frame_buffer_functions(&ctx, get_frame_buffer, release_frame_buffer, NULL);
    }
    
    /* Decode available data */
    if (offset < Size) {
        size_t decode_size = Size - offset;
        if (decode_size > 4096) decode_size = 4096; /* Limit decode size */
        aom_codec_decode(&ctx, Data + offset, decode_size, NULL);
    }
    
    /* Get stream info */
    aom_codec_get_stream_info(&ctx, &si);
    
    /* Try various control operations */
    int ctrl_value = 0;
    switch (selector & 0x07) {
        case 0: aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &ctrl_value); break;
        case 1: aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &ctrl_value); break;
        case 2: aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &ctrl_value); break;
        case 3: aom_codec_control(&ctx, AOME_SET_CPUUSED, 8); break;
        case 4: aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, 1); break;
        default: break;
    }
    
    /* Clean up */
    aom_codec_destroy(&ctx);
    
    return 0;
}