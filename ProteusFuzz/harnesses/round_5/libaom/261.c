// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int setup_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    /* Initialize configuration */
    aom_codec_enc_config_default(NULL, cfg, 0);

    /* Use fixed values for width/height to ensure valid initialization */
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    /* Removed g_row_mt as it may not exist in all versions */
    cfg->g_error_resilient = 0;
    /* Fixed AOM_CQP to AOM_CQ */
    cfg->rc_end_usage = AOM_CQ;
    cfg->rc_target_bitrate = 1000;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    err = aom_codec_enc_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static void fuzz_control(aom_codec_ctx_t *codec, int control_id, int value) {
    aom_codec_err_t err;
    
    /* 
     * Invoke the control function. 
     * We pass the value directly. 
     */
    err = aom_codec_control(codec, control_id, value);
    
    /* 
     * We ignore the error code here to allow the fuzzer to explore 
     * invalid states without crashing.
     */
    (void)err;
}

int LLVMFuzzerTestOneInput_261(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int value;
    int idx = 0;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!setup_encoder(&codec, &cfg)) {
        return 0;
    }

    /* 
     * Extract values from input data to fuzz the control parameters.
     */
    while (idx < Size) {
        /* Read a value. If not enough data, stop. */
        if (idx + 4 > Size) break;
        
        value = *(int32_t *)(Data + idx);
        idx += 4;

        /* 
         * Determine which control ID to use based on the next byte.
         */
        int selector = 0;
        if (idx < Size) {
            selector = Data[idx] % 6;
            idx++;
        }

        switch (selector) {
            case 0:
                /* AV1E_SET_QM_U */
                fuzz_control(&codec, AV1E_SET_QM_U, value);
                break;
            case 1:
                /* AV1E_SET_TILE_ROWS */
                fuzz_control(&codec, AV1E_SET_TILE_ROWS, value);
                break;
            case 2:
                /* AV1E_SET_AUTO_TILES */
                fuzz_control(&codec, AV1E_SET_AUTO_TILES, value);
                break;
            case 3:
                /* AV1E_SET_TILE_COLUMNS */
                fuzz_control(&codec, AV1E_SET_TILE_COLUMNS, value);
                break;
            case 4:
                /* AV1E_SET_SINGLE_TILE_DECODING */
                fuzz_control(&codec, AV1E_SET_SINGLE_TILE_DECODING, value);
                break;
            case 5:
                /* AOME_SET_SHARPNESS */
                fuzz_control(&codec, AOME_SET_SHARPNESS, value);
                break;
            default:
                break;
        }
    }

    cleanup_encoder(&codec);
    return 0;
}