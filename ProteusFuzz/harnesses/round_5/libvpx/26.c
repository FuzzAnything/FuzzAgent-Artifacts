// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

/*
 * Fuzz driver for libvpx focusing on vpx_codec_control_ and related control IDs.
 */

static int fuzz_encoder_control(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    int ctrl_id = (int)(data[0] | (data[1] << 8));
    int value = 0;

    if (size >= 4) {
        value = (int)(data[2] | (data[3] << 8));
    }

    /*
     * Valid control IDs for VP8/VP9 encoders found in vp8cx.h
     */
    int ids_to_try[] = {
        ctrl_id,
        VP9E_SET_DISABLE_LOOPFILTER,
        VP9E_SET_FRAME_PARALLEL_DECODING,
        VP9E_SET_NOISE_SENSITIVITY,
        VP8E_SET_CPUUSED,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_SCALEMODE,
        VP8E_SET_TOKEN_PARTITIONS,
        VP9E_GET_LEVEL,
        -1 // Sentinel
    };

    for (int i = 0; ids_to_try[i] != -1; i++) {
        int id = ids_to_try[i];
        
        /* Skip invalid IDs */
        if (id < 0) continue; 

        vpx_codec_err_t res;
        
        /*
         * Call vpx_codec_control_ with the control ID and a value.
         */
        res = vpx_codec_control_(ctx, id, value);
        
        (void)res; 
    }

    return 1;
}

static int fuzz_decoder_control(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    int ctrl_id = (int)(data[0] | (data[1] << 8));
    int value = 0;

    if (size >= 4) {
        value = (int)(data[2] | (data[3] << 8));
    }

    /*
     * Valid control IDs for VP8 decoders found in vp8dx.h
     * VP8D_SET_ERROR_RESILIENCE was removed as it is not defined in standard headers.
     */
    int ids_to_try[] = {
        ctrl_id,
        VP8D_GET_FRAME_CORRUPTED,
        -1
    };

    for (int i = 0; ids_to_try[i] != -1; i++) {
        int id = ids_to_try[i];
        if (id < 0) continue;

        vpx_codec_err_t res;
        res = vpx_codec_control_(ctx, id, value);
        (void)res;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Try Encoder Context */
    vpx_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    enc_ctx.name = "dummy_encoder";
    
    fuzz_encoder_control(&enc_ctx, Data, Size);

    /* Try Decoder Context */
    vpx_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    dec_ctx.name = "dummy_decoder";

    fuzz_decoder_control(&dec_ctx, Data, Size);

    /* Write input to dummy file as per requirements */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    return 0;
}