// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

/* Dummy frame buffer callbacks for testing aom_codec_set_frame_buffer_functions */
static int dummy_get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1; /* Indicate failure to allocate */
}

static int dummy_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_stream_info_t si;
    aom_codec_err_t res;
    
    /* Initialize with default values */
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&si, 0, sizeof(si));
    
    /* Get AV1 decoder interface */
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }
    
    /* Use fuzzer data to set configuration parameters if we have enough data */
    if (Size >= sizeof(cfg)) {
        memcpy(&cfg, Data, sizeof(cfg));
        Data += sizeof(cfg);
        Size -= sizeof(cfg);
    } else {
        /* Use some default values */
        cfg.threads = 1;
        cfg.w = 128;
        cfg.h = 128;
    }
    
    /* Initialize decoder with version checking */
    res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        /* Initialization failed, cleanup and exit */
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    /* Try to set frame buffer functions with some probability based on input */
    if (Size > 0 && (Data[0] & 0x01)) {
        aom_codec_set_frame_buffer_functions(&ctx, dummy_get_frame_buffer, 
                                            dummy_release_frame_buffer, NULL);
    }
    
    /* Decode the provided data if we have any */
    if (Size > 0) {
        /* Use a subset of the data for decoding to explore different states */
        size_t decode_size = Size;
        if (Size > 1024) {
            decode_size = 1024; /* Limit decode size for efficiency */
        }
        
        res = aom_codec_decode(&ctx, Data, decode_size, NULL);
        
        /* Try to get stream info after decoding */
        if (res == AOM_CODEC_OK || res == AOM_CODEC_UNSUP_BITSTREAM) {
            aom_codec_get_stream_info(&ctx, &si);
        }
    } else {
        /* Try to get stream info even without decoding */
        aom_codec_get_stream_info(&ctx, &si);
    }
    
    /* Cleanup */
    aom_codec_destroy(&ctx);
    
    return 0;
}