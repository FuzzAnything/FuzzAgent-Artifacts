// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/* Helper to safely write to a dummy file if needed */
static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_320(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Attempt to write data to dummy file to trigger file-based paths if any */
    write_dummy_file(Data, Size);

    /* Initialize decoder context */
    aom_codec_ctx_t ctx_dec;
    aom_codec_dec_cfg_t cfg_dec;
    memset(&ctx_dec, 0, sizeof(ctx_dec));
    memset(&cfg_dec, 0, sizeof(cfg_dec));
    cfg_dec.w = 64;
    cfg_dec.h = 64;
    cfg_dec.threads = 1;

    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    if (aom_codec_dec_init(&ctx_dec, dec_iface, &cfg_dec, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Initialize encoder context */
    aom_codec_ctx_t ctx_enc;
    aom_codec_enc_cfg_t cfg_enc;
    memset(&ctx_enc, 0, sizeof(ctx_enc));
    memset(&cfg_enc, 0, sizeof(cfg_enc));
    cfg_enc.g_w = 64;
    cfg_enc.g_h = 64;
    cfg_enc.g_timebase.num = 1;
    cfg_enc.g_timebase.den = 30;
    cfg_enc.rc_target_bitrate = 100;
    cfg_enc.g_error_resilient = 1;

    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    if (aom_codec_enc_config_default(enc_iface, &cfg_enc, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    if (aom_codec_enc_init(&ctx_enc, enc_iface, &cfg_enc, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    /* Fuzzing logic for control functions */

    /* 1. AOMD_GET_LAST_QUANTIZER (Decoder) */
    int quantizer_val = 0;
    if (aom_codec_control(&ctx_dec, AOMD_GET_LAST_QUANTIZER, &quantizer_val) != AOM_CODEC_OK) {
        /* Ignore errors, just continue to explore states */
    }

    /* 2. AV1E_GET_LUMA_CDEF_STRENGTH (Encoder) */
    /* The API expects an array of int. CDEF_MAX_STRENGTHS is typically 8.
       However, the stack trace indicates a write of 64 bytes (8 * sizeof(int)) 
       occurred into a buffer that was too small or misaligned. 
       To be safe, we allocate a larger buffer to ensure we cover any potential 
       internal padding or larger constants in different builds. 
       We use 16 ints (64 bytes) to be safe, as the error showed a write of 64 bytes. */
    int *cdef_strengths = malloc(16 * sizeof(int));
    if (cdef_strengths) {
        memset(cdef_strengths, 0, 16 * sizeof(int));
        if (aom_codec_control(&ctx_enc, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths) != AOM_CODEC_OK) {
            /* Ignore errors */
        }
        free(cdef_strengths);
    }

    /* 3. AV1E_SET_FILM_GRAIN_TEST_VECTOR (Encoder) */
    /* We construct a dummy test vector from input data if size permits */
    if (Size >= 16) {
        uint8_t *fg_vector = malloc(16);
        if (fg_vector) {
            memcpy(fg_vector, Data, 16);
            if (aom_codec_control(&ctx_enc, AV1E_SET_FILM_GRAIN_TEST_VECTOR, fg_vector) != AOM_CODEC_OK) {
                /* Ignore errors */
            }
            free(fg_vector);
        }
    }

    /* 4. AOMD_GET_SB_SIZE (Decoder) */
    aom_superblock_size_t sb_size = AOM_SUPERBLOCK_SIZE_DYNAMIC;
    if (aom_codec_control(&ctx_dec, AOMD_GET_SB_SIZE, &sb_size) != AOM_CODEC_OK) {
        /* Ignore errors */
    }

    /* 5. AV1E_SET_INTRA_DEFAULT_TX_ONLY (Encoder) */
    int tx_only = (Size > 0) ? (Data[0] % 2) : 0;
    if (aom_codec_control(&ctx_enc, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &tx_only) != AOM_CODEC_OK) {
        /* Ignore errors */
    }

    /* 6. AV1E_GET_BASELINE_GF_INTERVAL (Encoder) */
    int gf_interval = 0;
    if (aom_codec_control(&ctx_enc, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval) != AOM_CODEC_OK) {
        /* Ignore errors */
    }

    /* Cleanup */
    aom_codec_destroy(&ctx_dec);
    aom_codec_destroy(&ctx_enc);

    return 0;
}